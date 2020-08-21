#include <entry.h>
#include <stdio.h>
#include "main.h"
#include "RC_Channel.h"
#include "AP_Show.h"
#include "AP_Buffer.h"

extern vel_target vel;
extern uint8_t    key_value;

uint8_t switch_y = 0;

RC_Channel* rc;
AP_Show* show;
AP_Buffer *buffer;

#if defined(__cplusplus)
extern "C" {
#endif

void setup(void)
{
  rc = new RC_Channel();
  show = new AP_Show();
  show->init(AP_Show::SSD1306_OLED_I2C);
  buffer = new AP_Buffer();
  buffer->init(AP_Buffer::FIFO);
}

void loop(void* parameter)
{  
  while(1){
    if(key_value == 2){
      switch_y = !switch_y;
      key_value = 0;
    }
    vel.vel_x = rc->vel_x();
    if(switch_y == 1){
      vel.ext_2 = rc->vel_y(-1);
      vel.vel_y = 0.0f;
    } else {
      vel.vel_y = rc->vel_y(-1);
    }
    vel.rad_z = rc->rad_z(-1);
    vel.ext_1 = rc->ext_1();
    
    rt_thread_delay(1);
  }
}

#if defined(__cplusplus)
}
#endif
