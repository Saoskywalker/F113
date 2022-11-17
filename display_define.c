#include "display_define.h"
#include "app_global.h"

static void dis_white_brightness_set(uint8_t brightness)
{
  if (app_work_mode == MODE_A)
  {
    LED1W_GRADUAL(brightness);
  }
  else
  {
    LED1W_GRADUAL(brightness);
    LED2W_GRADUAL(brightness);
  }
}

static void dis_red_brightness_set(uint8_t brightness)
{
  if (app_work_mode == MODE_A)
  {
    LED1R_GRADUAL(brightness);
  }
  else
  {
    LED1R_GRADUAL(brightness);
    LED2R_GRADUAL(brightness);
  }
}

/*************************************************
 // 函数名称    : LedDsp_content
 // 功能描述    : 用户定义显示函数
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
static void LedDsp_content(void)
{
  static uint8_t _dis_charge_500ms = 0, _dis_charge_cnt = 0;
  static uint8_t _flash_cnt = 0, _delay_sleep_cnt = 0;
  static uint8_t _gradual_cnt = 0, _gradual_2_cnt = 0;

  if(app_flag_sys_ready==0)
  {
    Led_dis_All(); //上电准备时全显
    return;
  }

  Led_Clear_All(); //清除所有显示数据

  if (app_flag_light_state)
  {
    SCREEN_GRADUAL(100);
  }

  if (app_flag_usb_insert) //整体策略上禁止充电时睡眠
  {
    _flash_cnt = 0;
    //充电显示
    if (app_flag_work)
    {
      if (app_flag_charge_full==0)
      {
        if (++_gradual_cnt >= 200) //2s
        {
          _gradual_2_cnt = 0;
          _gradual_cnt = 0;
        }
        if (_gradual_cnt >= 100) // 1s
        {
          _gradual_2_cnt -= 1;
          dis_white_brightness_set(_gradual_2_cnt);
        }
        else
        {
          _gradual_2_cnt += 1;
          dis_white_brightness_set(_gradual_2_cnt);
        }
      }
      else
      {
        dis_white_brightness_set(100);
      }
    }
    else
    {
      if (app_flag_charge_full==0)
      {
        LED1R_GRADUAL(100);
      }
    }
  }
  else
  {
    //放电显示
    if (app_battery_level <= BATTERY_LOSE)
    {
      if (++_gradual_cnt >= 200) // 2s
      {
        _gradual_2_cnt = 0;
        _gradual_cnt = 0;
        _flash_cnt++;
      }
      if (_gradual_cnt >= 100) // 1s
      {
        _gradual_2_cnt -= 1;
        dis_red_brightness_set(_gradual_2_cnt);
      }
      else
      {
        _gradual_2_cnt += 1;
        dis_red_brightness_set(_gradual_2_cnt);
      }
    }
    else
    {
      if (app_flag_work)
      {
        dis_white_brightness_set(100);
      }
    }

    if (app_flag_work == 0 && app_flag_light_state == 0)
    {
      if (app_battery_level <= BATTERY_LOSE)
      {
        if (_flash_cnt >= 3) //低电睡眠策略, 闪3次后睡眠
        {
          app_flag_disp_battery_level = 0;
        }
      }
      else
      {
        Led_Clear_All(); //避免特殊情况闪一下
        if (++_delay_sleep_cnt >= 10) //正常睡眠策略, 延时100ms
        {
          app_flag_disp_battery_level = 0;
        }
      }
    }

    if (!app_flag_disp_battery_level)
    {
      Led_Clear_All();
      _flash_cnt = 0;
      _delay_sleep_cnt = 0;
      app_flag_sleep = 1; //进入睡眠命令统一由此发出, 由sleep运行event_handle退出, 流程为, 先停止工作(但可显示), 再进睡眠
    }
  }
}

/*************************************************
 // 函数名称    : Led_display
 // 功能描述    :
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Led_display(void)
{ 
  LedDsp_content();
  Led_Dis_Update();
}
