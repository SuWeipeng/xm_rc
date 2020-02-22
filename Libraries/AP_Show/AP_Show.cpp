#include <entry.h>
#include <string.h>
#include "AP_Show.h"
#include "AP_Show_SSD1306_I2C.h"

using namespace rtthread;

AP_Show *AP_Show::_instance;
AP_Show::page_content AP_Show::_content;

AP_Show::AP_Show()
{
  _instance = this;
}

void 
AP_Show::init(void)
{
  for(uint8_t i=0; i<DISP_MAX_DRIVERS; i++){
    _drivers[i] = new AP_Show_SSD1306_I2C(*this);
  }
}

void
AP_Show::show(uint8_t* str, uint8_t x_pos, uint8_t y_pos)
{
  for(uint8_t i=0; i<DISP_MAX_DRIVERS; i++){
    _drivers[i]->show(str, x_pos, y_pos);
  }
}

void
AP_Show::update()
{
  for(uint8_t i=0; i<DISP_MAX_PAGES; i++){
    _update_page_bitmask(i);
    for(uint8_t num=0; num<DISP_MAX_DRIVERS; num++){
	_drivers[0]->update(i);
    }
  }
}

void
AP_Show::page_write(uint8_t page_num, uint8_t line_num, char* content, char* head)
{
  if(page_num > DISP_MAX_PAGES - 1) return;

  strncpy(&_content.page_head[page_num][0], head, DISP_MAX_CHAR_PER_LINE);
  strncpy(&_content.content[page_num * DISP_MAX_LINE_PER_PAGE + line_num][0], content, DISP_MAX_CHAR_PER_LINE);
}

void
AP_Show::_update_page_bitmask(uint8_t page_num)
{
  for(uint8_t i=0; i<DISP_MAX_LINE_PER_PAGE; i++){
    if(strcmp(&_content.content_last[page_num * DISP_MAX_LINE_PER_PAGE + i][0],
	      &_content.content[page_num * DISP_MAX_LINE_PER_PAGE + i][0]) != 0){
      _content.refresh_bitmask |= (0x01 << page_num);
      strncpy(&_content.content_last[page_num * DISP_MAX_LINE_PER_PAGE + i][0],
	      &_content.content[page_num * DISP_MAX_LINE_PER_PAGE + i][0],DISP_MAX_CHAR_PER_LINE);
    }
  }
}
