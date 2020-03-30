#include "rtt_interface.h"
#include <string.h>
#include <entry.h>
#include "AP_Show.h"
#include "AP_Buffer.h"

extern vel_target vel;
extern AP_Show* show;
extern AP_Buffer *buffer;
extern uint8_t key_value;

extern "C"{
char global_buf[4][16];
}

rt_thread_t show_thread = RT_NULL;

extern "C"
void show_thread_entry(void* parameter)
{  
  char line[3][15];
  char head[16];
  int8_t  page_num = 0;
  while(1) {
    // Switch page
    if(key_value == 2 && page_num != 1){
      page_num--;
      if(page_num < 0) page_num = 0;
      show->show_now(page_num);
      key_value = 0;
    } else if(key_value == 12){
      page_num--;
      if(page_num < 0) page_num = 0;
      show->show_now(page_num);
      key_value = 0;
    }

    if(key_value == 1){
      page_num++;
      if(page_num > 1) page_num = 1;
      show->show_now(page_num);
      key_value = 0;
    }
  
    // Show page
    show->show_page(page_num);
    
    // Page 0
    sprintf (line[0], "vel_x:%.3f", vel.vel_x);
    sprintf (line[1], "vel_y:%.3f", vel.vel_y);
    sprintf (line[2], "rad_z:%.3f", vel.rad_z);
    for(uint8_t i=0; i<3; i++){
      show->page_write(0, i, line[i], "rc output");
    }
    
    // Page 1
    sprintf(head, "%s \r\n", "key monitor");
    show->page_write(1, 0, global_buf[0], head);
    show->page_write(1, 2, global_buf[2], head);
    if(key_value == 9){
	memset(global_buf[2], 0, 16);
    }
    
    // update screen
    rt_enter_critical();
    show->update();
    rt_exit_critical();
    
    rt_thread_delay(500);
  }
}
