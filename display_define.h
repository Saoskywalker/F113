#ifndef _DISPLAY_DEFINE_H
#define _DISPLAY_DEFINE_H

#include "display_board.h"

/*****************************
 * 应用对应的led显存功能定义
 * **************************/
// #define Dsp_RESlocate(N) (LED_data_buf[0] &= ~(N))
// #define Dsp_locate(N) LED_data_buf[0] |= N

// #define led1r_locate Dsp_locate(0X01)
// #define led1w_locate Dsp_locate(0X02)
// #define led2r_locate Dsp_locate(0X04)
// #define led2w_locate Dsp_locate(0X08)

#define SCREEN_GRADUAL(x) LED_data_buf[0] = x
#define LED1R_GRADUAL(x) LED_data_buf[1] = x
#define LED1W_GRADUAL(x) LED_data_buf[2] = x
#define LED2R_GRADUAL(x) LED_data_buf[3] = x
#define LED2W_GRADUAL(x) LED_data_buf[4] = x
/////////////////////////////////////

//==============================================================================
//      3) 函数声明(对外接口)
//==============================================================================
void Led_display(void);
void Disp_All(void);

#endif
