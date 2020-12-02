#ifndef __RC_CHANNEL__
#define __RC_CHANNEL__

#include <stdint.h>
#include "rtt_interface.h"

//#define RC_MODE_1
#define PCT_MODE

#define ADC_CHANNEL_CNT 4
#define ADC_BUFF_LEN    ADC_CHANNEL_CNT*2
#define ADC_DEAD_ZONE   300
#define ADC_VCOM_DEBUG  1

#define ADC_CHANNEL_X_MID 1985
#define ADC_CHANNEL_Y_MID 1960
#define ADC_CHANNEL_Z_MID 1926
#define ADC_CHANNEL_X_MAX 4023
#define ADC_CHANNEL_Y_MAX 4012
#define ADC_CHANNEL_Z_MAX 4007

#if !defined(PCT_MODE)
#define VEL_X_MAX_M_S   0.104f
#define VEL_Y_MAX_M_S   0.104f
#define RAD_Z_MAX_RAD_S 0.7f
#else
#define VEL_X_MAX_M_S   1.0f
#define VEL_Y_MAX_M_S   1.0f
#define RAD_Z_MAX_RAD_S 1.0f
#endif

class RC_Channel
{
public:
  RC_Channel();
  ~RC_Channel() {}
  
  void     adc_update(void);
  uint32_t get_value(uint8_t channel);
  float    vel_x(int8_t inv = 1);
  float    vel_y(int8_t inv = 1);
  float    rad_z(int8_t inv = 1);
  float    ext_1(int8_t inv = 1);
  
private:
  uint32_t           _adc_buf[ADC_BUFF_LEN];
};

#endif /* __RC_CHANNEL__ */
