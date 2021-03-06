#include "ssd1306.h"
#include <string.h>
#include <stdio.h>

void ssd1306_TestBorder() {
    SSD1306_Fill(SSD1306_COLOR_BLACK);
   
    uint32_t start = HAL_GetTick();
    uint32_t end = start;
    uint8_t x = 0;
    uint8_t y = 0;
    do {
        ssd1306_DrawPixel(x, y, SSD1306_COLOR_BLACK);

        if((y == 0) && (x < 127))
            x++;
        else if((x == 127) && (y < 63))
            y++;
        else if((y == 63) && (x > 0)) 
            x--;
        else
            y--;

        ssd1306_DrawPixel(x, y, SSD1306_COLOR_WHITE);
        SSD1306_UpdateScreen();
    
        HAL_Delay(5);
        end = HAL_GetTick();
    } while((end - start) < 8000);
   
    HAL_Delay(1000);
}

void ssd1306_TestFonts() {
    SSD1306_Fill(SSD1306_COLOR_BLACK);
    SSD1306_GotoXY(2, 0);
    SSD1306_Puts((char*)"Font 16x26", &Font_16x26, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(2, 26);
    SSD1306_Puts((char*)"Font 11x18", &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(2, 26+18);
    SSD1306_Puts((char*)"Font 7x10", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_UpdateScreen();
}

void ssd1306_TestFPS() {
    SSD1306_Fill(SSD1306_COLOR_WHITE);
   
    uint32_t start = HAL_GetTick();
    uint32_t end = start;
    int fps = 0;
    char message[] = "ABCDEFGHIJK";
   
    SSD1306_GotoXY(2,0);
    SSD1306_Puts((char*)"Testing...", &Font_11x18, SSD1306_COLOR_BLACK);
   
    do {
        SSD1306_GotoXY(2, 18);
        SSD1306_Puts(message, &Font_11x18, SSD1306_COLOR_BLACK);
        SSD1306_UpdateScreen();
       
        char ch = message[0];
        memmove(message, message+1, sizeof(message)-2);
        message[sizeof(message)-2] = ch;

        fps++;
        end = HAL_GetTick();
    } while((end - start) < 5000);
   
    HAL_Delay(1000);

    char buff[64];
    fps =(int) (fps / ((end - start) / 1000.0f));
    snprintf(buff, sizeof(buff), "~%d FPS", fps);
   
    SSD1306_Fill(SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(2, 18);
    SSD1306_Puts(buff, &Font_11x18, SSD1306_COLOR_BLACK);
    SSD1306_UpdateScreen();
}

void ssd1306_TestAll() {
    SSD1306_Init();
    ssd1306_TestFPS();
    HAL_Delay(3000);
    ssd1306_TestBorder();
    ssd1306_TestFonts();
}
