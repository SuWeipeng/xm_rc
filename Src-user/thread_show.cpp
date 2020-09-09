#include "rtt_interface.h"
#include <string.h>
#include <entry.h>
#include "AP_Show.h"
#include "AP_Buffer.h"

extern vel_target vel;
extern AP_Show*   show;
extern AP_Buffer* buffer;
extern uint8_t    key_value;
extern ap_t       mav_data;
extern uint8_t    mode_changed;
extern int8_t     car_mode;
extern uint8_t    switch_y;

extern "C"{
char global_buf[4][16];
}

rt_thread_t show_thread = RT_NULL;

extern "C"
void show_thread_entry(void* parameter)
{  
  char line[4][15];
  char head[16];
  char mode_page[16];
  int8_t  page_num = 0;
  uint8_t page_max = 2;
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
      if(page_num > page_max) page_num = page_max;
      show->show_now(page_num);
      key_value = 0;
    }
  
    // Show page
    show->show_page(page_num);
    
    // Page 0
    sprintf (line[0], "vel_x:%.3f", vel.vel_x);
    if(switch_y == 1){
      sprintf (line[1], "ext_2:%.3f", vel.ext_2);
    } else {
      sprintf (line[1], "vel_y:%.3f", vel.vel_y);
    }
    sprintf (line[2], "rad_z:%.3f", vel.rad_z);
    sprintf (line[3], "ext_1:%.3f", vel.ext_1);
    for(uint8_t i=0; i<4; i++){
      show->page_write(0, i, line[i], "rc output");
    }
    
    // Page 1
    sprintf(head, "%s \r\n", "key monitor");
    show->page_write(1, 0, global_buf[0], head);
    show->page_write(1, 2, global_buf[2], head);
    if(key_value == 15){
	memset(global_buf[2], 0, 16);
    }
    
    // Page 2
    static int8_t prev_mode;
    char mode_name[6];
    switch(mav_data.mode){
    case 0:{
      sprintf (mode_name, "%s", "Manual");
      break;
    }
    case 1:{
      sprintf (mode_name, "%s", "Auto");
      break;
    }
    case 2:{
      sprintf (mode_name, "%s", "ROS");
      break;
    }
    }
    sprintf (mode_page, "Mode:%s", mode_name);
    show->page_write(2, 0, mode_page, "car mode");
    if(page_num == 2){
      switch(key_value){
      case 3:{
        car_mode++;
        if(car_mode > 2) car_mode = 2;
        break;
      }
      case 4:{
        car_mode--;
        if(car_mode < 0) car_mode = 0;
        break;
      }
      default:{
        break;
      }
      }
      if(prev_mode != car_mode){
        mode_changed = 1;
        prev_mode = car_mode;
      }
    }
    
    // update screen
    rt_enter_critical();
    show->update();
    rt_exit_critical();
    
    rt_thread_delay(500);
  }
}
