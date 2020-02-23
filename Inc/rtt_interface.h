#ifndef __RTT_INTERFACE_H
#define __RTT_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <entry.h>

#define LED_R_PIN    GET_PIN(C, 13)
#define K4           GET_PIN(B, 12)

typedef struct vel_target {
  float vel_x; // m/s
  float vel_y; // m/s
  float rad_z; // rad/s
} vel_target;

extern rt_thread_t led_thread;
extern rt_thread_t show_thread;

void led_thread_entry(void* parameter);
void show_thread_entry(void* parameter);

int loop_start(void);

#ifdef __cplusplus
}
#endif

#endif /* __RTT_INTERFACE_H */