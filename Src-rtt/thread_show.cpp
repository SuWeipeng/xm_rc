#include "rtt_interface.h"
#include <entry.h>
#include "AP_Show.h"
#include "AP_Buffer.h"

extern vel_target vel;
extern AP_Show* show;
extern AP_Buffer *buffer;

rt_thread_t show_thread = RT_NULL;

extern "C"
void show_thread_entry(void* parameter)
{  
  char line[3][15];
  char c[1];
  char buf[DISP_MAX_CHAR_PER_LINE];
  uint32_t cnt = 0;
  while(1) {
    show->show_page(1);
    
    // Page 0
    sprintf (line[0], "vel_x:%.3f", vel.vel_x);
    sprintf (line[1], "vel_y:%.3f", vel.vel_y);
    sprintf (line[2], "rad_z:%.3f", vel.rad_z);
    for(uint8_t i=0; i<3; i++){
      show->page_write(0, i, line[i], "rc output");
    }
    
    // Page 1
    sprintf(c, "%d", cnt++ % 10);
    buffer->write(c,sizeof(c)); 
    sprintf(buf, "buf :%s \r\n", (uint8_t*)buffer->get_buffer());
    show->page_write(1, 0, buf, "ring fifo");
    sprintf(buf, "len :%d \r\n", buffer->buf_len());
    show->page_write(1, 1, buf, "ring fifo");
    if(cnt%3==0 && buffer->read()>0){
      sprintf(buf, "read:%s \r\n", (uint8_t*)buffer->read_buf_addr());
      show->page_write(1, 2, buf, "ring fifo");
    }
    
    // update screen
    rt_enter_critical();
    show->update();
    rt_exit_critical();
    
    rt_thread_delay(500);
  }
}
