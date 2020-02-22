#include <entry.h>
#include "AP_Show.h"
#include "AP_Show_SSD1306_I2C.h"

using namespace rtthread;

AP_Show *AP_Show::_instance;

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