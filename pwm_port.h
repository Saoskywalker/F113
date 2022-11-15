#ifndef _PWM_PORT_H
#define _PWM_PORT_H

#include "types_base.h"

uint8_t MM_pwm1_init(void);
uint8_t MM_pwm1_exit(void);
void MM_pwm1_start(void);
void MM_pwm1_suspend(void);
void MM_pwm1_set(uint16_t Period, uint16_t Duty);
void MM_pwm1_set_duty(uint16_t Duty);

#endif
