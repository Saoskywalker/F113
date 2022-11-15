#include "display_board.h"

static uint8_t dis_start = 0;

/*************************************************
 // 函数名称    : uint8_t Led_Display_init(void)
 // 功能描述    : 开启LED显示
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Led_Display_init(void)
{
  dis_start = 1;
}

/*************************************************
 // 函数名称    : uint8_t Led_Display_exit(void)
 // 功能描述    : 关闭LED显示
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Led_Display_exit(void)
{
  dis_start = 0;
  LED1W_PIN_OFF;
  LED1R_PIN_OFF;
  LED2W_PIN_OFF;
  LED2R_PIN_OFF;
  LIGHT_PIN_OFF;
}

/*************************************************
 // 函数名称    : Led_Dis_Update
 // 功能描述    : 显存更新到led
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Led_Dis_Update(void)
{
    LED_data = LED_data_buf;
}

/*************************************************
 // 函数名称    : Led_Clear_All
 // 功能描述    : 清除所有显示
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Led_Clear_All(void)
{
  LED_data_buf = 0;
}

/*************************************************
 // 函数名称    : Led_dis_All
 // 功能描述    : 所有显示
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
void Led_dis_All(void)
{
    LED_data_buf = 0XFF;
}

/*************************************************
 // 函数名称    : Led_scan
 // 功能描述    : 此函数在定时中执行
 // 入口参数    : 无
 // 出口参数    : 无
***************************************************/
uint8_t LED_data = 0; //数码管显示输出缓存
uint8_t LED_data_buf = 0; //LED显示data
void Led_Scan(void)
{
  if (dis_start==0)
    return;
  
  if (LED_data&0X01)
    LED1W_PIN_ON;
  else
    LED1W_PIN_OFF;

  if (LED_data&0X02)
    LED1R_PIN_ON;
  else
    LED1R_PIN_OFF;

  if (LED_data&0X04)
    LED2W_PIN_ON;
  else
    LED2W_PIN_OFF;

  if (LED_data&0X08)
    LED2R_PIN_ON;
  else
    LED2R_PIN_OFF;

  if (LED_data&0X10)
    LIGHT_PIN_ON;
  else
    LIGHT_PIN_OFF;
}
