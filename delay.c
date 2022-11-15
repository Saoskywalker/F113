#include "delay.h"

#define CPU_FREQUENCY 4000 //CPU: 4M

#define _MS_CNT 40
#define _US_CNT 1

void delay(volatile uint32_t ms)
{
	delay_ms(ms);
}

void delay_us(volatile uint16_t us) //PS: 34us

{
	while(us--) //about 32us
	{
		// volatile uint32_t n = _US_CNT;
		// while(n--);
	}
}

void delay_ms(volatile uint32_t us)
{
	while(us--)
	{
		volatile uint32_t n = _MS_CNT;
		while(n--);
	}
}
