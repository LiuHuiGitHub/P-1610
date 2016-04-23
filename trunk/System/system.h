#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "STC15F2K60S2.H"

#define DISABLE_INTERRUPT   do{EA=0;}while(0)
#define ENABLE_INTERRUPT    do{EA=1;}while(0)

#define RESET_WATCH_DOG     do{WDT_CONTR=0x33;}while(0)
#define RESET_MCU           do{WDT_CONTR=0x20;}while(0)

#endif
