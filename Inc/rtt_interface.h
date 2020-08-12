#ifndef __RTT_INTERFACE_H
#define __RTT_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#define AP_BUFFER_MAX_SIZE  (6)
#include <entry.h>

#define AP_BUFFER_MAX_SIZE  (6)
#define LED_R_PIN           GET_PIN(C, 13)
#define K1                  GET_PIN(B, 14)
#define K2                  GET_PIN(B, 15)
#define K3                  GET_PIN(B, 13)
#define K4                  GET_PIN(B, 12)

typedef union{
  struct{
    uint8_t key_value: 3;
    uint8_t w_number : 2;
    uint8_t mode     : 2;
    uint8_t com      : 1;
  };
  uint8_t value;
}ap_t;

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