#include "AP_Show_Backend.h"

AP_Show_Backend::AP_Show_Backend(AP_Show &show)
: _frontend(show)
{

}

void
AP_Show_Backend::update(uint8_t i)
{
  if(i > DISP_MAX_PAGES - 1) return;

  if(i == _frontend._content.current_page){
    if(_frontend._content.refresh_bitmask & (0x01 << i)){
      show(i);
      _frontend._content.refresh_bitmask &= ~(0x01 << i);
    }
  }
}
