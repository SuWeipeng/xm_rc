#include <string.h>
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
AP_Show_SSD1306_I2C::_refresh()
{
  SSD1306_UpdateScreen();
}

void
AP_Show_SSD1306_I2C::show(uint8_t* str, uint8_t x_pos, uint8_t y_pos)
{
  SSD1306_GotoXY(x_pos, y_pos);
  SSD1306_Puts((char*) str, &Font_7x10, SSD1306_COLOR_WHITE);
  _refresh();
}

void
AP_Show_SSD1306_I2C::show(uint8_t page_num)
{
  uint8_t y_pos = 0;
  static uint8_t page_last = 0;

  if(page_last != page_num){
    SSD1306_Fill(SSD1306_COLOR_BLACK);
    _refresh();
    page_last = page_num;
  }

  for(uint8_t i= 0; i<DISP_MAX_LINE_PER_PAGE; i++){
      SSD1306_GotoXY(10, y_pos);
      if(y_pos == 0){
        if(strlen(&_frontend._content.page_head[page_num][0]) != 0){
            SSD1306_Puts(&_frontend._content.page_head[page_num][0], &Font_7x10, SSD1306_COLOR_WHITE);
        }
        y_pos = 16;
        SSD1306_GotoXY(10, y_pos);
      }
      SSD1306_Puts(&_frontend._content.content[page_num * DISP_MAX_LINE_PER_PAGE + i][0], &Font_7x10, SSD1306_COLOR_WHITE);
      y_pos += 10;
  }
  _refresh();
}
