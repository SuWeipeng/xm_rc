#ifndef __AP_SHOW__
#define __AP_SHOW__

#include <stdint.h>

#define DISP_MAX_DRIVERS 1

class AP_Show_Backend;

class AP_Show
{
  friend class AP_Show_Backend;
public:
  AP_Show();
  ~AP_Show() {}
  
  // get singleton
  static AP_Show *get_instance(void) {
    return _instance;
  }
  
  void init(void);
  void show(uint8_t* str, uint8_t x_pos, uint8_t y_pos);
  
private:
  static AP_Show *_instance;
  AP_Show_Backend *_drivers[DISP_MAX_DRIVERS];
};

#endif /* __AP_SHOW__ */
