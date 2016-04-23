#include "hwa_uart.h"
#include "sys_uart.h"
#include "string.h"

UINT8 u8ErrorCnt = 0;

#define MSG_LEN		((UINT8)8)
#define MSG_HEAD	(0xA5)

typedef enum
{
    /*Req Cmd*/
    REQ_STATE       = 0x00,
    REQ_REFUND,
    REQ_CHANNEL,
    REQ_PAUSE_BRUSH,
    
    /*Resp Cmd*/
    RESP_NONE       = 0x80,
    RESP_BRUSH,
    RESP_PAUSE_BRUSH,
    RESP_CLR_BRUSH,
    RESP_REFUND,
    RESP_CLR_REFUND,
    
	RX_NONE         = 0xF0,
	RX_RIGHT        = 0xF1,
	RX_LEN_ERROR    = 0xF2,
	RX_HEAD_ERROR   = 0xF3,
	RX_CRC_ERROR    = 0xF4,
    RX_CMD_ERROR    = 0xF5,
}STATE_ENUM;

UINT8 u8_PauseBrushDlyCnt = 0;
BOOL b_Brush = FALSE;
UINT8 u8_BrushCount = 0;
BOOL b_Refund = FALSE;
BOOL b_RefundSuccess = FALSE;
UINT8 u8_RefundChannel = 0;

static UINT8 LastBrush = 0;

typedef struct
{
	UINT8 head;
	STATE_ENUM cmd;
	UINT8 dat[4];
	UINT8 crc[2];
}MSG_STRUCT;

MSG_STRUCT Rx;
STATE_ENUM state = RX_NONE;

UINT16 CRC16_RTU( UINT8 *dat, UINT8 len)
{
    UINT16 crc = 0xFFFF;
    UINT8 i;

    while(len--)
    {
        crc = crc ^*dat++;
        for ( i = 0; i < 8; i++)
        {
            if( ( crc & 0x0001) > 0)
            {
                crc = crc >> 1;
                crc = crc ^ 0xa001;
            }
            else
                crc = crc >> 1;
        }
    }
    return ( crc);
}

void hwa_uartInit(void)
{
    sys_uartInit();
}

void hwa_uartHandler1ms(void)
{
	UINT8 len;
	if(sys_uartOverTime1ms()!=FALSE)
	{
		len = sys_uartReadData((UINT8*)&Rx);
		if(len < MSG_LEN)
		{
			state = RX_LEN_ERROR;
		}
		else if(Rx.head != MSG_HEAD)
		{
			state = RX_HEAD_ERROR;
		}
		else if(CRC16_RTU((UINT8*)&Rx, MSG_LEN-2)!=(Rx.crc[0]|Rx.crc[1]<<8))
		{
			state = RX_CRC_ERROR;
		}
		else
		{
			state = RX_RIGHT;
		}
	}
}

void hwa_uartHandler10ms(void)
{
	UINT16 crc;
	MSG_STRUCT Tx = {0};
	if(state != RX_NONE)
	{
		Tx.head = MSG_HEAD;
		if(state != RX_RIGHT)
		{
			Tx.cmd = state;
			u8ErrorCnt += 2;
			if(u8ErrorCnt>=10)
			{
				u8ErrorCnt = 10;
			}
		}
		else
		{
			if(u8ErrorCnt)
			{
				u8ErrorCnt--;
			}
			Tx.cmd = RESP_NONE;
			switch(Rx.cmd)
			{
				case REQ_STATE:
                    if(b_Brush)
                    {
                        Tx.cmd = RESP_BRUSH;
                        Tx.dat[0] = u8_BrushCount;      //dat[0]返回扣款次数
                    }
                    else if(b_Refund)
                    {
                        Tx.cmd = RESP_REFUND;
                    }
					break;
					
				case REQ_REFUND:
                    if(b_RefundSuccess)
                    {
                        Tx.cmd = RESP_CLR_REFUND;
                    }
					break;
					
				case REQ_CHANNEL:
                    u8_BrushCount = 0;
                    Tx.cmd = RESP_CLR_BRUSH;
                    Tx.dat[0] = u8_RefundChannel;
					break;
					
				case REQ_PAUSE_BRUSH:           //请求暂停刷卡
                    u8_PauseBrushDlyCnt = PAUSE_BRUSH_DLY_CNT;
                    Tx.cmd = RESP_PAUSE_BRUSH;
                    break;
                    
				default:
					Tx.cmd = RX_CMD_ERROR;
					break;
    		}
		}
		crc = CRC16_RTU((UINT8*)&Tx, MSG_LEN-2);
		Tx.crc[0] = crc;
		Tx.crc[1] = crc>>8;
		sys_uartSendData((UINT8*)&Tx, MSG_LEN);
		state = RX_NONE;
	}
	if(u8_PauseBrushDlyCnt)
	{
		 u8_PauseBrushDlyCnt--;
	}
}

