#ifndef __HWA_UART_H__
#define __HWA_UART_H__

#include "typedef.h"

#define PAUSE_BRUSH_DLY_CNT		250

extern UINT8 u8_PauseBrushDlyCnt;
extern BOOL b_Brush;
extern UINT8 u8_BrushCount;
extern BOOL b_Refund;
extern BOOL b_RefundSuccess;
extern UINT8 u8_RefundChannel;

void hwa_uartInit(void);
void hwa_uartHandler1ms(void);
void hwa_uartHandler10ms(void);

#endif

