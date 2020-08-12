#include "rtt_interface.h"

extern uint32_t nrf24_timestamp;

rt_thread_t led_thread = RT_NULL;

void led_thread_entry(void* parameter)
{
  while(1) {
    if(HAL_GetTick() - nrf24_timestamp > 500){
      rt_pin_write(LED_R_PIN, 0);
    } else {
      rt_pin_write(LED_R_PIN, 1);
    }
    rt_thread_delay(100);
  }
}
