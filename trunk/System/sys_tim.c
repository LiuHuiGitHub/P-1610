#include "STC15F2K60S2.h"
#include "sys_tim.h"

//void(*TimCallBackFun[3])(void) =
//{
//    (void *)0,
//    (void *)0,
//    (void *)0,
//};

//void sys_timInit(TIM_ENUM Tim_x, UINT16 LoadValue, void(*CallBack)(void))
//{
//    if(Tim_x == TIM_0)
//    {
//        AUXR |= 0x80;
//        TMOD = 0x00;
//        TL0 = LoadValue;
//        TH0 = LoadValue >> 8;
//        TR0 = 0;
//        ET0 = 1;
//    }
//    else if(Tim_x == TIM_1)
//    {
//        AUXR |= 0x40;
//        TMOD &= 0x0F;
//        TL1 = LoadValue;
//        TH1 = LoadValue >> 8;
//        TR1 = 0;
//        ET1 = 1;
//    }
//    else if(Tim_x == TIM_2)
//    {
//        AUXR |= 0x04;
//        T2L = LoadValue;
//        T2H = LoadValue >> 8;
//        IE2 |= 0x04;        //open t2 interrupt
//        AUXR &= ~0x10;      //close t2 switch
//    }
//    TimCallBackFun[Tim_x] = CallBack;
//    EA = 1;
//}
//void sys_timOpen(TIM_ENUM Tim_x)
//{
//    if(Tim_x == TIM_0)
//    {
//        TR0 = 1;
//    }
//    else if(Tim_x == TIM_1)
//    {
//        TR1 = 1;
//    }
//    else if(Tim_x == TIM_2)
//    {
//        AUXR |= 0x10;      //close t2 switch
//    }
//}
//void sys_timClose(TIM_ENUM Tim_x)
//{
//    if(Tim_x == TIM_0)
//    {
//        TR0 = 0;
//    }
//    else if(Tim_x == TIM_1)
//    {
//        TR1 = 0;
//    }
//    else if(Tim_x == TIM_2)
//    {
//        AUXR &= ~0x10;      //close t2 switch
//    }
//}

//void sys_tim0Isr() interrupt 1
//{
//    if(TimCallBackFun[0] != (void *)0)
//    {
//        (*TimCallBackFun[0])();
//    }
//}

//void sys_tim1Isr() interrupt 3
//{
//    if(TimCallBackFun[1] != (void *)0)
//    {
//        (*TimCallBackFun[1])();
//    }
//}

//void sys_tim2Isr() interrupt 12
//{
//    if(TimCallBackFun[2] != (void *)0)
//    {
//        (*TimCallBackFun[2])();
//    }
//}

#define FOSC		22118400

#define T1MS (65536-FOSC/12/1000) //12T

void sys_tim0Init(void)
{
	TMOD &= 0xF0;
	TH0 = (UINT8)(T1MS>>8);
	TL0 = (UINT8)T1MS;
	ET0 = 1;
	TR0 = 1;
//	PT1 = 1;
    EA = 1;
}
