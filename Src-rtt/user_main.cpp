#include <entry.h>
#include <stdio.h>
#include "main.h"
#include "RC_Channel.h"
#include "AP_Show.h"

extern vel_target vel;

RC_Channel* rc;
AP_Show* show;

#if defined(__cplusplus)
extern "C" {
#endif

void setup(void)
{
  rc = new RC_Channel();
  show = new AP_Show();
  show->init();
}

void loop(void* parameter)
{
  uint8_t i = 0;
  uint32_t cnt = 0;
  
  while(1){
    vel.vel_x = rc->vel_x();
    vel.vel_y = rc->vel_y(-1);
    vel.rad_z = rc->rad_z(-1);
    
    i++;
    if(i>7) i=0;
    show->show_page(i);
    char buf[15];
    char head[15];
    sprintf (buf, "cnt:%d", cnt++);
    sprintf (head, "page:%d", i);
    if(i%2==0){
      show->page_write(i, i%4, buf, head);
    } else {
      show->page_write(i, i%4, buf);
    }
    show->update();
    
    rt_thread_delay(500);
  }
}

#if defined(__cplusplus)
}
#endif
