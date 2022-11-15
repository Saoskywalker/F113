#ifndef _GPIO_BOARD_H
#define _GPIO_BOARD_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "types_base.h"

/****************************
应用定义接口
*****************************/
//io定义
//key
#define POWER_KEY_PIN()  RA1

#define IC4056_STDBY_PIN() RA5
#define ULTRASOUND_PWM(x) RB1 = x
#define IC4056_CHRG_PIN() RA6

#define LED1W_PIN(x) RA0 = x
#define LED1R_PIN(x) RA2 = x
#define EN_5V_PIN(x) RA0 = x
#define LED2W_PIN(x) RB2 = x
#define LED2R_PIN(x) RB3 = x
#define LIGHT_PIN(x) RB4 = x

#define	LED1W_PIN_ON LED1W_PIN(0)
#define	LED1R_PIN_ON LED1R_PIN(0)
#define	LED2W_PIN_ON LED2W_PIN(0)
#define	LED2R_PIN_ON LED2R_PIN(0)
#define	LIGHT_PIN_ON LIGHT_PIN(1)

#define	LED1W_PIN_OFF LED1W_PIN(1)
#define	LED1R_PIN_OFF LED1R_PIN(1)
#define	LED2W_PIN_OFF LED2W_PIN(1)
#define	LED2R_PIN_OFF LED2R_PIN(1)
#define	LIGHT_PIN_OFF LIGHT_PIN(0)

void main_IO_init(void);
void main_IO_exit(void);
///////////////////////////////

#define TEST_TOGGLE_PIN() RC0 = ~RC0 //测试引脚, 翻转
#define TEST_PIN(x) RC0 = x //测试引脚

void gpio_test(void);

#ifdef __cplusplus
}
#endif

#endif
