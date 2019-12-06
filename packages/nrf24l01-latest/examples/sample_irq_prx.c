#include <rtthread.h>
#include "nrf24l01.h"
#include "mavlink.h"
#include "sample.h"
#include "rtt_interface.h"

extern vel_target vel;

static rt_sem_t nrfirq_sem;

static void _irq_init(void);
static void _waitirq(void);

void sample_nrf24l01_task(void *param)
{
    struct hal_nrf24l01_port_cfg halcfg;
    nrf24_cfg_t cfg;
    int rlen;
    uint8_t rbuf[32 + 1];
    uint8_t tbuf[32] = "first\r\n";
    uint32_t cnt = 0;

    _irq_init();

    nrf24_default_param(&cfg);
    halcfg.ce_pin = NRF24L01_CE_PIN;
    halcfg.spi_device_name = NRF24L01_SPI_DEVICE;    
    cfg.role = ROLE_PRX;    // PRX
    cfg.ud = &halcfg;
    cfg.use_irq = 1;        // True    
    nrf24_init(&cfg);

    while (1) {
        rlen = nrf24_irq_prx_run(rbuf, tbuf, 32, _waitirq);
        if (rlen > 0) {       // received data (also indicating that the previous frame of data was sent complete)
            uint8_t i;
            mavlink_message_t msg_receive;
            mavlink_status_t mav_status;
            
            for(i=0; i<32; i++) {
              if(mavlink_parse_char(0, rbuf[i], &msg_receive, &mav_status)) {
                switch (msg_receive.msgid) {
                case MAVLINK_MSG_ID_SIMPLE: {
                  mavlink_message_t msg_ack;
                  mavlink_msg_velocity_pack(0, 0, &msg_ack, vel.vel_x, vel.vel_y, vel.rad_z);
                  mavlink_msg_to_send_buffer((uint8_t *)tbuf, &msg_ack);
                  break;
                }
                }
              }
            }
        }
        else {  // no data
            ;
        }
    }
}

static void _irq_cb(void *param)
{
    rt_sem_release(nrfirq_sem);
}

static void _irq_init(void)
{
    nrfirq_sem = rt_sem_create("nrfIRQ", 0, RT_IPC_FLAG_FIFO);

    rt_pin_attach_irq(NRF24_IRQ_PIN, PIN_IRQ_MODE_FALLING, _irq_cb, 0);
    rt_pin_irq_enable(NRF24_IRQ_PIN, PIN_IRQ_ENABLE);
}

static void _waitirq(void)
{
    rt_sem_take(nrfirq_sem, RT_WAITING_FOREVER);
}

static int nrf24l01_sample_init(void)
{
    rt_thread_t thread;

    thread = rt_thread_create("samNrfPRX", sample_nrf24l01_task, RT_NULL, 2048, 3, 20);
    rt_thread_startup(thread);

    return RT_EOK;
}

INIT_APP_EXPORT(nrf24l01_sample_init);
