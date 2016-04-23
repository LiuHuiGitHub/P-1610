#ifndef _NORMAL_H
#define _NORMAL_H

#define uchar 	unsigned char
#define uint	unsigned int
#define ulong	unsigned long

#define TRUE	1
#define FALSE	0	
#define true	1
#define false	0	
#define bool	bit

//定义ISP/IAP命令
#define ISP_IAP_BYTE_READ 		1	   //字节读
#define ISP_IAP_BYTE_PROGRAM 	2	   //字节编程，可以将1写成0
#define ISP_IAP_BYTE_ERASE 		3	   //扇区擦出，可以将0擦成1

// 定义Flash操作等待时间并打开IAP 
#define ENABLE_ISP 0X82		//系统时钟<20MHz
// 定义EEPROM起始地址
#define DATA_FLASH_START_ADDRESS 0X0		//STC11/STC10xx系列单片机EEPROM起始地址

// EEPROM读写相关函数 一系列IAP操作后需要调用IAP_Disable关闭IAP并清除相关寄存器 
void ByteProgram(uint unAddr,uchar ucData);
uchar ByteRead(uint unDataAddr);
void SectorErase(uint unDataAddr);
void IAP_Disable(void);

void Ding(void);
void Delay_10us(uchar _10us);
void Delay_1ms(uchar _1ms);


void Write_E2PROM8_CardID(void);
void Read_E2PROM8_CardID(void);

#endif










