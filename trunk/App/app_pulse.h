#ifndef __APP_PULSE_H__
#define __APP_PULSE_H__

#include "typedef.h"

void app_pulseInit(void);
void app_pulseHandler1ms(void);
void app_pulseSendPulse(UINT8 width, UINT8 n);

#endif
