#ifndef __AP_SHOW_BACKEND__
#define __AP_SHOW_BACKEND__

#include <stdint.h>
#include "AP_Show.h"

class AP_Show_Backend
{
public:
  AP_Show_Backend(AP_Show &show);
  ~AP_Show_Backend() {}
  virtual void show(uint8_t* str, uint8_t x_pos, uint8_t y_pos) = 0;
  
private:
  AP_Show &_frontend;
};

#endif /* __AP_SHOW_BACKEND__ */
