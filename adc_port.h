#ifndef _ADC_PORT_H
#define _ADC_PORT_H

#include "types_base.h"

//ADC vref define
#define MTF_ADC_VREF_VDD 4  /*Vref = Vdd*/
#define MTF_ADC_VREF_2V 1   /*Vref =2.0v*/
#define MTF_ADC_VREF_2P4V 0 /*Vref =2.4v*/

//ADN channel define
#define MTF_ADC_CH4 4
#define MTF_ADC_CH15 15

uint8_t MM_adc1_init(void);
uint8_t MM_adc1_exit(void);
void MM_adc1_start(void);
void MM_adc1_suspend(void);
void MM_adc1_set_ref(uint8_t ref);
void MM_adc1_set_channel(uint8_t channel);
#define MM_adc1_convert_start() GODONE = 1
#define MM_adc1_get_state() GODONE
#define MM_adc1_get_result() ((uint16_t)ADRESH << 4) + (ADRESL >> 4);

#endif
