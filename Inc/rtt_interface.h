#ifndef __RTT_INTERFACE_H
#define __RTT_INTERFACE_H

#include <entry.h>

#define LED_R_PIN    GET_PIN(C, 13)

typedef struct vel_target {
  float vel_x; // m/s
  float vel_y; // m/s
  float rad_z; // rad/s
} vel_target;

extern rt_thread_t led_thread;

void led_thread_entry(void* parameter);

#endif /* __RTT_INTERFACE_H */