#include "app_global.h"
#include "types_base.h"
#include "timer_port.h"
#include "gpio_board.h"
#include "pwm_board.h"
#include "adc_board.h"
#include "watch_dog_port.h"
#include "display_define.h"
#include "envent_define.h"
#include "app_timer.h"

// app_flag_type app_flag_= 0;//{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
bit app_flag_sleep; //睡眠
bit app_flag_sys_ready; //系统准备完毕
bit app_flag_work; //工作
bit app_flag_error; //系统错误
bit app_flag_sleep_updata; //用于标记在唤醒后一段时间内, 进行一定的唤醒处理
bit app_flag_nc1;
bit app_flag_usb_insert; //充电线插入
bit app_flag_charge_full; //充满电
bit app_flag_ntc_error; //ntc AD错误
bit app_flag_current_error; //电流AD错误
bit app_flag_battery_error; //电池AD错误
bit app_flag_temp_unit_C; //系统温度单位
bit app_flag_disp_battery_level; //显示电池电量
bit app_flag_nc2;
bit app_flag_nc3;
bit app_flag_nc4;
uint8_t app_mode = MODE_A;
uint8_t app_battery_level = BATTERY_FULL;

/*****************************
 * 电机控制输出
 * ********************************/
static void motor_function(void)
{
	if (app_flag_work)
	{
		PWM_ULTRASOUND_SET_DUTY(PWM_DUTY_50);
	}
	else
	{
		PWM_ULTRASOUND_SET_DUTY(PWM_DUTY_0);
	}
}

/**************
 * 电池电量处理
 * *****************/

static void battery_deal(void)
{	
	static uint16_t __temp1 = 1000, __temp2 = 1000;
	static uint8_t __cnt = 0;

	//电池AD = 1.2*4096/VDD
	if (app_flag_sys_ready == 0)// || app_flag_sleep_updata)
	{
		if (ADC_BATTERY_VALUE() <= 702) //7V voltage over high
		{
			app_battery_level = BATTERY_HIGH;
		}
		else if (ADC_BATTERY_VALUE() <= 1293) // 3.8V
		{
			app_battery_level = BATTERY_FULL;
		}
		else if (ADC_BATTERY_VALUE() <= 1404) // 3.5V
		{
			app_battery_level = BATTERY_LV2;
		}
		else if (ADC_BATTERY_VALUE() <= 1586) // 3.1V
		{
			app_battery_level = BATTERY_LV1;
		}
		else if (ADC_BATTERY_VALUE() <= 1638) // 3.0V
		{
			app_battery_level = BATTERY_LV0;
		}
		else // 2.75V
		{
			app_battery_level = BATTERY_LOSE;
		}
		return;
	}

	//如果变化较大则进行多次确认
	if (ADC_BATTERY_VALUE() >= __temp1 || ADC_BATTERY_VALUE() <= __temp2)
	{
		__temp1 = ADC_BATTERY_VALUE() + 32;
		if (ADC_BATTERY_VALUE() >= 32)
			__temp2 = ADC_BATTERY_VALUE() - 32;
		else
			__temp2 = 0;

		__cnt = 0;
		return;
	}
	else
	{
		//变化幅度大时重新确认, 变化幅度小时不重新确认
		__temp1 = ADC_BATTERY_VALUE() + 32;
		if (ADC_BATTERY_VALUE() >= 32)
			__temp2 = ADC_BATTERY_VALUE() - 32;
		else
			__temp2 = 0;

		if (__cnt < 9)
		{
			__cnt++;
			return;
		}
	}

	if(app_flag_usb_insert)
	{
		//单向往上走
		if (ADC_BATTERY_VALUE() <= 702) //7V voltage over high
		{
			app_battery_level = BATTERY_HIGH;
		}
		else if (ADC_BATTERY_VALUE() <= 1293) // 3.8V
		{
			app_battery_level = BATTERY_FULL;
		}
		else if (ADC_BATTERY_VALUE() <= 1404 && app_battery_level <= BATTERY_LV2) // 3.5V
		{
			app_battery_level = BATTERY_LV2;
		}
		else if (ADC_BATTERY_VALUE() <= 1586 && app_battery_level <= BATTERY_LV1) // 3.1V
		{
			app_battery_level = BATTERY_LV1;
		}
		else if (ADC_BATTERY_VALUE() <= 1638 && app_battery_level <= BATTERY_LV0) // 3.0V
		{
			app_battery_level = BATTERY_LV0;
		}
	}
	else
	{
		//单向往下走
		if (ADC_BATTERY_VALUE() <= 702) //7V voltage over high
		{
			app_battery_level = BATTERY_HIGH;
		}
		else if (ADC_BATTERY_VALUE() <= 1293 && app_battery_level >= BATTERY_FULL) // 3.8V
		{
			app_battery_level = BATTERY_FULL;
		}
		else if (ADC_BATTERY_VALUE() <= 1404 && app_battery_level >= BATTERY_LV2) // 3.5V
		{
			app_battery_level = BATTERY_LV2;
		}
		else if (ADC_BATTERY_VALUE() <= 1586 && app_battery_level >= BATTERY_LV1) // 3.1V
		{
			app_battery_level = BATTERY_LV1;
		}
		else if (ADC_BATTERY_VALUE() <= 1638 && app_battery_level >= BATTERY_LV0) // 3.0V
		{
			app_battery_level = BATTERY_LV0;
		}
		else // 2.9V
		{
			app_battery_level = BATTERY_LOSE;
		}
	}
}

/*****************************
 * 睡眠处理
***************************/
static void sleep(void)
{
	uint16_t check_count = 0;
	
	if(app_flag_sleep)
	{
		Led_Display_exit();
		MM_adc1_suspend();
		PWM_SUSPEND();
		main_IO_exit();

		while (app_flag_sleep)
		{
			MTF_sys_stop(); //进入休眠模式
			check_count = 0;
			while (check_count < 300)
			{
				MTF_watch_dog_feed();
				app_flag_sleep_updata = 1;
				if (app_timer_flag_10ms)
				{
					app_timer_flag_10ms = 0;
					event_produce();
					event_handle();
					check_count++;
				}
				if (app_flag_sleep == 0)
					break;
			}
		}
		
		main_IO_init();
		PWM_START();
		MM_adc1_start();
		Led_Display_init();
	}
}

/*****************************
 * 应用初始化
 * **************************/
static void app_init(void)
{
	//PADAUK bit type don not support init that global value
	app_flag_sleep = 0; //睡眠
	app_flag_sys_ready = 0; //系统准备完毕
	app_flag_work = 0; //工作
	app_flag_error = 0; //系统错误
	app_flag_sleep_updata = 0; //用于标记在唤醒后一段时间内, 进行一定的唤醒处理
	app_flag_nc1 = 0;
	app_flag_usb_insert = 0; //充电线插入
	app_flag_charge_full = 0; //充满电
	app_flag_ntc_error = 0; //ntc AD错误
	app_flag_current_error = 0; //电流AD错误
	app_flag_battery_error = 0; //电池AD错误
	app_flag_temp_unit_C = 0; //系统温度单位
	app_flag_disp_battery_level = 0; //显示电池电量
	app_flag_nc2 = 0;
	app_flag_nc3 = 0;
	app_flag_nc4 = 0;	
	app_timer_flag_200us = 0;
	app_timer_flag_2ms = 0;
	app_timer_flag_10ms = 0;
	app_timer_flag_100ms = 0;
}

#include "simulate_uart.h"
void main(void)
{	
    uint8_t sys_read_delay = 0, sleep_updata_delay = 0; 
	
	uint16_t tempADC = 0;
	uint8_t ccc = 0, itoc = 0;
	
	main_IO_init();
	PWM_INIT();
	MM_adc1_init();
	MTF_timer_init_handle();
	Led_Display_init();

    MTF_watch_dog_init();

	app_init();

	while (1)
	{
		MTF_watch_dog_feed();

		if (app_timer_flag_2ms)
		{
			app_timer_flag_2ms = 0;
			adc_run();
		}

		if (app_timer_flag_10ms)
		{
			app_timer_flag_10ms = 0;
			// temp_deal();
			battery_deal();
			// motor_current_deal();
			if (app_flag_sleep_updata == 0)
			{
				event_produce();
				event_handle();
				motor_function();
				Led_display();
			}
		}

		if (app_timer_flag_100ms)
		{
			app_timer_flag_100ms = 0;

			if(app_flag_sys_ready==0)
			{
                sys_read_delay++;
				if(sys_read_delay>=10) //上电等待系统稳定
				{
					sys_read_delay = 0;
					app_flag_sys_ready = 1;
					app_flag_disp_battery_level = 1;
					app_flag_sleep = 0;
					app_flag_work = 1;
				}
			}

			if (app_flag_sleep_updata)
			{
				sleep_updata_delay++;
				if (sleep_updata_delay >= 2)
				{
					sleep_updata_delay = 0;
					app_flag_sleep_updata = 0;
				}
			}
		}

#if 0
		if(simulate_uart_send_cnt()==0) //uart打印调试
		{
			if(ccc==0)
			{
				simulate_uart_send_buffer('A');
			}
			else if(ccc==1)
			{
				simulate_uart_send_buffer('D');
			}
			else if(ccc==2)
			{
				simulate_uart_send_buffer(':');
			}
			else if(ccc==3)
			{
				itoc = ((tempADC&0XF000)>>12);
				if(itoc>=10)
					itoc = itoc-10+'A';
				else
					itoc = itoc+'0';
				simulate_uart_send_buffer(itoc);
			}
			else if(ccc==4)
			{
				itoc = ((tempADC&0X0F00)>>8);
				if(itoc>=10)
					itoc = itoc-10+'A';
				else
					itoc = itoc+'0';
				simulate_uart_send_buffer(itoc);
			}
			else if(ccc==5)
			{
				itoc = ((tempADC&0X00F0)>>4);
				if(itoc>=10)
					itoc = itoc-10+'A';
				else
					itoc = itoc+'0';
				simulate_uart_send_buffer(itoc);
			}
			else if(ccc==6)
			{
				itoc = tempADC&0X000F;
				if(itoc>=10)
					itoc = itoc-10+'A';
				else
					itoc = itoc+'0';
				simulate_uart_send_buffer(itoc);
			}
			else if(ccc==7)
			{
				simulate_uart_send_buffer(10);
			}

			ccc++;
			if(ccc>7)
			{
				ccc = 0;
				tempADC = ADC_BATTERY_VALUE();
			}
		}
#endif
		
		sleep();

        //外设和程序测试
#if 0
        $ PB.4 TOGGLE;
        .delay 8000;
        gpio_test();
        
        //电池AD = 1.2*4096/VDD
        if (ADC_BATTERY_VALUE() >= 1328 - 10 && ADC_BATTERY_VALUE() <= 1328 + 10)
            PWM_MOTOR_START();
        else
            PWM_MOTOR_SUSPEND();
#endif
    }
}