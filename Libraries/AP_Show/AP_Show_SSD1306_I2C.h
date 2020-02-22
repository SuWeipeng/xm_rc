#ifndef __AP_SHOW_SSD1306_I2C__
#define __AP_SHOW_SSD1306_I2C__

#include "AP_Show_Backend.h"

class AP_Show_SSD1306_I2C : public AP_Show_Backend
{
public:
  AP_Show_SSD1306_I2C(AP_Show &show);
  ~AP_Show_SSD1306_I2C() {}
  
  virtual void show(uint8_t* str, uint8_t x_pos, uint8_t y_pos);
  virtual void show(uint8_t page_num);
  
private:
  void _refresh();
};

#endif /* __AP_SHOW_SSD1306_I2C__ */
