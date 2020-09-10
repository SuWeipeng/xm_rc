#include <AP_Math.h>
#include <stdio.h>
#include <entry.h>
#include "RC_Channel.h"

#if ADC_VCOM_DEBUG != 0
extern rt_device_t vcom;
#endif

#define ADC_DEV_NAME        "adc1"
#define ADC_X_MODE1_CHANNEL 8
#define ADC_X_MODE2_CHANNEL 0
#define ADC_Y_CHANNEL       9
#define ADC_Z_CHANNEL       1

RC_Channel::RC_Channel()
{

}

uint32_t RC_Channel::get_value(uint8_t channel)
{
  rt_adc_device_t adc_dev;
  
  adc_dev = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);
  if (adc_dev == RT_NULL)
  {
    rt_kprintf("adc sample run failed! can't find %s device!\n", ADC_DEV_NAME);
    return RT_ERROR;
  }

  rt_adc_enable(adc_dev, ADC_X_MODE1_CHANNEL);
  _adc_buf[3] = rt_adc_read(adc_dev, ADC_X_MODE1_CHANNEL);
  rt_adc_disable(adc_dev, ADC_X_MODE1_CHANNEL);
  
  rt_adc_enable(adc_dev, ADC_X_MODE2_CHANNEL);
  _adc_buf[0] = rt_adc_read(adc_dev, ADC_X_MODE2_CHANNEL);
  rt_adc_disable(adc_dev, ADC_X_MODE2_CHANNEL);
  
  rt_adc_enable(adc_dev, ADC_Y_CHANNEL);
  _adc_buf[2] = rt_adc_read(adc_dev, ADC_Y_CHANNEL);
  rt_adc_disable(adc_dev, ADC_Y_CHANNEL);
  
  rt_adc_enable(adc_dev, ADC_Z_CHANNEL);
  _adc_buf[1] = rt_adc_read(adc_dev, ADC_Z_CHANNEL);
  rt_adc_disable(adc_dev, ADC_Z_CHANNEL);

  if(channel > ADC_CHANNEL_CNT -1) return 0;
#if ADC_VCOM_DEBUG == 2  
  char buf[30];
#if defined(RC_MODE_1)
  sprintf(buf, "x:%d, y:%d, z:%d\r\n", _adc_buf[0], _adc_buf[2], _adc_buf[1]);
#else
  sprintf(buf, "x:%d, y:%d, z:%d\r\n", _adc_buf[3], _adc_buf[2], _adc_buf[1]);
#endif
if(vcom != NULL){
  rt_device_write(vcom, 0, buf, rt_strlen(buf));
}
#endif  
  return _adc_buf[channel];
}

float RC_Channel::vel_x(int8_t inv)
{
#if !defined(RC_MODE_1)
//  inv *= -1;
#endif
  float    ret = 0.0f;
  uint16_t min = 0;
  uint16_t mid = ADC_CHANNEL_X_MID;
  uint16_t max = ADC_CHANNEL_X_MAX;
#if defined(RC_MODE_1)
  uint16_t val = get_value(0);
#else
  uint16_t val = get_value(3);
#endif
  uint16_t mid_zone_up   = mid + ADC_DEAD_ZONE;
  uint16_t mid_zone_down = mid - ADC_DEAD_ZONE;

  if(abs(val-mid) < ADC_DEAD_ZONE) return 0.0f;

  if(val > mid) {
    ret = (val-mid_zone_up)*(VEL_X_MAX_M_S/(max-mid_zone_up));
  } else {
    ret = (val-mid_zone_down)*(VEL_X_MAX_M_S/(mid_zone_down-min));
  }
  
  if(inv == -1) ret *= inv;
  
#if ADC_VCOM_DEBUG == 1  
  if(vcom != NULL){
    char buf[30];
    sprintf(buf, "vel_x:%.3f\r\n", ret);
    rt_device_write(vcom, 0, buf, rt_strlen(buf));
  }
#endif 
  
  return constrain_float(ret, -VEL_X_MAX_M_S, VEL_X_MAX_M_S);
}

float RC_Channel::vel_y(int8_t inv)
{
  float    ret = 0.0f;
  uint16_t min = 0;
  uint16_t mid = ADC_CHANNEL_Y_MID;
  uint16_t max = ADC_CHANNEL_Y_MAX;
  uint16_t val = get_value(2);
  uint16_t mid_zone_up   = mid + ADC_DEAD_ZONE;
  uint16_t mid_zone_down = mid - ADC_DEAD_ZONE;

  if(abs(val-mid) < ADC_DEAD_ZONE) return 0.0f;

  if(val > mid) {
    ret = (val-mid_zone_up)*(VEL_Y_MAX_M_S/(max-mid_zone_up));
  } else {
    ret = (val-mid_zone_down)*(VEL_Y_MAX_M_S/(mid_zone_down-min));
  }
  
  if(inv == -1) ret *= inv;
  
#if ADC_VCOM_DEBUG == 1  
  if(vcom != NULL){
    char buf[30];
    sprintf(buf, "vel_y:%.3f\r\n", ret);
    rt_device_write(vcom, 0, buf, rt_strlen(buf));
  }
#endif  
  
  return constrain_float(ret, -VEL_Y_MAX_M_S, VEL_Y_MAX_M_S);
}

float RC_Channel::rad_z(int8_t inv)
{
  float    ret = 0.0f;
  uint16_t min = 0;
  uint16_t mid = ADC_CHANNEL_Z_MID;
  uint16_t max = ADC_CHANNEL_Z_MAX;
  uint16_t val = get_value(1);
  uint16_t mid_zone_up   = mid + ADC_DEAD_ZONE;
  uint16_t mid_zone_down = mid - ADC_DEAD_ZONE;

  if(abs(val-mid) < ADC_DEAD_ZONE) return 0.0f;

  if(val > mid) {
    ret = (val-mid_zone_up)*(RAD_Z_MAX_RAD_S/(max-mid_zone_up));
  } else {
    ret = (val-mid_zone_down)*(RAD_Z_MAX_RAD_S/(mid_zone_down-min));
  }
  
  if(inv == -1) ret *= inv;
  
#if ADC_VCOM_DEBUG == 1  
  if(vcom != NULL){
    char buf[30];
    sprintf(buf, "rad_z:%.3f\r\n", ret);
    rt_device_write(vcom, 0, buf, rt_strlen(buf));
  }
#endif 
  
  return constrain_float(ret, -RAD_Z_MAX_RAD_S, RAD_Z_MAX_RAD_S);
}

float RC_Channel::ext_1(int8_t inv)
{
#if !defined(RC_MODE_1)
//  inv *= -1;
#endif
  float    ret = 0.0f;
  uint16_t min = 0;
  uint16_t mid = ADC_CHANNEL_X_MID;
  uint16_t max = ADC_CHANNEL_X_MAX;
#if defined(RC_MODE_1)
  uint16_t val = get_value(3);
#else
  uint16_t val = get_value(0);
#endif
  uint16_t mid_zone_up   = mid + ADC_DEAD_ZONE;
  uint16_t mid_zone_down = mid - ADC_DEAD_ZONE;

  if(abs(val-mid) < ADC_DEAD_ZONE) return 0.0f;

  if(val > mid) {
    ret = (val-mid_zone_up)*(VEL_X_MAX_M_S/(max-mid_zone_up));
  } else {
    ret = (val-mid_zone_down)*(VEL_X_MAX_M_S/(mid_zone_down-min));
  }
  
  if(inv == -1) ret *= inv;
  
#if ADC_VCOM_DEBUG == 1  
  if(vcom != NULL){
    char buf[30];
    sprintf(buf, "vel_x:%.3f\r\n", ret);
    rt_device_write(vcom, 0, buf, rt_strlen(buf));
  }
#endif 
  
  return constrain_float(ret, -VEL_X_MAX_M_S, VEL_X_MAX_M_S);
}

