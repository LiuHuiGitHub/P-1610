
//=============================2014-08-21========================================

//U3.1版本为最新窄刷卡板，可以直接替代投币器安装，无刷卡金额纪录，程序已经做了防快刷错误，数据可以自动恢复。

#include "reg52.h"
#include "normal.h"
#include "mifare.h"
#include "LED.h"
#include "fm1702sl.h"
#include "string.h"
#include "UART.h"

//bit AutoBrush;
bit Brushed;	 //用来判断当前是否进行价格中“分”的设置
uchar gPrice[2];   //gPrice[1]单位为元，gPrice[0]单位为分（0~99）
//uchar gPlusWidth;
unsigned int Timer_Buff;  //暂存主机返回的通道时间
unsigned int SK_Timer;	  //刷卡的单位时间，用于退款
unsigned char Channel = 0x0f;
unsigned char USER_Card_ID[10][5];
unsigned char RevBuffer[40];  

/*
*	id: 	密码在RC500 中的索引
*	ucType:	函数执行类型：
*			0   将gBuff[0]~gBuff[5]内的密码存入
*			1   将gBuff[0]~gBuff[5]和gBuff[10]~gBuff[15]两组密码存入
*/
uchar ChangePWD(uchar id,uchar ucType)
{
	uchar mm0[6];
	uchar mm1[6];
    //1CB533FBAFC6
	mm0[0]=gBuff[0];mm0[1]=gBuff[1];mm0[2]=gBuff[2];mm0[3]=gBuff[3];mm0[4]=gBuff[4];mm0[5]=gBuff[5];
	mm1[0]=gBuff[10];mm1[1]=gBuff[11];mm1[2]=gBuff[12];mm1[3]=gBuff[13];mm1[4]=gBuff[14];mm1[5]=gBuff[15];
	if(ucType==0)
		return Load_Key(id,mm0);
	else if(Load_Key(id,mm0)||Load_Key(id+1,mm1))
		  return 1;
	return 0;
}


	//等待刷卡	
void WaitingBrush(void)
{
	unsigned char i,j;
	do
	{
		WDT_CONTR = 0x3E;
		MIF_Halt();
		Request(RF_CMD_REQUEST_STD);
		AntiColl();
		if(SelectCard()==FM1702_OK)
			return;
		else if(Brushed ==1)
		{
			Brushed = 0;
			Show(0,0);
		}
		else
		{
			if(RxD_Buffer == 0x55)	 //0x55表示指令代码
			{
				RxD_Buffer = 0;
				SendData(0x00);		 //在没有刷卡的状态下，发送给主机0x00；
			}
			else if((RxD_Buffer>>4) == 6) //RxD_Buffer高4位为6，底4位是通道号,记忆卡的ID号
			{
				if((RxD_Buffer!=0x6f)&&(RxD_Buffer!=0x6e)) Channel = RxD_Buffer&0x0f;
				else
				{
					if(RxD_Buffer==0x6f)
					{
						for(i=0;i<5;i++){USER_Card_ID[Channel][i]=gCard_UID[i];}//Last_gCard_UID[i]=0;
						Write_E2PROM8_CardID();	 //每开通某个通道，就写E2PROM一次，记录下该卡号的id
						Delay_1ms(200);
						Delay_1ms(200);
						Delay_1ms(200);
						Delay_1ms(200);
					}
					Channel = 0x0f;
				}
			}
			else if(RxD_Buffer == 0x77)		 //0x77功能代码表示，刷卡单位时间的发送
			{
				Delay_1ms(100);
				SendData(0x33);	//无效指令
				Delay_1ms(100);
				SK_Timer = RxD_Buffer*100;
				RxD_Buffer = 0;
				SendData(0x33);	//无效指令
				Delay_1ms(100);
				SK_Timer += RxD_Buffer;
				RxD_Buffer = 0;
				SendData(0x33);	//无效指令
				Delay_1ms(100);

				//Show(SK_Timer,0);
				SectorErase(0x600);
				ByteProgram(0x600,(SK_Timer/100));
				ByteProgram(0x601,(SK_Timer%100));
				IAP_Disable();
			}
			else if((RxD_Buffer>>4) == 8)
			{
				Delay_1ms(100);
				SendData(0x33);	//无效指令
				for(i=0;i<5;i++){USER_Card_ID[RxD_Buffer&0x0f][i]=0;} 
				Write_E2PROM8_CardID();	 
				Delay_1ms(200);
				Delay_1ms(200);
				Delay_1ms(200);
				Delay_1ms(200);
			}
			else if(RxD_Buffer == 0x99)
			{
				for(j=0;j<10;j++){for(i=0;i<5;i++){USER_Card_ID[j][i]=0;}}
				Write_E2PROM8_CardID();	 
				Delay_1ms(200);
				Delay_1ms(200);
				Delay_1ms(200);
				Delay_1ms(200);
			}
		}
	}while(1);
}

/*
*	寻卡并使用A、B码建立通讯
*	ucField:	要进行读写的数据区
*	ucBlock:	要进行读写的数据块
*	ucKey:		需要验证的密码 ，按位判断  
*	成功：返回卡的类型：0 管理卡   1 用户卡    2 密码卡   3 发行卡
*	失败：返回0XFF
*/
uchar InitCard(uchar ucField,uchar ucBlock,uchar ucKey)
{
	uchar i;
	
	/*使用A密码进行寻卡认证*/
	for(i=0;i<4;i++)
	{
		if((ucKey>>i) & 0x01)
		{
			WaitingBrush();
			Load_Key_EE(i);
			if(Authentication(gCard_UID,ucField,0x60) ==FM1702_OK) //认证通过，进行读测试
			{
				if(Read_Block(gBuff,ucField*4+ucBlock)==FM1702_OK)//读测试通过，则返回，不再进行B密码认证
					return i;
				break;
			}
		}
	}
	/*使用B密码进行寻卡认证*/
	for(i=0;i<4;i++)
	{
		if((ucKey>>i) & 0x01)
		{
			WaitingBrush();
			Load_Key_EE(i);
			if(Authentication(gCard_UID,ucField,0x61) ==FM1702_OK) //认证通过，进行读测试
			{
				if(Read_Block(gBuff,ucField*4+ucBlock)==FM1702_OK)//读测试通过，则返回，不再进行B密码认证
					return i;
				break;
			}
		}
	}
	return 0xFF;
}

/*****************************************************判断用户卡返款函数*******************************************************/
unsigned char USER_Card_Check(void)
{
	unsigned char i;
	for(i = 0;i < 10;i++)
	{
		if((USER_Card_ID[i][0]==gCard_UID[0])&&(USER_Card_ID[i][1]==gCard_UID[1])&&(USER_Card_ID[i][2]==gCard_UID[2])&&(USER_Card_ID[i][3]==gCard_UID[3])&&
		   (USER_Card_ID[i][4]==gCard_UID[4])) 
		{
			return i;
		}
	}
	return 0x55;
}


void Init(void)
{
	P1M1 = 0;
	P1M0 = 0xFF;
	P3M1 = 0;
	P3M0 = 0x20;
	UART_INIT();          //串口初始化
	Timer0_initialize();
	//gPlusWidth = 0;
	while(Init_FM1702()==false);
	SPEAKER=0;           //关闭蜂鸣器
//	Ding();
}
//=======================================================
//	名称: main
//	说明: 主函数，
//FLASH：0扇区首地址为0：0、1数据存储ucPrice（元、分）,表示单次消费金额
//		1扇区首地址为0x200 ：0数据存储ucWorkState，表示工作状态，为0xFF时执行密码更新程序
//		2扇区首地址为0x400:0,1,2,3数据存储历史消费记录，ucTotal[4]
//		  消费记录 = ucTotal[2]*10000+ucTotal[1]*100+ucTotal[0]+ucTotal[4]/100 (元)
//=======================================================
void main(void)
{
	bit Back_Menoy_Flag = FALSE;
	uchar i;
	uchar Channel_Value;
	uchar ucTmp,ucPrice[2];
	uchar ucWorkState;
    unsigned int Total_Money[2]={0}; //Total_Money[0]:单位元(存储数据范围9999~0)；Total_Money[1]:单位分(存储数据范围9999~0)；
	uint unMoney;
	Init();		  //端口初始化
	
	/***********************************E2PROM读取**********************************/
	ucPrice[0] = ByteRead(0); if(ucPrice[0] == 0xff) ucPrice[0] = 0;
	ucPrice[1] = ByteRead(1); if(ucPrice[1] == 0xff) ucPrice[1] = 0;
	ucWorkState= ByteRead(0x200);
	Total_Money[0] = ByteRead(0x400)*100+ByteRead(0x401);if(ByteRead(0x400) == 0xff)	Total_Money[0] = 0;
	Total_Money[1] = ByteRead(0x402)*100+ByteRead(0x403);if(ByteRead(0x402) == 0xff)	Total_Money[1] = 0;
	SK_Timer = ByteRead(0x600)*100+ByteRead(0x601);
	if(ByteRead(0x600) == 0xff)	SK_Timer = 200;
	Read_E2PROM8_CardID();
	IAP_Disable();
	/*******************************************************************************/
	switch(ucWorkState)
	{
	case 0xFF://200905052154(出厂初始化密码卡密码)
		Show(ucWorkState,0);
		gBuff[0]=0x20;gBuff[1]=0x09;gBuff[2]=0x05;gBuff[3]=0x05;gBuff[4]=0x21;gBuff[5]=0x54;
		if(ChangePWD(2,0))
		{
			Ding();
			Ding();
		}
		while(1)//等待刷初始密码卡写入默认管理卡、默认用户卡和用户密码卡通讯密码
		{
			if(InitCard(1,0,0x04)==2)
			{
				if(Read_Block(gBuff,0x4)!=FM1702_OK || ChangePWD(0,1))
				{
					Ding();
					Ding();
				}
				else if(Read_Block(gBuff,0x5)!=FM1702_OK)
				{
					Ding();
					Ding();
				}
				else
				{
					ucWorkState = 1;	 //默认钱包使用分区1
					if(ChangePWD(2,0))
					{
						Ding();
						Ding();
					}
					else
					{
						Ding();
						SectorErase(0x200);
						ByteProgram(0x200,ucWorkState);
						IAP_Disable();
						Delay_1ms(200);
						Delay_1ms(200);
						break;
					}
				}
			}
		}
		break;
	default:
		break;	
	}
	//显示版本号
	gShowDot=0;
	gLedBuf[0]=12;
	gLedBuf[1]=12;
	gLedBuf[2]=12;
	gLedBuf[3]=10;
	gLedBuf[4]=13;
	gLedBuf[5]=2;
	gShowDot=0;
	Delay_1ms(200);
	Delay_1ms(200);

//	Delay_1ms(200);
//	Delay_1ms(200);
//	Delay_1ms(200);
//	Delay_1ms(200);

	//本台机子总消费金额
	Show(Total_Money[0],Total_Money[1]/100);
	Delay_1ms(200);
	Delay_1ms(200);
//	Delay_1ms(200);
//	Delay_1ms(200);
//	Delay_1ms(200);
//	Delay_1ms(200);
	Ding();
	Show(0,0);
	gShowDot=1;
	Brushed = 0;
	while (1)
	{
		switch (InitCard(1,0,0x04))
		{
		case 2:	  //用户密码卡
			Brushed = 0;
			if(Read_Block(gBuff,0x4)!=FM1702_OK)
			{
				Ding();
				Ding();
			}
			else
			{ 
				if(gBuff[6] == 0x10) //说明该密码卡是用来设置钱包分区的，分区信息在gBuff[7]存储
				{
					ucWorkState = gBuff[7];
					SectorErase(0x200);
					ByteProgram(0x200,ucWorkState);
					IAP_Disable();
					Delay_1ms(2);
					Show(ucWorkState,0);
					Ding();
					Delay_1ms(200);
					Delay_1ms(200);
					Delay_1ms(200);
					Delay_1ms(200);
					Show(0,0);
				}
				else if(ChangePWD(0,1))
				{
					Ding();
					Ding();
				}
				else
					Ding();
			}
			break;
		case 0xff: //不是用户密码卡，则判断是否管理卡、用户卡
			switch (InitCard(ucWorkState,0,0x03))
			{
			case 0:	  //管理卡
				ucTmp=Read_Block(gBuff,ucWorkState*4);
				if(gBuff[0] == 0xF0)  //设定脉冲宽度
				{
					Brushed = 0;
					gShowDot=0;
					Show(Total_Money[0],Total_Money[1]/100);
				}
				else
				{
					Brushed = 0;
					
					if(gBuff[0] == 0xfa)	 //单位：角
					{
						ucPrice[1]+=10;

						if(ucPrice[1] >= 100)
						{
							ucPrice[1] = 0;
						}
					}
					if(gBuff[0] == 0x01)	 //单位：元
					{
						ucPrice[0]++;
						if(ucPrice[0] >= 6)  //大于10元的最高单价，单价清零
						{
						    ucPrice[0] = 0;
						}
					}
					SectorErase(0);
					ByteProgram(0,ucPrice[0]);
					ByteProgram(1,ucPrice[1]);
					//ByteProgram(2,gPlusWidth);
					IAP_Disable();
					Show(ucPrice[0],ucPrice[1]);
				}
				Ding();
				Delay_1ms(250);
				Delay_1ms(250);
				if(	Brushed == 0)
				{	
					Delay_1ms(250);
					Delay_1ms(250);
					Delay_1ms(250);
					Delay_1ms(250);
					Delay_1ms(250);
					Delay_1ms(250);
					Show(0,0);
					gShowDot=1;
				}
				break;
			case 1:	  //用户卡
				Brushed = 0;
				ucTmp=Read_Block(gBuff,ucWorkState*4+1);
				if( gBuff[1] == 0xFF) gBuff[1] = 0;
				if (ucTmp!=FM1702_OK || ucPrice[0]==0xff || gBuff[0]*100+gBuff[1]<ucPrice[0]*100+ucPrice[1])
				{
					Ding();
					Ding();
					break;
				}
                if(gBuff[0]>200)
                {
				    ucTmp=Read_Block(gBuff,ucWorkState*4+2);
					if( gBuff[1] == 0xFF) gBuff[1] = 0;
				    if (ucTmp!=FM1702_OK || ucPrice[0]==0xff || gBuff[0]*100+gBuff[1]<ucPrice[0]*100+ucPrice[1]||gBuff[0]>200)
				    {
					    Ding();
					    Ding();
					    break;
				    }
               	}
				/************************************返款**********************************/
				Channel_Value = USER_Card_Check(); //
				if(Channel_Value != 0x55) 
				{
					while(RxD_Buffer != 0x55);     //等待主机指令
				    SendData(0x20+Channel_Value);  //主机接受到
					Delay_1ms(50);
					Timer_Buff = RxD_Buffer*100;
					SendData(0x33);	//无效指令
					Delay_1ms(50);
					Timer_Buff += RxD_Buffer;
					SendData(0x33);	//无效指令
					unMoney = gBuff[0]*100+gBuff[1] + ((ucPrice[0]*10+ucPrice[1]/10)*Timer_Buff)/(SK_Timer/10)-1;//这里的SK_Timer单价时间，单位分钟
					if((((ucPrice[0]*10+ucPrice[1]/10)*Timer_Buff)/(SK_Timer/10)-1)<Total_Money[1])
						Total_Money[1] -= ((ucPrice[0]*10+ucPrice[1]/10)*Timer_Buff)/(SK_Timer/10)-1;
					Back_Menoy_Flag = TURE; 
//					if((Channel>=0)&&(Channel<=9));
//					else	
					Channel = 0x0f;
				}
				/**************************************************************************/
				else 
				{	
					Back_Menoy_Flag = FALSE;
					if((Channel>=0)&&(Channel<=9))unMoney = gBuff[0]*100+gBuff[1] - ucPrice[0]*100-ucPrice[1];
					else unMoney = gBuff[0]*100+gBuff[1];
				}
				gBuff[1] = unMoney%100;
				gBuff[0] = unMoney/100;
				
				ucTmp=Write_Block(gBuff,ucWorkState*4+2);
				ucTmp=Write_Block(gBuff,ucWorkState*4+1);
				if (ucTmp!=FM1702_OK)
				{
					if(ucTmp == 1)
					{
						Ding();
					    Ding();
					    break;
					}						
				}
				
				Show(gBuff[0],gBuff[1]);
				if(!Back_Menoy_Flag) 
				{
					if((Channel>=0)&&(Channel<=9))
					{
						Total_Money[1] += (ucPrice[0]*100+ucPrice[1]);	      //单位：分
						if(Total_Money[1]>9999){Total_Money[1] -= 9999;Total_Money[0]++;}	
					}
				}
				else 
				{
					for(i=0;i<5;i++)USER_Card_ID[Channel_Value][i]=0;
					SendData(0x33);	         //该语句发送给主机，表明退款成功，可以把时间清零了
					Write_E2PROM8_CardID();	 //每次清空某个通道的ID卡号，就写E2PROM一次
				}
				if((Total_Money[0] >= 9999)&&((Total_Money[1]/100) >= 99))	 //最高纪录999999百,当达到999999时，不能清零，因为要考虑到退款问题
				{
					Total_Money[0] = 0;
					Total_Money[1] = 0;
				}
				SectorErase(0x400);
				ByteProgram(0x400,Total_Money[0]/100);
				ByteProgram(0x401,Total_Money[0]%100);
				ByteProgram(0x402,Total_Money[1]/100);
				ByteProgram(0x403,Total_Money[1]%100);
				IAP_Disable();
				if(!Back_Menoy_Flag)
				{
					if((Channel>=0)&&(Channel<=9))
					{
						while(RxD_Buffer != 0x55);//等待主机指令
						RxD_Buffer = 0;
						SendData(0x11);	 //主机接受到0x11，累加一次时间
						//for(i=0;i<5;i++)Last_gCard_UID[i]=gCard_UID[i];
					}
				}
				Ding();
				Delay_1ms(200);
				Delay_1ms(200);
				Delay_1ms(200);
				Delay_1ms(200);
				Delay_1ms(200);
				Delay_1ms(200);
				Delay_1ms(200);
				Delay_1ms(200);
				Show(0,0);
				//重新载入ucPrice的值；如果确定脉冲发出后刷卡器断电，则可以不执行
				ucPrice[0] = ByteRead(0);
				ucPrice[1] = ByteRead(1);
				IAP_Disable();
				break;
			case 0xff:
				Ding();
				Ding();
				break;
			default:
				break;
			}
				break;
		default:
			break;
		}
	}	  
}