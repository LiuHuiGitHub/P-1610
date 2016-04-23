#include "STC15F2K60S2.h"
#include "sys_tim.h"
#include "sys_delay.h"
#include "sys_tim.h"
#include "sys_uart.h"
#include "hwa_eeprom.h"
#include "hwa_uart.h"
#include "LED.h"
#include "buzzer.h"
#include "app_brush.h"
#include "app_config.h"
#include "app_pulse.h"

BOOL taskCycle1sFlag = FALSE;
UINT8 taskHandlerCnt10ms = 0;
UINT8 taskHandlerCnt100ms = 0;
UINT8 taskHandlerCnt1s = 0;

void sys_taskHandler10ms(void);
void sys_taskHandler100ms(void);
void sys_taskHandler1s(void);

void sys_gpioInit(void)
{
}

void sys_taskInit(void)
{
    WDT_CONTR = 0x33;
    len_Init();
    sys_gpioInit();
    buzzer_Init();
    sys_uartInit();
    sys_tim0Init();
    app_configInit();
	app_Show();
    app_brushInit();
}

void sys_tim0Isr(void) interrupt 1      //1ms interrupt task
{
    led_Handler1ms();
	taskHandlerCnt10ms++;
    if(taskHandlerCnt10ms>=10)
    {
        taskHandlerCnt10ms = 0;
        sys_taskHandler10ms();
        taskHandlerCnt100ms++;
    }
    if(taskHandlerCnt100ms>=10)
    {
        taskHandlerCnt100ms = 0;
        sys_taskHandler100ms();
        taskHandlerCnt1s++;
    }
    if(taskHandlerCnt1s>=10)
    {
        taskHandlerCnt1s = 0;
        sys_taskHandler1s();
        taskCycle1sFlag = TRUE;
    }
}

void sys_taskHandler10ms(void)            //10ms interrupt task
{
}

void sys_taskHandler100ms(void)           //100ms interrupt task
{
	buzzer_Handler100ms();
}

void sys_taskHandler1s(void)              //1s interrupt task
{
}

void main(void)
{
    sys_taskInit();
    buzzer_SoundNumber(1);
    while(1)
    {
        WDT_CONTR = 0x33;          //reset watch dog      SyttemClk is 11.0592M Over Time 568.8ms
        if(taskCycle1sFlag == TRUE)
        {
        	taskCycle1sFlag = FALSE;
            app_confingHandler1s();
            app_brushCycle1s();
        }
    }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
//    UINT8 s[30];
//    sprintf(s, "%s,%ld\n", file, line);
//    sys_uartSendString(s);
	while(1);
}
#endif
