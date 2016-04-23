#ifndef __SYS_TIM_H__
#define __SYS_TIM_H__

#include "typedef.h"

//typedef enum
//{
//	TIM_0 = 0,
//	TIM_1,
//	TIM_2,
//} TIM_ENUM;

//void sys_timInit(TIM_ENUM Tim_x, UINT16 LoadValue, void(*CallBack)(void));
//void sys_timOpen(TIM_ENUM Tim_x);
//void sys_timClose(TIM_ENUM Tim_x);
void sys_tim0Init(void);

#endif
