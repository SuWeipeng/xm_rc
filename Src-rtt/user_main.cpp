#include "RC_Channel.h"
#include "AP_Show.h"

extern vel_target vel;

RC_Channel* rc;
AP_Show* show;

#if defined(__cplusplus)
extern "C" {
#endif

void setup(void)
{
  rc = new RC_Channel();
  show = new AP_Show();
  show->init();
  show->show((uint8_t*)"head", 10, 0);
  show->show((uint8_t*)"HELLO", 10, 16);
  show->show((uint8_t*)"WORLD", 10, 26);
  show->show((uint8_t*)"STM32", 10, 36);
}

void loop(void)
{
  vel.vel_x = rc->vel_x();
  vel.vel_y = rc->vel_y(-1);
  vel.rad_z = rc->rad_z(-1);
}

#if defined(__cplusplus)
}
#endif
