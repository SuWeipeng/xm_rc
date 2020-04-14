/**
 * @File:    demo_rtt_iotboard.c
 * @Author:  MurphyZhao
 * @Date:    2018-09-29
 * 
 * Copyright (c) 2018-2019 MurphyZhao <d2014zjt@163.com>
 *               https://github.com/murphyzhao
 * All rights reserved.
 * License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * message:
 * This demo is base on rt-thread IoT Board, reference
 *     https://github.com/RT-Thread/IoT_Board
 * Hardware version: RT-Thread IoT Board Pandora v2.51.
 * 
 * Change logs:
 * Date        Author       Notes
 * 2018-09-29  MurphyZhao   First add
 * 2019-08-02  MurphyZhao   Migrate code to github.com/murphyzhao account
 * 2020-02-14  MurphyZhao   Fix grammar bug
*/

#include <rtthread.h>
#include <board.h>
#include <rtt_interface.h>

#include "flexible_button.h"

#define ENUM_TO_STR(e) (#e)

extern char global_buf[4][16];

/*
 * 0   - default
 * 1   - K4 clicked
 * 2   - K3 clicked
 * 3   - K2 clicked
 * 4   - K1 clicked
 * 5   - K4 short pressed
 * 6   - K3 short pressed
 * 7   - K2 short pressed
 * 8   - K1 short pressed
 * 9   - K4 and K3 short pressed
 * 10  - K2 and K1 short pressed
 * 11  - K4 double clicked
 * 12  - K3 double clicked
 * 13  - K2 double clicked
 * 14  - K1 double clicked
 * 15  - K4 and K3 clicked
 * 16  - K2 and K1 clicked
 */
uint8_t key_value = 0;

typedef enum
{
    USER_BUTTON_0 = 0,
    USER_BUTTON_1,
    USER_BUTTON_2,
    USER_BUTTON_3,
    USER_BUTTON_MAX
} user_button_t;

static char *enum_event_string[] = {
    ENUM_TO_STR(FLEX_BTN_PRESS_DOWN),
    ENUM_TO_STR(FLEX_BTN_PRESS_CLICK),
    ENUM_TO_STR(FLEX_BTN_PRESS_DOUBLE_CLICK),
    ENUM_TO_STR(FLEX_BTN_PRESS_REPEAT_CLICK),
    ENUM_TO_STR(FLEX_BTN_PRESS_SHORT_START),
    ENUM_TO_STR(FLEX_BTN_PRESS_SHORT_UP),
    ENUM_TO_STR(FLEX_BTN_PRESS_LONG_START),
    ENUM_TO_STR(FLEX_BTN_PRESS_LONG_UP),
    ENUM_TO_STR(FLEX_BTN_PRESS_LONG_HOLD),
    ENUM_TO_STR(FLEX_BTN_PRESS_LONG_HOLD_UP),
    ENUM_TO_STR(FLEX_BTN_PRESS_MAX),
    ENUM_TO_STR(FLEX_BTN_PRESS_NONE),
};

static char *enum_btn_id_string[] = {
    ENUM_TO_STR(USER_BUTTON_0),
    ENUM_TO_STR(USER_BUTTON_1),
    ENUM_TO_STR(USER_BUTTON_2),
    ENUM_TO_STR(USER_BUTTON_3),
    ENUM_TO_STR(USER_BUTTON_MAX),
};

static flex_button_t user_button[USER_BUTTON_MAX];

static uint8_t common_btn_read(void *arg)
{
    uint8_t value = 0;

    flex_button_t *btn = (flex_button_t *)arg;

    switch (btn->id)
    {
    case USER_BUTTON_0:
        value = rt_pin_read(K1);
        break;
    case USER_BUTTON_1:
        value = rt_pin_read(K2);
        break;
    case USER_BUTTON_2:
        value = rt_pin_read(K3);
        break;
    case USER_BUTTON_3:
        value = rt_pin_read(K4);
        break;
    default:
        RT_ASSERT(0);
    }

    return value;
}

static void common_btn_evt_cb(void *arg)
{
    flex_button_t *btn = (flex_button_t *)arg;
    uint8_t tmp_key_value = 0;

    // ignore FLEX_BTN_PRESS_LONG_START and FLEX_BTN_PRESS_LONG_HOLD
    if(btn->event == FLEX_BTN_PRESS_LONG_START || btn->event == FLEX_BTN_PRESS_LONG_HOLD)
      return;

    if(btn->id == 0 && btn->event == FLEX_BTN_PRESS_CLICK && btn->click_cnt == 1){
	tmp_key_value = 1;
    }

    if(btn->id == 1 && btn->event == FLEX_BTN_PRESS_CLICK && btn->click_cnt == 1){
    	tmp_key_value = 2;
    }

    if(btn->id == 2 && btn->event == FLEX_BTN_PRESS_CLICK && btn->click_cnt == 1){
    	tmp_key_value = 3;
    }

    if(btn->id == 3 && btn->event == FLEX_BTN_PRESS_CLICK && btn->click_cnt == 1){
    	tmp_key_value = 4;
    }
    if(btn->id == 0 && btn->event == FLEX_BTN_PRESS_SHORT_START && btn->click_cnt == 0){
	tmp_key_value = 5;
    }

    if(btn->id == 1 && btn->event == FLEX_BTN_PRESS_SHORT_START && btn->click_cnt == 0){
    	tmp_key_value = 6;
    }

    if(btn->id == 2 && btn->event == FLEX_BTN_PRESS_SHORT_START && btn->click_cnt == 0){
    	tmp_key_value = 7;
    }

    if(btn->id == 3 && btn->event == FLEX_BTN_PRESS_SHORT_START && btn->click_cnt == 0){
    	tmp_key_value = 8;
    }

    if ((flex_button_event_read(&user_button[USER_BUTTON_0]) == FLEX_BTN_PRESS_SHORT_START) &&\
        (flex_button_event_read(&user_button[USER_BUTTON_1]) == FLEX_BTN_PRESS_SHORT_START))
    {
	tmp_key_value = 9;
    }

    if ((flex_button_event_read(&user_button[USER_BUTTON_2]) == FLEX_BTN_PRESS_SHORT_START) &&\
        (flex_button_event_read(&user_button[USER_BUTTON_3]) == FLEX_BTN_PRESS_SHORT_START))
    {
	tmp_key_value = 10;
    }

    if(btn->id == 0 && btn->event == FLEX_BTN_PRESS_DOUBLE_CLICK && btn->click_cnt == 2){
	tmp_key_value = 11;
    }

    if(btn->id == 1 && btn->event == FLEX_BTN_PRESS_DOUBLE_CLICK && btn->click_cnt == 2){
    	tmp_key_value = 12;
    }

    if(btn->id == 2 && btn->event == FLEX_BTN_PRESS_DOUBLE_CLICK && btn->click_cnt == 2){
    	tmp_key_value = 13;
    }

    if(btn->id == 3 && btn->event == FLEX_BTN_PRESS_DOUBLE_CLICK && btn->click_cnt == 2){
    	tmp_key_value = 14;
    }

    if ((flex_button_event_read(&user_button[USER_BUTTON_0]) == FLEX_BTN_PRESS_CLICK) &&\
            (flex_button_event_read(&user_button[USER_BUTTON_1]) == FLEX_BTN_PRESS_CLICK))
    {
    	tmp_key_value = 15;
    }

    if ((flex_button_event_read(&user_button[USER_BUTTON_2]) == FLEX_BTN_PRESS_CLICK) &&\
                (flex_button_event_read(&user_button[USER_BUTTON_3]) == FLEX_BTN_PRESS_CLICK))
    {
	tmp_key_value = 16;
    }

    key_value = tmp_key_value;
    rt_kprintf("key_value: %d\n", key_value);
    sprintf(global_buf[0], "key_value: %d", key_value);
}

static void button_scan(void *arg)
{
    while(1)
    {
        flex_button_scan();
        rt_thread_mdelay(20); // 20 ms
    }
}

static void user_button_init(void)
{
    int i;
    
    rt_memset(&user_button[0], 0x0, sizeof(user_button));

    rt_pin_mode(K1, PIN_MODE_INPUT_PULLUP); /* set KEY pin mode to input */
    rt_pin_mode(K2, PIN_MODE_INPUT_PULLUP); /* set KEY pin mode to input */
    rt_pin_mode(K3, PIN_MODE_INPUT_PULLUP); /* set KEY pin mode to input */
    rt_pin_mode(K4, PIN_MODE_INPUT_PULLUP); /* set KEY pin mode to input */

    for (i = 0; i < USER_BUTTON_MAX; i ++)
    {
        user_button[i].id = i;
        user_button[i].usr_button_read = common_btn_read;
        user_button[i].cb = common_btn_evt_cb;
        user_button[i].pressed_logic_level = 0;
        user_button[i].short_press_start_tick = FLEX_MS_TO_SCAN_CNT(1500);
        user_button[i].long_press_start_tick = FLEX_MS_TO_SCAN_CNT(3000);
        user_button[i].long_hold_start_tick = FLEX_MS_TO_SCAN_CNT(4500);

        flex_button_register(&user_button[i]);
    }
}

int flex_button_main(void)
{
    rt_thread_t tid = RT_NULL;

    user_button_init();

    /* Create background ticks thread */
    tid = rt_thread_create("flex_btn", button_scan, RT_NULL, 666, 10, 10);
    if(tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }

    return 0;
}
#ifdef FINSH_USING_MSH
INIT_APP_EXPORT(flex_button_main);
#endif
