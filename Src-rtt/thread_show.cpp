#include "rtt_interface.h"
#include <entry.h>
#include "AP_Show.h"

extern vel_target vel;
extern AP_Show* show;

rt_thread_t show_thread = RT_NULL;

extern "C"
void show_thread_entry(void* parameter)
{  
  char line[3][15];
  while(1) {
    show->show_page(0);
    sprintf (line[0], "vel_x:%.3f", vel.vel_x);
    sprintf (line[1], "vel_y:%.3f", vel.vel_y);
    sprintf (line[2], "rad_z:%.3f", vel.rad_z);
    for(uint8_t i=0; i<3; i++){
      show->page_write(0, i, line[i], "rc output");
    }
    
    rt_enter_critical();
    show->update();
    rt_exit_critical();
    
    rt_thread_delay(100);
  }
}
