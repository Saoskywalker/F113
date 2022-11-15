#include "gpio_board.h"

void main_IO_init(void)
{
	//此IC只有端口A有电平变化中断

	// RA0: LED1-W OUTPUT
	// RA1: KEY-1 INPUT
	// RA2: LED1-R OUTPUT
	// RA3: 5V-EN OUTPUT
	// RA4: BAT-AD INPUT
	// RA5: INPUT INPUT
	// RA6: CHRG INPUT
	PORTA = 0B00001101;	 // A端口数据寄存器
	ANSEL0 = 0B00010000; // A端口模拟功能寄存器, 默认0, 1设置为模拟功能, 0关闭
	TRISA = 0B01110010;	 //端口A方向寄存器, 默认1, 1高阻输入, 0推挽输出
	WPUA = 0B01000010;	 //端口A输入上拉(输出时自动关闭), 默认0, 1开启, 0关闭
	WPDA = 0;			 //端口A输入下拉(输出时自动关闭), 默认0, 1开启, 0关闭

	IOCA = 0B00100010; //端口A电平变化中断, 默认0, 1开启, 0关闭
	PIE2 |= 0X02;	   //使能PORTA电平变化中断
	PORTA;			   //读取PORTA并锁存

	// RB0: WH-AD INPUT
	// RB1: WH-PWM OUTPUT
	// RB2: LED2-W OUTPUT
	// RB3: LED2-R OUTPUT
	// RB4: LIGHT OUTPUT
	PORTB = 0B00001100; // B端口数据寄存器
	ANSEL1 = 0;			// B端口模拟功能寄存器, 默认0, 1设置为模拟功能, 0关闭, B端口注意规格书位顺序描述有误?
	TRISB = 0B00000001; //端口B方向寄存器, 默认1, 1高阻输入, 0推挽输出
	WPUB = 0;			//端口B输入上拉(输出时自动关闭), 默认0, 1开启, 0关闭
	WPDB = 0;			//端口B输入下拉(输出时自动关闭), 默认0, 1开启, 0关闭

	// RC0: TX-DAT OUTPUT
	// RC1: RX-CLK OUTPUT
	PORTC = 0;			// C端口数据寄存器
	ANSEL2 = 0;			// C端口模拟功能寄存器, 默认0, 1设置为模拟功能, 0关闭, C端口注意规格书位顺序描述有误?
	TRISC = 0; //端口C方向寄存器, 默认1, 1高阻输入, 0推挽输出
	WPUC = 0;			//端口C输入上拉(输出时自动关闭), 默认0, 1开启, 0关闭
	WPDC = 0;			//端口C输入下拉(输出时自动关闭), 默认0, 1开启, 0关闭
}

//睡眠前设置gpio
void main_IO_exit(void)
{
	EN_5V_PIN(0);
}
