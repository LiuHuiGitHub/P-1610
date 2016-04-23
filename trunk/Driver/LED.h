#ifndef _LED_H
#define _LED_H

#include "typedef.h"

extern UINT8 gShowDot;  //小数点位置 1~6
extern UINT8 gLedBuf[6]; // 从左到右为1~6

void len_Init(void);
void led_Handler1ms(void);
void led_ShowNumber(UINT16 L_num, UINT8 R_num, UINT8 DotPlace);
void led_ShowNumber1(UINT16 L_num, UINT16 R_num, UINT8 DotPlace);
void Led_ShowZero(void);
void led_Switch(bit sw);

#endif
