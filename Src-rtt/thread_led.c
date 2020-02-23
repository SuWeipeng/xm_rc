#include "rtt_interface.h"

rt_thread_t led_thread = RT_NULL;

void led_thread_entry(void* parameter)
{
  while(1) {
    rt_pin_write(LED_R_PIN, !rt_pin_read(LED_R_PIN));
    rt_thread_delay(500);
  }
}
