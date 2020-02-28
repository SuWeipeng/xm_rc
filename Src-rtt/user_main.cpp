#include <entry.h>
#include <stdio.h>
#include "main.h"
#include "RC_Channel.h"
#include "AP_Show.h"
#include "AP_Buffer.h"

extern vel_target vel;

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
  show->init();
  buffer = new AP_Buffer();
  buffer->init(AP_Buffer::RING);
}

void loop(void* parameter)
{  
  while(1){
    vel.vel_x = rc->vel_x();
    vel.vel_y = rc->vel_y(-1);
    vel.rad_z = rc->rad_z(-1);
    
    rt_thread_delay(1);
  }
}

#if defined(__cplusplus)
}
#endif
