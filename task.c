
#include <stdio.h>
#include "type.h"

#include "mcu.h"
#include "system.h"
#include "uart.h"

#include "usbhost.h"
#include "keyboardled.h"

#include "ps2protocol.h"


#include "task.h"

#include "trace.h"
#include "defs.h"
#include "util.h"

static UINT8 volatile s_5MsCounter = 0;

static BOOL volatile s_CheckUsbPort0 = FALSE;
static BOOL volatile s_CheckUsbPort1 = FALSE;

void mTimer2Interrupt(void) __interrupt(5)
{

	if (TF2)
    { 	

        TF2 = 0; 
		
		//uart receive timeout
		//RecvBufferTimerout();

		if (s_5MsCounter == 0)
		{
			s_CheckUsbPort0 = TRUE;
		}
		else if (s_5MsCounter == 1)
		{
			s_CheckUsbPort1 = TRUE;
		}
		
		s_5MsCounter++;
		if (s_5MsCounter == 2)
		{
			s_5MsCounter = 0;
		}

	}
}
void mTimer2Setup(UINT8 T2Out)
{
    RCLK = 0;
    TCLK = 0;
    CP_RL2 = 0;                                                                //�����Զ����ض�ʱ������
    if(T2Out)
    {
	      T2MOD |= T2OE;                                                        //�Ƿ�����T2���ʱ��,�������ʱ��=1/2��ʱ��2�����
    }
    else
    {
	      T2MOD &= ~T2OE;
    }
}

void mTimer2Init(UINT16 Tim)
{
    UINT16 tmp;
    tmp = 65536 - Tim;
    RCAP2L = TL2 = tmp & 0xff;
    RCAP2H = TH2 = (tmp >> 8) & 0xff;
}


static void InitTimer2(void)
{
	mTimer2Clk12DivFsys( ); 												//ʱ��ѡ��Fsys��ʱ����ʽ
	mTimer2Setup(0);														//��ʱ��������ʾ
	mTimer2Init(FREQ_SYS / 12 * 5 / 1000);									//��ʱ������ֵ
	
	ET2 = 1;																//ʹ��ȫ���ж�
	mTimer2RunCTL(1); 	
																	//������ʱ��
}

void InitSystem(void)
{
	CfgFsys();      //CH559ʱ��ѡ������   
    mDelaymS(500);   


	//InitRecvBuffer();
	
	InitTimer2();

#if !defined(BOARD_MICRO)
	InitUART0();
#endif

	InitUsbData();
	InitUsbHost();

	HAL_ENABLE_INTERRUPTS();


	
	TRACE("system init\r\n");
}

void ProcessUsbHostPort()
{	

	DealUsbPort();
	if (s_CheckUsbPort0)
	{
		s_CheckUsbPort0 = FALSE;
		
		InterruptProcessRootHubPort(0);
	}
	if (s_CheckUsbPort1)
	{
		s_CheckUsbPort1 = FALSE;

		InterruptProcessRootHubPort(1);
	}
}



/*void ProcessKeyboardLed()
{
	if (!IsRecvBufferEmpty())
	{
		UINT8 *packet = GetOutputBuffer();
		UINT8 id = packet[0];
		UINT8 *pData = &packet[1];

		switch (id)
		{
		case ID_LED_STATUS:
			{
				UINT8 led = pData[0];
				UINT8 ledSave = GetKeyboardLedStatus();
				if (led != ledSave)
				{
					
					UpdateUsbKeyboardLed(led);

					SetKeyboardLedStatus(led);
				}
			}
			
			break;

		default:
			break;
		}
	}
}*/

