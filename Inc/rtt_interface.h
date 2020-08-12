#ifndef __RTT_INTERFACE_H
#define __RTT_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#define AP_BUFFER_MAX_SIZE  (6)
#include <entry.h>

#define LED_R_PIN           GET_PIN(B, 7)
#define LED_B_PIN           GET_PIN(B, 3)
#define K1                  GET_PIN(A, 8)
#define K2                  GET_PIN(B, 10)
#define K3                  GET_PIN(B, 11)
#define K4                  GET_PIN(C, 15)

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