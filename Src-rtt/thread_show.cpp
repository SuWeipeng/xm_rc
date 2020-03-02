#include "rtt_interface.h"
#include <entry.h>
#include "AP_Show.h"
#include "AP_Buffer.h"

extern vel_target vel;
extern AP_Show* show;
extern AP_Buffer *buffer;
extern uint8_t k1_pressed;
extern uint8_t k2_pressed;

rt_thread_t show_thread = RT_NULL;

extern "C"
void show_thread_entry(void* parameter)
{  
  char line[3][15];
  char c[1];
  char buf[DISP_MAX_CHAR_PER_LINE];
  char head[16];
  uint32_t cnt = 0;
  int8_t  page_num = 0;
  while(1) {
    // Switch page
    if(k1_pressed){
      page_num--;
      if(page_num < 0) page_num = 0;
      show->show_now(page_num);
      k1_pressed = 0;
    }
    if(k2_pressed){
      page_num++;
      if(page_num > 1) page_num = 1;
      show->show_now(page_num);
      k2_pressed = 0;
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
    switch(buffer->get_buf_type()){
    case AP_Buffer::RING:
      sprintf(head, "%s \r\n", "ring fifo");
      break;
    case AP_Buffer::FIFO:
      sprintf(head, "%s \r\n", "fifo");
      break; 
    }
    sprintf(c, "%d", cnt++ % 10);
    buffer->write(c,sizeof(c)); 
//    buffer->write("buffer",6); 
    sprintf(buf, "buf :%s \r\n", (uint8_t*)buffer->get_buffer());
    show->page_write(1, 0, buf, head);
    sprintf(buf, "len :%d \r\n", buffer->buf_len());
    show->page_write(1, 1, buf, head);
    if(cnt%3==0 && buffer->read()>0){
      sprintf(buf, "read:%s \r\n", (uint8_t*)buffer->read_buf_addr());
      show->page_write(1, 2, buf, head);
    }
    
    // update screen
    rt_enter_critical();
    show->update();
    rt_exit_critical();
    
    rt_thread_delay(100);
  }
}
