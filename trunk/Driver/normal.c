#include <intrins.h>
#include "normal.h"
#include "reg52.h"
#include "fm1702sl.h"

void Delay_10us(uchar _10us)
{
    while(_10us--)
	{
	    _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	    _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();		
//		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
//		_nop_();	  //6.79MHz
    }
}

///////////////////////////////////////////////////////////////////////
// Delay 1ms
///////////////////////////////////////////////////////////////////////
void Delay_1ms(uchar _1ms)
{
    while (_1ms--)
    {
	    Delay_10us(100);
    }
}

///////////////////////////////////////////////////////////////////////
// 蜂鸣器 响一声
///////////////////////////////////////////////////////////////////////
void Ding(void)
{
	SPEAKER=1;
	Delay_1ms(150);
	SPEAKER=0;
	Delay_1ms(80);
}

/* 关闭IAP功能，清除相关特殊寄存器 */
void IAP_Disable(void)
{
	ISP_CONTR=0;
	ISP_CMD=0;
	ISP_TRIG=0;
	ISP_ADDRH = 0X80;
	ISP_ADDRL = 0X00;
}
/* 扇区擦除 */
void SectorErase(uint unDataAddr)
{
	uint unAddrTmp;
	unAddrTmp = unDataAddr + DATA_FLASH_START_ADDRESS;

	ISP_ADDRH=(uchar)(unAddrTmp>>8);
	ISP_ADDRL=0;
	ISP_CMD = ISP_IAP_BYTE_ERASE;
	ISP_CONTR = ENABLE_ISP;
	EA=0;
	ISP_TRIG=0x5A;
	ISP_TRIG=0xA5;
	EA=1;
}

/* 读一字节*/
uchar ByteRead(uint unDataAddr)
{
	uint unAddrTmp;
	unAddrTmp = unDataAddr + DATA_FLASH_START_ADDRESS;
	
	ISP_ADDRH=(uchar)(unAddrTmp>>8);
	ISP_ADDRL=(uchar)(unAddrTmp&0x00ff);
	ISP_CMD = ISP_IAP_BYTE_READ;
	ISP_CONTR = ENABLE_ISP;
	EA=0;
	ISP_TRIG=0X5A;
	ISP_TRIG=0XA5;
	EA=1;
	return ISP_DATA;
}

/* 编程一个字节 */
void ByteProgram(uint unAddr, uchar ucData)
{
	uint unAddrTmp;
	unAddrTmp = unAddr + DATA_FLASH_START_ADDRESS;

	ISP_ADDRH=(uchar)(unAddrTmp>>8);
	ISP_ADDRL=(uchar)(unAddrTmp&0x00ff);
	ISP_CMD = ISP_IAP_BYTE_PROGRAM;
	ISP_CONTR = ENABLE_ISP;
	ISP_DATA = ucData;
	EA=0;
	ISP_TRIG=0x5A;
	ISP_TRIG=0xA5;
	EA=1;
}

void Write_E2PROM8_CardID(void)
{
	SectorErase(0x800);
	ByteProgram(0x800,USER_Card_ID[0][0]);	   //第0路，第0位CardID
	ByteProgram(0x801,USER_Card_ID[0][1]);	   //第0路，第1位CardID
	ByteProgram(0x802,USER_Card_ID[0][2]);	   //第0路，第2位CardID
	ByteProgram(0x803,USER_Card_ID[0][3]);	   //第0路，第3位CardID
	ByteProgram(0x804,USER_Card_ID[0][4]);	   //第0路，第4位CardID

	ByteProgram(0x805,USER_Card_ID[1][0]);	   //第1路，第0位CardID
	ByteProgram(0x806,USER_Card_ID[1][1]);	   //第1路，第1位CardID
	ByteProgram(0x807,USER_Card_ID[1][2]);	   //第1路，第2位CardID
	ByteProgram(0x808,USER_Card_ID[1][3]);	   //第1路，第3位CardID
	ByteProgram(0x809,USER_Card_ID[1][4]);	   //第1路，第4位CardID

	ByteProgram(0x80a,USER_Card_ID[2][0]);	   //第2路，第0位CardID
	ByteProgram(0x80b,USER_Card_ID[2][1]);	   //第2路，第1位CardID
	ByteProgram(0x80c,USER_Card_ID[2][2]);	   //第2路，第2位CardID
	ByteProgram(0x80d,USER_Card_ID[2][3]);	   //第2路，第3位CardID
	ByteProgram(0x80e,USER_Card_ID[2][4]);	   //第2路，第4位CardID

	ByteProgram(0x80f,USER_Card_ID[3][0]);	   //第3路，第0位CardID
	ByteProgram(0x810,USER_Card_ID[3][1]);	   //第3路，第1位CardID
	ByteProgram(0x811,USER_Card_ID[3][2]);	   //第3路，第2位CardID
	ByteProgram(0x812,USER_Card_ID[3][3]);	   //第3路，第3位CardID
	ByteProgram(0x813,USER_Card_ID[3][4]);	   //第3路，第4位CardID

	ByteProgram(0x814,USER_Card_ID[4][0]);	   //第4路，第0位CardID
	ByteProgram(0x815,USER_Card_ID[4][1]);	   //第4路，第1位CardID
	ByteProgram(0x816,USER_Card_ID[4][2]);	   //第4路，第2位CardID
	ByteProgram(0x817,USER_Card_ID[4][3]);	   //第4路，第3位CardID
	ByteProgram(0x818,USER_Card_ID[4][4]);	   //第4路，第4位CardID

	ByteProgram(0x819,USER_Card_ID[5][0]);	   //第5路，第0位CardID
	ByteProgram(0x81a,USER_Card_ID[5][1]);	   //第5路，第1位CardID
	ByteProgram(0x81b,USER_Card_ID[5][2]);	   //第5路，第2位CardID
	ByteProgram(0x81c,USER_Card_ID[5][3]);	   //第5路，第3位CardID
	ByteProgram(0x81d,USER_Card_ID[5][4]);	   //第5路，第4位CardID

	ByteProgram(0x81e,USER_Card_ID[6][0]);	   //第6路，第0位CardID
	ByteProgram(0x81f,USER_Card_ID[6][1]);	   //第6路，第1位CardID
	ByteProgram(0x820,USER_Card_ID[6][2]);	   //第6路，第2位CardID
	ByteProgram(0x821,USER_Card_ID[6][3]);	   //第6路，第3位CardID
	ByteProgram(0x822,USER_Card_ID[6][4]);	   //第6路，第4位CardID

	ByteProgram(0x823,USER_Card_ID[7][0]);	   //第7路，第0位CardID
	ByteProgram(0x824,USER_Card_ID[7][1]);	   //第7路，第1位CardID
	ByteProgram(0x825,USER_Card_ID[7][2]);	   //第7路，第2位CardID
	ByteProgram(0x826,USER_Card_ID[7][3]);	   //第7路，第3位CardID
	ByteProgram(0x827,USER_Card_ID[7][4]);	   //第7路，第4位CardID

	ByteProgram(0x828,USER_Card_ID[8][0]);	   //第8路，第0位CardID
	ByteProgram(0x829,USER_Card_ID[8][1]);	   //第8路，第1位CardID
	ByteProgram(0x82a,USER_Card_ID[8][2]);	   //第8路，第2位CardID
	ByteProgram(0x82b,USER_Card_ID[8][3]);	   //第8路，第3位CardID
	ByteProgram(0x82c,USER_Card_ID[8][4]);	   //第8路，第4位CardID

	ByteProgram(0x82d,USER_Card_ID[9][0]);	   //第9路，第0位CardID
	ByteProgram(0x82e,USER_Card_ID[9][1]);	   //第9路，第1位CardID
	ByteProgram(0x82f,USER_Card_ID[9][2]);	   //第9路，第2位CardID
	ByteProgram(0x830,USER_Card_ID[9][3]);	   //第9路，第3位CardID
	ByteProgram(0x831,USER_Card_ID[9][4]);	   //第9路，第4位CardID
	IAP_Disable();
}

void Read_E2PROM8_CardID(void)
{
    if(ByteRead(0x800) == 0xff) USER_Card_ID[0][0]=0; else USER_Card_ID[0][0]=ByteRead(0x800);    //第0路，第0位CardID
	if(ByteRead(0x801) == 0xff) USER_Card_ID[0][1]=0; else USER_Card_ID[0][1]=ByteRead(0x801);    //第0路，第1位CardID
	if(ByteRead(0x802) == 0xff) USER_Card_ID[0][2]=0; else USER_Card_ID[0][2]=ByteRead(0x802);    //第0路，第2位CardID
	if(ByteRead(0x803) == 0xff) USER_Card_ID[0][3]=0; else USER_Card_ID[0][3]=ByteRead(0x803);    //第0路，第3位CardID
	if(ByteRead(0x804) == 0xff) USER_Card_ID[0][4]=0; else USER_Card_ID[0][4]=ByteRead(0x804);    //第0路，第4位CardID

	if(ByteRead(0x805) == 0xff) USER_Card_ID[1][0]=0; else USER_Card_ID[1][0]=ByteRead(0x805);    //第1路，第0位CardID
	if(ByteRead(0x806) == 0xff) USER_Card_ID[1][1]=0; else USER_Card_ID[1][1]=ByteRead(0x806);    //第1路，第1位CardID
	if(ByteRead(0x807) == 0xff) USER_Card_ID[1][2]=0; else USER_Card_ID[1][2]=ByteRead(0x807);    //第1路，第2位CardID
	if(ByteRead(0x808) == 0xff) USER_Card_ID[1][3]=0; else USER_Card_ID[1][3]=ByteRead(0x808);    //第1路，第3位CardID
	if(ByteRead(0x809) == 0xff) USER_Card_ID[1][4]=0; else USER_Card_ID[1][4]=ByteRead(0x809);    //第1路，第4位CardID

	if(ByteRead(0x80a) == 0xff) USER_Card_ID[2][0]=0; else USER_Card_ID[2][0]=ByteRead(0x80a);    //第2路，第0位CardID
	if(ByteRead(0x80b) == 0xff) USER_Card_ID[2][1]=0; else USER_Card_ID[2][1]=ByteRead(0x80b);    //第2路，第1位CardID
	if(ByteRead(0x80c) == 0xff) USER_Card_ID[2][2]=0; else USER_Card_ID[2][2]=ByteRead(0x80c);    //第2路，第2位CardID
	if(ByteRead(0x80d) == 0xff) USER_Card_ID[2][3]=0; else USER_Card_ID[2][3]=ByteRead(0x80d);    //第2路，第3位CardID
	if(ByteRead(0x80e) == 0xff) USER_Card_ID[2][4]=0; else USER_Card_ID[2][4]=ByteRead(0x80e);    //第2路，第4位CardID

	if(ByteRead(0x80f) == 0xff) USER_Card_ID[3][0]=0; else USER_Card_ID[3][0]=ByteRead(0x80f);    //第3路，第0位CardID
	if(ByteRead(0x810) == 0xff) USER_Card_ID[3][1]=0; else USER_Card_ID[3][1]=ByteRead(0x810);    //第3路，第1位CardID
	if(ByteRead(0x811) == 0xff) USER_Card_ID[3][2]=0; else USER_Card_ID[3][2]=ByteRead(0x811);    //第3路，第2位CardID
	if(ByteRead(0x812) == 0xff) USER_Card_ID[3][3]=0; else USER_Card_ID[3][3]=ByteRead(0x812);    //第3路，第3位CardID
	if(ByteRead(0x813) == 0xff) USER_Card_ID[3][4]=0; else USER_Card_ID[3][4]=ByteRead(0x813);    //第3路，第4位CardID

	if(ByteRead(0x814) == 0xff) USER_Card_ID[4][0]=0; else USER_Card_ID[4][0]=ByteRead(0x814);    //第4路，第0位CardID
	if(ByteRead(0x815) == 0xff) USER_Card_ID[4][1]=0; else USER_Card_ID[4][1]=ByteRead(0x815);    //第4路，第1位CardID
	if(ByteRead(0x816) == 0xff) USER_Card_ID[4][2]=0; else USER_Card_ID[4][2]=ByteRead(0x816);    //第4路，第2位CardID
	if(ByteRead(0x817) == 0xff) USER_Card_ID[4][3]=0; else USER_Card_ID[4][3]=ByteRead(0x817);    //第4路，第3位CardID
	if(ByteRead(0x818) == 0xff) USER_Card_ID[4][4]=0; else USER_Card_ID[4][4]=ByteRead(0x818);    //第4路，第4位CardID

	if(ByteRead(0x819) == 0xff) USER_Card_ID[5][0]=0; else USER_Card_ID[5][0]=ByteRead(0x819);    //第5路，第0位CardID
	if(ByteRead(0x81a) == 0xff) USER_Card_ID[5][1]=0; else USER_Card_ID[5][1]=ByteRead(0x81a);    //第5路，第1位CardID
	if(ByteRead(0x81b) == 0xff) USER_Card_ID[5][2]=0; else USER_Card_ID[5][2]=ByteRead(0x81b);    //第5路，第2位CardID
	if(ByteRead(0x81c) == 0xff) USER_Card_ID[5][3]=0; else USER_Card_ID[5][3]=ByteRead(0x81c);    //第5路，第3位CardID
	if(ByteRead(0x81d) == 0xff) USER_Card_ID[5][4]=0; else USER_Card_ID[5][4]=ByteRead(0x81d);    //第5路，第4位CardID

	if(ByteRead(0x81e) == 0xff) USER_Card_ID[6][0]=0; else USER_Card_ID[6][0]=ByteRead(0x81e);    //第6路，第0位CardID
	if(ByteRead(0x81f) == 0xff) USER_Card_ID[6][1]=0; else USER_Card_ID[6][1]=ByteRead(0x81f);    //第6路，第1位CardID
	if(ByteRead(0x820) == 0xff) USER_Card_ID[6][2]=0; else USER_Card_ID[6][2]=ByteRead(0x820);    //第6路，第2位CardID
	if(ByteRead(0x821) == 0xff) USER_Card_ID[6][3]=0; else USER_Card_ID[6][3]=ByteRead(0x821);    //第6路，第3位CardID
	if(ByteRead(0x822) == 0xff) USER_Card_ID[6][4]=0; else USER_Card_ID[6][4]=ByteRead(0x822);    //第6路，第4位CardID

	if(ByteRead(0x823) == 0xff) USER_Card_ID[7][0]=0; else USER_Card_ID[7][0]=ByteRead(0x823);    //第7路，第0位CardID
	if(ByteRead(0x824) == 0xff) USER_Card_ID[7][1]=0; else USER_Card_ID[7][1]=ByteRead(0x824);    //第7路，第1位CardID
	if(ByteRead(0x825) == 0xff) USER_Card_ID[7][2]=0; else USER_Card_ID[7][2]=ByteRead(0x825);    //第7路，第2位CardID
	if(ByteRead(0x826) == 0xff) USER_Card_ID[7][3]=0; else USER_Card_ID[7][3]=ByteRead(0x826);    //第7路，第3位CardID
	if(ByteRead(0x827) == 0xff) USER_Card_ID[7][4]=0; else USER_Card_ID[7][4]=ByteRead(0x827);    //第7路，第4位CardID

	if(ByteRead(0x828) == 0xff) USER_Card_ID[8][0]=0; else USER_Card_ID[8][0]=ByteRead(0x828);    //第8路，第0位CardID
	if(ByteRead(0x829) == 0xff) USER_Card_ID[8][1]=0; else USER_Card_ID[8][1]=ByteRead(0x829);    //第8路，第1位CardID
	if(ByteRead(0x82a) == 0xff) USER_Card_ID[8][2]=0; else USER_Card_ID[8][2]=ByteRead(0x82a);    //第8路，第2位CardID
	if(ByteRead(0x82b) == 0xff) USER_Card_ID[8][3]=0; else USER_Card_ID[8][3]=ByteRead(0x82b);    //第8路，第3位CardID
	if(ByteRead(0x82c) == 0xff) USER_Card_ID[8][4]=0; else USER_Card_ID[8][4]=ByteRead(0x82c);    //第8路，第4位CardID

	if(ByteRead(0x82d) == 0xff) USER_Card_ID[9][0]=0; else USER_Card_ID[9][0]=ByteRead(0x82d);    //第9路，第0位CardID
	if(ByteRead(0x82e) == 0xff) USER_Card_ID[9][1]=0; else USER_Card_ID[9][1]=ByteRead(0x82e);    //第9路，第1位CardID
	if(ByteRead(0x82f) == 0xff) USER_Card_ID[9][2]=0; else USER_Card_ID[9][2]=ByteRead(0x82f);    //第9路，第2位CardID
	if(ByteRead(0x830) == 0xff) USER_Card_ID[9][3]=0; else USER_Card_ID[9][3]=ByteRead(0x830);    //第9路，第3位CardID
	if(ByteRead(0x831) == 0xff) USER_Card_ID[9][4]=0; else USER_Card_ID[9][4]=ByteRead(0x831);    //第9路，第4位CardID
}























