#include <rtthread.h>
#include "nrf24l01.h"
#include "mavlink.h"
#include "sample.h"

#define GET_BIT(value, i) ((value)>>i)

typedef struct vel_target {
  float vel_x; // m/s
  float vel_y; // m/s
  float rad_z; // rad/s
} vel_target;

static rt_sem_t nrfirq_sem;

static void _irq_init(void);
static void _waitirq(void);
static void _nrf24_param_set();

vel_target vel={.vel_x = 1.0f, .vel_y = 2.0f, .rad_z = 3.0f};

void nrf24l01_mavlink_entry(void *param)
{
//  struct hal_nrf24l01_port_cfg halcfg;
//  nrf24_cfg_t cfg;
//  int rlen;
//  static uint8_t tlen = 0;
//  uint8_t rbuf[32 + 1];
//  uint8_t tbuf[32] = "first\r\n";
//
//  _irq_init();
//
//  _nrf24_param_set(&cfg);
//  halcfg.ce_pin = NRF24L01_CE_PIN;
//  halcfg.spi_device_name = NRF24L01_SPI_DEVICE;
//  cfg.role = ROLE_PRX;    // PRX
//  cfg.ud = &halcfg;
//  cfg.use_irq = 1;        // True
//  nrf24_init(&cfg);
//
//  while (1) {
//    rlen = nrf24_irq_prx_run(rbuf, tbuf, tlen, _waitirq);
//    if (rlen > 0) {       // received data (also indicating that the previous frame of data was sent complete)
//      uint8_t i;
//      mavlink_message_t msg_receive;
//      mavlink_status_t mav_status;
//      for(i=0; i<32; i++) {
//        if(mavlink_parse_char(0, rbuf[i], &msg_receive, &mav_status)) {
//          switch (msg_receive.msgid) {
//          case MAVLINK_MSG_ID_SIMPLE: {
//            mavlink_simple_t packet;
//            mavlink_msg_simple_decode(&msg_receive, &packet);
//
//            mavlink_message_t msg_ack;
//            mavlink_msg_velocity_pack(0, 0, &msg_ack, vel.vel_x, vel.vel_y, vel.rad_z);
//            tlen = mavlink_msg_to_send_buffer((uint8_t *)tbuf, &msg_ack);
//            break;
//          }
//          }
//        }
//      }
//    }
//  }
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

static void _nrf24_param_set(nrf24_cfg_t *pt)
{
  pt->power = RF_POWER_0dBm;
  pt->esb.ard = 5;        // (5+1)*250 = 1500us
  pt->esb.arc = 6;        // up to 6 times
  pt->crc = CRC_2_BYTE;   // crc; fcs is two bytes
  pt->adr = ADR_1Mbps;    // air data rate 1Mbps
  pt->channel = 6;        // rf channel 6

  pt->address[4] = 0x12;  // address is 0x123456789A
  pt->address[3] = 0x34;
  pt->address[2] = 0x56;
  pt->address[1] = 0x78;
  pt->address[0] = 0x9A;
}

static int nrf24l01_mavlink_init(void)
{
  rt_thread_t thread;

  thread = rt_thread_create("nrf_mav", nrf24l01_mavlink_entry, RT_NULL, 1024, 3, 20);
  rt_thread_startup(thread);

  return RT_EOK;
}

INIT_APP_EXPORT(nrf24l01_mavlink_init);

