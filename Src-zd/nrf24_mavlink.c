#include <nrf24l01.h>
#include <entry.h>
#include <rtt_interface.h>
#include <mavlink.h>

#define NRF24_DEMO_ROLE     ROLE_PRX

#define NRF24_DEMO_SPI_DEV_NAME         "spi10"
#define NRF24_DEMO_CE_PIN               GET_PIN(A, 3)
#define NRF24_DEMO_IRQ_PIN              GET_PIN(C, 13)

static uint8_t  cmd_cnt = 0;

extern uint8_t key_value;
extern char global_buf[4][16];

uint32_t   nrf24_timestamp;
ap_t       mav_data;
uint8_t    mode_changed;
uint8_t    tlen;
int8_t     car_mode;
vel_target vel={.vel_x = 0.0f, .vel_y = 0.0f, .rad_z = 0.0f};
char tbuf[32];

static void rx_ind(nrf24_t nrf24, uint8_t *data, uint8_t len, int pipe)
{
  if (len > 0) {
    uint8_t i;
    mavlink_message_t msg_receive;
    mavlink_status_t mav_status;
    for(i=0; i<32; i++) {
      if(mavlink_parse_char(0, data[i], &msg_receive, &mav_status)) {
        switch (msg_receive.msgid) {
        case MAVLINK_MSG_ID_SIMPLE: {
          mavlink_simple_t packet;
          mavlink_msg_simple_decode(&msg_receive, &packet);
          
          nrf24_timestamp = HAL_GetTick();
          
          memcpy(&mav_data, &packet.data, sizeof(packet.data));
          
#if MAVLINK_VCOM_DEBUG == 1
          if(vcom != NULL){
            char buf[32];
            sprintf(buf, "%d\r\n", packet.data);
            rt_device_write(vcom, 0, buf, rt_strlen(buf));
          }
#endif
          
          mavlink_message_t msg_ack;
          
          if(key_value == 11 || cmd_cnt != 0){
            mavlink_msg_cmd_pack( 0, 0, &msg_ack, 1 );
            cmd_cnt++;
            sprintf(global_buf[2], "nrf24: cnt %d \r\n", cmd_cnt);
            if(cmd_cnt > 59){
              cmd_cnt = 0;
            }
            key_value = 0;
          } else if(mode_changed){
            mavlink_msg_mode_pack( 0, 0, &msg_ack, car_mode, 0);
            mode_changed = 0;
            key_value    = 0;
          } else {
            mavlink_msg_velocity_pack(0, 0, &msg_ack, vel.vel_x, vel.vel_y, vel.rad_z);
          }
          tlen = mavlink_msg_to_send_buffer((uint8_t *)tbuf, &msg_ack);
          break;
        }
        }
      }
    }
  }
}

static void tx_done(nrf24_t nrf24, int pipe)
{
    /*! Here just want to tell the user when the role is ROLE_PTX
    the pipe have no special meaning except indicating (send) FAILED or OK 
        However, it will matter when the role is ROLE_PRX*/
    
    nrf24_send_data(nrf24, (uint8_t *)tbuf, tlen, pipe);
}

const static struct nrf24_callback _cb = {
    .rx_ind = rx_ind,
    .tx_done = tx_done,
};

static void thread_entry(void *param)
{
    nrf24_t nrf24;

    rt_kprintf("[nrf24/demo] Version:%s\n", PKG_NRF24L01_VERSION);

    nrf24 = nrf24_default_create(NRF24_DEMO_SPI_DEV_NAME, NRF24_DEMO_CE_PIN, NRF24_DEMO_IRQ_PIN, &_cb, NRF24_DEMO_ROLE);

    if (nrf24 == RT_NULL)
    {
        rt_kprintf("\n[nrf24/demo] Failed to create nrf24. stop!\n");
        for(;;) rt_thread_mdelay(10000);
    }
    else
    {
        rt_kprintf("[nrf24/demo] running.");
    }

    nrf24_send_data(nrf24, "Hi\n", 3, NRF24_DEFAULT_PIPE);

    while (1)
    {
        nrf24_run(nrf24);

        if(!nrf24->flags.using_irq)
            rt_thread_mdelay(10);
    }
    
}

static int nrf24l01_sample_init(void)
{
    rt_thread_t thread;

    thread = rt_thread_create("nrf_mav", thread_entry, RT_NULL, 1024, RT_THREAD_PRIORITY_MAX/2, 20);
    rt_thread_startup(thread);

    return RT_EOK;
}

INIT_APP_EXPORT(nrf24l01_sample_init);
