#ifndef _PWM_BOARD_H
#define _PWM_BOARD_H

#include "pwm_port.h"

#define PWM_DUTY_0 0
#define PWM_DUTY_30 44
#define PWM_DUTY_40 58
#define PWM_DUTY_50 73
#define PWM_DUTY_75 109

#define PWM_INIT() MM_pwm1_init()
#define PWM_START() MM_pwm1_start()
#define PWM_SUSPEND() MM_pwm1_suspend()
#define PWM_ULTRASOUND_SET_DUTY(x) MM_pwm1_set_duty(x)

#endif
