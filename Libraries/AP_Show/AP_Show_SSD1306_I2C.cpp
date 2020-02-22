#include "stm32f1xx_hal.h"
#include "ssd1306.h"
#include "AP_Show_SSD1306_I2C.h"

AP_Show_SSD1306_I2C::AP_Show_SSD1306_I2C(AP_Show &show)
: AP_Show_Backend(show)
{
  uint8_t check = SSD1306_Init();
  while(check==0){
    HAL_Delay(1);
  }	
  SSD1306_Fill(SSD1306_COLOR_BLACK);
  _refresh();
}

void
AP_Show_SSD1306_I2C::show(uint8_t* str, uint8_t x_pos, uint8_t y_pos)
{
  SSD1306_GotoXY(x_pos, y_pos);
  SSD1306_Puts((char*) str, &Font_11x18, SSD1306_COLOR_WHITE);
  _refresh();
}

void
AP_Show_SSD1306_I2C::_refresh()
{
	SSD1306_UpdateScreen();
}