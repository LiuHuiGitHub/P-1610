#ifndef __SYS_UART_H__
#define __SYS_UART_H__

#include "typedef.h"

void sys_uartInit(void);
void sys_uartSendString(UINT8 *s);
void sys_uartSendData(UINT8 *pData, UINT8 len);

#endif
