#include "adc_port.h"

uint8_t MM_adc1_init(void)
{
    //设置模拟IO
    // RA4
    TRISA |= 0B00010000;  //1设置为输入口, 0配置为输出口
    ANSEL0 |= 0B00010000; //1配置为模拟口, 0配置为普通IO或特殊功能
    // RB0
    // TRISB |= 0B00000001;  //1设置为输入口, 0配置为输出口
    // ANSEL1 |= 0B00000001; //1配置为模拟口, 0配置为普通IO或特殊功能

    //设置ADC的运行模式
    ADCON0 = 0B10000000; //设置ADC时钟为Fsys/32
    ADCON1 = 0; //ADC结果为左对齐12bit

    //设置ADC转换通道
    MM_adc1_set_channel(MTF_ADC_CH4); //切换通道后建议500ns后开采样

    //设置参考电压
    MM_adc1_set_ref(MTF_ADC_VREF_2P4V);

    //设置ADC中断
    // ADC_EnableInt();
    // IRQ_SET_PRIORITY(IRQ_ADC,IRQ_PRIORITY_HIGH);
    // IRQ_ALL_ENABLE();

    //开启ADC
    MM_adc1_start();

    return 0;
}

uint8_t MM_adc1_exit(void)
{
    return 0;
}

void MM_adc1_start(void)
{
    //开启ADC模块
    ADCON0 |= 0X01;
}

void MM_adc1_suspend(void)
{
    //停止ADC模块
    ADCON0 &= ~0X01;
}

void MM_adc1_set_ref(uint8_t ref)
{
    //设置参考电压
    if (ref == MTF_ADC_VREF_VDD)
    {
        ADCON1 &= ~0X04;
    }
    else
    {
        //设置ADC LDO
        ADCON1 |= 0X04;        //开启后只能使用MCU LDO参考电压, 关闭后使用VDD参考电压
        if (ref == MTF_ADC_VREF_2V)
            ADCON1 |= 0X01;
        if (ref == MTF_ADC_VREF_2P4V)
            ADCON1 &= ~0X01;
    }
}

void MM_adc1_set_channel(uint8_t channel)
{
    if (channel <= 15)
    {
        ADCON0 &= ~0X3C;
        ADCON0 |= channel << 2; // BIT3~0
        ADCON1 &= ~0X40;        // BIT4
    }
    else if (channel <= 18)
    {
        ADCON0 &= ~0X3C;
        ADCON0 |= (channel & 0X0F) << 2; // BIT3~0
        ADCON1 |= 0X40;                  // BIT4
    }
}
