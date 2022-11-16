#ifndef _WATCH_DOG_PORT_H
#define _WATCH_DOG_PORT_H

#include "types_base.h"

#ifndef DEBUG

#define MTF_watch_dog_init() {asm("clrwdt"); WDTCON = 1;} //start WDOG, overtime about 125ms
#define MTF_watch_dog_exit() WDTCON = 0 //disable WDOG
#define MTF_watch_dog_feed() asm("clrwdt") //feed WDOG

#else

#define MTF_watch_dog_init()
#define MTF_watch_dog_exit()
#define MTF_watch_dog_feed()

#endif

//进入掉电模式, 按官方流程
#define MTF_sys_stop() {PORTA; asm("clrwdt"); asm("nop"); asm("nop"); asm("sleep"); asm("nop"); asm("nop");} //进入掉电模式, 通过IO电平变化或外设中断唤醒, LVR无效

#endif
