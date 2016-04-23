#include "STC15F2K60S2.h"
#include "app_pulse.h"

sbit pulse = P3^7;

#define PULSE_Interval      300      //1ms*300=300ms

UINT8 u8_pulse = 0;
UINT16 u16_count = 0;
UINT8 u8_width = 0;

void app_pulseInit(void)
{
    P3M1 &= ~0x80;      //ÍÆÍìÊä³ö
    P3M0 |= 0x80;
    pulse = 1;
}

void app_pulseHandler1ms(void)
{
    if(u8_pulse)
    {
        u16_count++;
        if(u16_count<=u8_width)
        {
            pulse = 0;
        }
        else if(u16_count<=PULSE_Interval)
        {
            pulse = 1;
        }
        else
        {
            u16_count = 0;
            u8_pulse--;
        }
    }
}

void app_pulseSendPulse(UINT8 width, UINT8 n)
{
    u8_width = width;
    u8_pulse = n;
}

