#include <STC15F2K60S2.H>
#include "buzzer.h"

sbit Buzzer = P3^5;

UINT8 SoundNumberCount = 0;

void buzzer_Init(void)
{
    P3M1 &= ~0x20;      //ÍÆÍìÊä³ö
    P3M0 |= 0x20;
    P3 &= ~0x20;
}

void buzzer_Handler100ms(void)
{
    if(Buzzer)
	{
		Buzzer = 0;
	}
	else if(SoundNumberCount)
	{
		Buzzer = 1;
		SoundNumberCount--;
	}
}

void buzzer_SoundNumber(UINT8 num)
{
    SoundNumberCount = num;
}