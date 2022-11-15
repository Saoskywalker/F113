#include "timer_port.h"
#include "types_base.h"

//开启定时器
void MTF_timer_init_handle(void)
{
	OSCCON = 0x50; //系统时钟设置, 内部震荡Fsys=Fosc/4, Fsys源由CONFIG选择

	//设置定时器2
	PR2 = 199; //赋初值
	T2CON = 0B00000001; //模块输入时钟以此为基础(T2 = Fsys/4), 无分频
	TMR2ON = 1; //使能timer2
	TMR2 = 0; //timer2计数器
	TMR2IF = 0; //清中断标志位
	TMR2IE = 1; //开timer2中断

	PEIE = 1; //允许外设所有未被屏蔽中断
	GIE = 1; //开总中断
}

void MTF_timer_start(void)
{
	TMR2ON = 1; //开启定时器
}

void MTF_timer_suspend(void)
{
	TMR2ON = 0; //暂停定时器
}
