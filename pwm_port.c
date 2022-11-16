#include "pwm_port.h"
#include "gpio_board.h"

uint8_t MM_pwm1_init(void)
{
    //将对应引脚TRIS配置为0并开启PWM功能, 即相应引脚为PWM输出
    TRISB &= ~0B00000010; //RB1

    //IC PWM均为10BIT
    //配置PWM1
	PWMCON0 = 0B00000000; //时钟分频Fosc/1
	PWMCON1 = 0B01000000; //PWM IO配置在B组, 关闭死区, 即关闭互补输出
	PWMCON2 = 0B00000000; //关闭PWM反向输出

    MM_pwm1_set(145, 0); //周期: 110K
	
    MM_pwm1_start(); //开启PWM1

    // PWM4TL = 64;             //PWM4周期低8
	// PWMD4L = 5;               //PWM4占空比低8
	// PWM4EN=1;		//开启PWM4

	return 0;
}

uint8_t MM_pwm1_exit(void)
{
	return 0;
}

void MM_pwm1_start(void)
{
    PWM1EN = 1; //开启PWM0
}

void MM_pwm1_suspend(void)
{
    PWM1EN = 0; //关闭PWM0
    ULTRASOUND_PWM(0);
}

/**********************************
 * period: PWM周期, 传入0XFFFF不改变状态
 * Duty: PWM占空比, 传入0XFFFF不改变状态
 * ******************************/
void MM_pwm1_set(uint16_t Period, uint16_t Duty)
{
    uint8_t i = 0;

    //PWM0~3共用周期, PWM4独立
    //PWM占空比设置顺序, 先高位后低位
    if (Period != 0XFFFF)
    {
        i = PWMTH;
        i &= ~0X03;
        i |= (Period>>8)&0X03;
        PWMTH = i; //Bit1~Bit0 PWMT[9:8]: PWM0~PWM3周期高2位
        PWMTL = (uint8_t)Period; //PWM0~3周期低8位
    }

    if (Duty != 0XFFFF)
    {
        if (Duty == 0)
        {
            PWM1EN = 0; //关闭PWM0
            ULTRASOUND_PWM(0);
        }
        else
        {
            i = PWMD01H;
            i &= ~0X30;
            i |= (Duty & 0X0300) >> 4;
            PWMD01H = i;            // PWM1[5:4]和PWM0[1:0] 占空比高2
            PWMD1L = (uint8_t)Duty; // PWM1占空比低8
        }
    }
}

/**********************************
 * Duty: PWM占空比, 传入0XFFFF不改变状态
 * ******************************/
void MM_pwm1_set_duty(uint16_t Duty)
{
    uint8_t i = 0;

    if (Duty != 0XFFFF)
    {
        if (Duty == 0)
        {
            PWM1EN = 0; //关闭PWM0
            ULTRASOUND_PWM(0);
        }
        else
        {
            i = PWMD01H;
            i &= ~0X30;
            i |= (Duty & 0X0300) >> 4;
            PWMD01H = i;
            PWMD1L = (uint8_t)Duty;
        }
    }
}
