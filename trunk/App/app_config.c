#include "app_config.h"
#include "sys_eeprom.h"
#include "sys_uart.h"
#include "hwa_eeprom.h"
#include "hwa_uart.h"
#include "LED.h"
#include "mifare.h"
#include "buzzer.h"
#include "string.h"
#include "intrins.h"

//typedef struct
//{
//    UINT8 MoneySumH;    //营业额整数部分
//    UINT8 MoneySumL;    //营业额小数部分
//    UINT8 Reserve[48];
//} MONEY_STRUCT;

/*
typedef struct
{
    UINT8 MGM_Card[6];
    UINT8 PWD_Card[6];
    UINT8 USER_Card[6];
    UINT8 Sector;       //钱所在扇区
	UINT8 RecoveryOldCard;          //1->兼容旧卡
    UINT16 Money;

}SYSTEM_STRUCT;
*/
UINT32 MoneySum = 0;

SYSTEM_STRUCT s_System;
code SYSTEM_STRUCT s_FactorySystem =
{
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    1, 10, {0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},

    0,
    0,
};

BOOL b_FactorySystem = FALSE;       //恢复出厂设置标志


void app_configInit(void)
{
    BOOL state = FALSE;
    led_Switch(OFF);
    hwa_eepromInit();
    if(app_configRead(SYSTEM_SETTING_SECTOR) == FALSE)
    {
        memcpy(&s_System, &s_FactorySystem, sizeof(s_System));
        b_FactorySystem = TRUE;
        led_ShowNumber(8888,88,0xFF);
    }
    else
    {
        if(s_System.Sector > 16 || s_System.Sector < 1)                 //钱包分区不在1~16
        {
            s_System.Sector = s_FactorySystem.Sector;
            state = TRUE;
        }
        if(s_System.Money > 20000)                                        //金额大于200
        {
            s_System.Money = s_FactorySystem.Money;
            state = TRUE;
        }
        if(state)
        {
			app_configWrite(SYSTEM_SETTING_SECTOR);
            b_FactorySystem = TRUE;
        }
    }
    
    if(app_configRead(MONEY_SECTOR) == FALSE)
    {
        memset(&MoneySum, 0x00, sizeof(MoneySum));
		app_configWrite(MONEY_SECTOR);
    }
    led_Switch(ON);
}

BOOL app_configRead(UINT8 Sector)
{
    BOOL state;
    if(Sector == SYSTEM_SETTING_SECTOR)
    {
        state = hwa_eepromReadSector((UINT8*)&s_System, SYSTEM_SETTING_SECTOR);
    }
    else
    {
        state = hwa_eepromReadSector((UINT8*)&MoneySum, MONEY_SECTOR);
    }
    return state;
}

void app_configWrite(UINT8 Sector)
{
    led_Switch(OFF);
    if(Sector == SYSTEM_SETTING_SECTOR)
    {
        hwa_eepromWriteSector((UINT8*)&s_System, SYSTEM_SETTING_SECTOR);
    }
    else
    {
        hwa_eepromWriteSector((UINT8*)&MoneySum, MONEY_SECTOR);
    }
    led_Switch(ON);
}

void app_confingHandler1s(void)
{
    led_Switch(OFF);
//    hwa_eepromReadSector((UINT8*)&USER_Card_ID, USER_CAED_ID_SECTOR);
//    sys_uartSendData((UINT8*)&USER_Card_ID, 5);
//    USER_Card_ID[0][0]++;
//    hwa_eepromWriteSector((UINT8*)&USER_Card_ID, USER_CAED_ID_SECTOR);
    led_Switch(ON);
}

