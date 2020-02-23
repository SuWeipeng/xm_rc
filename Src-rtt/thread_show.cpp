#include "rtt_interface.h"
#include <entry.h>
#include "AP_Show.h"

extern AP_Show* show;

rt_thread_t show_thread = RT_NULL;

extern "C"
void show_thread_entry(void* parameter)
{
  uint8_t i = 0;
  uint32_t cnt = 0;
  
  while(1) {
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
