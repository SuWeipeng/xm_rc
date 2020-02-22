#ifndef __AP_SHOW__
#define __AP_SHOW__

#include <stdint.h>
#include <stddef.h>

#define DISP_MAX_DRIVERS          1
#define DISP_MAX_PAGES            8
#define DISP_MAX_LINE_PER_PAGE    4
#define DISP_MAX_CHAR_PER_LINE    16

class AP_Show_Backend;

class AP_Show
{
  friend class AP_Show_Backend;
  friend class AP_Show_SSD1306_I2C;
public:
  AP_Show();
  ~AP_Show() {}
  
  typedef struct PageContent {
    uint8_t refresh_bitmask;
    uint8_t current_page;
    char  page_head[DISP_MAX_PAGES][DISP_MAX_CHAR_PER_LINE];
    char  content[DISP_MAX_PAGES * DISP_MAX_LINE_PER_PAGE][DISP_MAX_CHAR_PER_LINE];
    char  content_last[DISP_MAX_PAGES * DISP_MAX_LINE_PER_PAGE][DISP_MAX_CHAR_PER_LINE];
  }page_content;

  // get singleton
  static AP_Show *get_instance(void) {
    return _instance;
  }
  
  void init(void);
  void show(uint8_t* str, uint8_t x_pos, uint8_t y_pos);
  void update();
  void page_write(uint8_t page_num, uint8_t line_num, char* content, char* head="");
  void show_page(uint8_t page_num) { _content.current_page = page_num; }

private:
  static AP_Show*     _instance;
  static page_content _content;
  AP_Show_Backend*    _drivers[DISP_MAX_DRIVERS];

  void _update_page_bitmask(uint8_t page_num);
};

#endif /* __AP_SHOW__ */
