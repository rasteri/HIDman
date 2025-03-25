

#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "ch559.h"
#include "system.h"
#include "defs.h"
#include "usbdef.h"
#include "usbhost.h"
#include "menu.h"
#include "data.h"
#include "settings.h"
#include "andyalloc.h"
#include "keyboardled.h"
#include "parsedescriptor.h"
#include "ps2protocol.h"
#include "usbll.h"

__at(0x0000) unsigned char __xdata RxBuffer[MAX_PACKET_SIZE];
__at(0x0100) unsigned char __xdata TxBuffer[MAX_PACKET_SIZE];

//root hub port
USB_HUB_PORT __xdata RootHubPort[ROOT_HUB_PORT_NUM];

//sub hub port
USB_HUB_PORT __xdata SubHubPort[ROOT_HUB_PORT_NUM][MAX_EXHUB_PORT_NUM];

UINT8 EnableRootHubPort(UINT8 rootHubIndex)
{
	if (rootHubIndex == 0)
	{
		if (USB_HUB_ST & bUHS_H0_ATTACH)
		{
			if ((UHUB0_CTRL & bUH_PORT_EN) == 0x00)
			{
				if (USB_HUB_ST & bUHS_DM_LEVEL)
				{
					RootHubPort[0].DeviceSpeed = LOW_SPEED;

					TRACE("low speed device on hub 0\r\n");
				}
				else
				{
					RootHubPort[0].DeviceSpeed = FULL_SPEED;

					TRACE("full speed device on hub 0\r\n");
				}

				if (RootHubPort[0].DeviceSpeed == LOW_SPEED)
				{
					UHUB0_CTRL |= bUH_LOW_SPEED;
				}
			}

			UHUB0_CTRL |= bUH_PORT_EN;

			return ERR_SUCCESS;
		}
	}
	else if (rootHubIndex == 1)
	{
		if (USB_HUB_ST & bUHS_H1_ATTACH)
		{
			if ((UHUB1_CTRL & bUH_PORT_EN) == 0x00)
			{
				if (USB_HUB_ST & bUHS_HM_LEVEL)
				{
					RootHubPort[1].DeviceSpeed = LOW_SPEED;

					TRACE("low speed device on hub 1\r\n");
				}
				else
				{
					RootHubPort[1].DeviceSpeed = FULL_SPEED;

					TRACE("full speed device on hub 1\r\n");
				}

				if (RootHubPort[1].DeviceSpeed == LOW_SPEED)
				{
					UHUB1_CTRL |= bUH_LOW_SPEED;
				}
			}

			UHUB1_CTRL |= bUH_PORT_EN;

			return ERR_SUCCESS;
		}
	}

	return ERR_USB_DISCON;
}

void DisableRootHubPort(UINT8 RootHubIndex)
{
	//reset data
	InitRootHubPortData(RootHubIndex);

	if (RootHubIndex == 0)
	{
		UHUB0_CTRL = 0x00;
	}
	else if (RootHubIndex == 1)
	{
		UHUB1_CTRL = 0x00;
	}
}



void InitRootHubPortData(UINT8 rootHubIndex)
{
	UINT8 i;

	InitHubPortData(&RootHubPort[rootHubIndex]);

	for (i = 0; i < MAX_EXHUB_PORT_NUM; i++)
	{
		InitHubPortData(&SubHubPort[rootHubIndex][i]);
	}
}

void SetHostUsbAddr(UINT8 addr)
{
	USB_DEV_AD = USB_DEV_AD & bUDA_GP_BIT | addr & 0x7F;
}

void SetUsbSpeed(UINT8 FullSpeed) // set current speed
{
	if (FullSpeed)
	{
		USB_CTRL &= ~bUC_LOW_SPEED;	 // ȫ��
		UH_SETUP &= ~bUH_PRE_PID_EN; // ��ֹPRE PID
	}
	else
	{
		USB_CTRL |= bUC_LOW_SPEED;
	}
}

void ResetRootHubPort(UINT8 RootHubIndex)
{
	SetHostUsbAddr(0x00);
	SetUsbSpeed(1); // Ĭ��Ϊȫ��

	if (RootHubIndex == 0)
	{
		UHUB0_CTRL = UHUB0_CTRL & ~bUH_LOW_SPEED | bUH_BUS_RESET; // Ĭ��Ϊȫ��,��ʼ��λ
		mDelaymS(15);											  // ��λʱ��10mS��20mS
		UHUB0_CTRL = UHUB0_CTRL & ~bUH_BUS_RESET;				  // ������λ
	}
	else if (RootHubIndex == 1)
	{
		UHUB1_CTRL = UHUB1_CTRL & ~bUH_LOW_SPEED | bUH_BUS_RESET; // Ĭ��Ϊȫ��,��ʼ��λ
		mDelaymS(15);											  // ��λʱ��10mS��20mS
		UHUB1_CTRL = UHUB1_CTRL & ~bUH_BUS_RESET;				  // ������λ
	}

	mDelayuS(250);
	UIF_DETECT = 0; // ���жϱ�־
}

void SelectHubPort(UINT8 RootHubIndex, UINT8 HubPortIndex)
{
	if (HubPortIndex == EXHUB_PORT_NONE)
	{
		//normal device
		SetHostUsbAddr(RootHubPort[RootHubIndex].DeviceAddress);
		SetUsbSpeed(RootHubPort[RootHubIndex].DeviceSpeed);
	}
	else
	{
		USB_HUB_PORT *pUsbDevice = &SubHubPort[RootHubIndex][HubPortIndex];
		SetHostUsbAddr(pUsbDevice->DeviceAddress);
		if (pUsbDevice->DeviceSpeed == LOW_SPEED)
		{
			UH_SETUP |= bUH_PRE_PID_EN;
		}

		SetUsbSpeed(pUsbDevice->DeviceSpeed);
	}
}

void InitUsbHost(void)
{
	UINT8 i;
	IE_USB = 0;

	USB_CTRL = bUC_HOST_MODE; // ���趨ģʽ
	USB_DEV_AD = 0x00;
	UH_EP_MOD = bUH_EP_TX_EN | bUH_EP_RX_EN;
	UH_RX_DMA = 0x0000;
	UH_TX_DMA = 0x0001;
	UH_RX_CTRL = 0x00;
	UH_TX_CTRL = 0x00;

	UHUB1_CTRL &= ~bUH1_DISABLE; //enable root hub1

	USB_CTRL = bUC_HOST_MODE | bUC_INT_BUSY | bUC_DMA_EN; // ����USB������DMA,���жϱ�־δ���ǰ�Զ���ͣ

	UH_SETUP = bUH_SOF_EN;
	USB_INT_FG = 0xFF; // ���жϱ�־
	for (i = 0; i < 2; i++)
	{
		DisableRootHubPort(i); // ���
	}
	USB_INT_EN = bUIE_TRANSFER | bUIE_DETECT;
}

UINT8 USBHostTransact(UINT8 endp_pid, UINT8 tog, UINT16 timeout)
{
	UINT8 TransRetry;
	UINT8 r;
	UINT16 i;
	UH_RX_CTRL = UH_TX_CTRL = tog;
	TransRetry = 0;

	do
	{
		// wait for next start of frame
		while (!(USB_MIS_ST & bUMS_SOF_ACT));

		UH_EP_PID = endp_pid; // ָ������PID��Ŀ�Ķ˵��
		UIF_TRANSFER = 0;	  // ��������
		for (i = WAIT_USB_TOUT_200US; i != 0 && UIF_TRANSFER == 0; i--)
		{
			;
		}

		UH_EP_PID = 0x00; // ֹͣUSB����

		if (UIF_TRANSFER == 0)
		{
			TRACE1("quit at line %d\r\n", (UINT16)__LINE__);

			return (ERR_USB_UNKNOWN);
		}

		if (UIF_TRANSFER) // �������
		{
			if (U_TOG_OK)
			{
				//TRACE1("retry:%d\r\n", (UINT16)TransRetry);

				return (ERR_SUCCESS);
			}

#if 0
			TRACE1("endp_pid=%02X\n", (UINT16)endp_pid);
			TRACE1("USB_INT_FG=%02X\n", (UINT16)USB_INT_FG);
			TRACE1("USB_INT_ST=%02X\n", (UINT16)USB_INT_ST);
			TRACE1("USB_MIS_ST=%02X\n", (UINT16)USB_MIS_ST);
			TRACE1("USB_RX_LEN=%02X\n", (UINT16)USB_RX_LEN);
			TRACE1("UH_TX_LEN=%02X\n", (UINT16)UH_TX_LEN);
			TRACE1("UH_RX_CTRL=%02X\n", (UINT16)UH_RX_CTRL);
			TRACE1("UH_TX_CTRL=%02X\n", (UINT16)UH_TX_CTRL);
			TRACE1("UHUB0_CTRL=%02X\n", (UINT16)UHUB0_CTRL);
			TRACE1("UHUB1_CTRL=%02X\n", (UINT16)UHUB1_CTRL);
#endif

			r = USB_INT_ST & MASK_UIS_H_RES; // USB�豸Ӧ��״̬
			//TRACE1("r:0x%02X\r\n", (UINT16)r);

			if (r == USB_PID_STALL)
			{
				TRACE1("quit at line %d\r\n", (UINT16)__LINE__);

				return (r | ERR_USB_TRANSFER);
			}

			if (r == USB_PID_NAK)
			{
				if (timeout == 0)
				{
					//TRACE1("quit at line %d\r\n", (UINT16)__LINE__);

					return (r | ERR_USB_TRANSFER);
				}
				if (timeout < 0xFFFF)
				{
					timeout--;
				}

				if (TransRetry > 0)
				{
					TransRetry--;
				}
			}
			else
			{
				switch (endp_pid >> 4)
				{
				case USB_PID_SETUP:
				case USB_PID_OUT:
					if (r)
					{
						TRACE1("quit at line %d\r\n", (UINT16)__LINE__);

						return (r | ERR_USB_TRANSFER); // ���ǳ�ʱ/����,����Ӧ��
					}
					break; // ��ʱ����

				case USB_PID_IN:
					if (r == USB_PID_DATA0 && r == USB_PID_DATA1) // ��ͬ�����趪��������
					{
					} // ��ͬ������
					else if (r)
					{
						TRACE1("quit at line %d\r\n", (UINT16)__LINE__);

						return (r | ERR_USB_TRANSFER); // ���ǳ�ʱ/����,����Ӧ��
					}
					break; // ��ʱ����
				default:

					return (ERR_USB_UNKNOWN); // �����ܵ����
					break;
				}
			}
		}
		else // �����ж�,��Ӧ�÷��������
		{
			USB_INT_FG = 0xFF; //���жϱ�־
		}
		mDelayuS(25);
	} while (++TransRetry < 250);

	TRACE1("quit at line %d\r\n", (UINT16)__LINE__);

	return (ERR_USB_TRANSFER); // Ӧ��ʱ
}

UINT8 HostCtrlTransfer(USB_SETUP_REQ *pSetupReq, UINT8 MaxPacketSize0, PUINT8 DataBuf, PUINT16 RetLen)
{
	UINT16 RemLen = 0;
	UINT8 s, RxLen, RxCnt, TxCnt;
	PUINT8 pBuf;
	PUINT16 pLen;

	pBuf = DataBuf;
	pLen = RetLen;
	mDelayuS(200);
	if (pLen)
	{
		*pLen = 0; // ʵ�ʳɹ��շ����ܳ���
	}

	for (TxCnt = 0; TxCnt < sizeof(USB_SETUP_REQ); TxCnt++)
	{
		TxBuffer[TxCnt] = ((UINT8 *)pSetupReq)[TxCnt];
	}

	UH_TX_LEN = sizeof(USB_SETUP_REQ);

	s = USBHostTransact(USB_PID_SETUP << 4, 0x00, 10000); // SETUP�׶�,200mS��ʱ
	if (s != ERR_SUCCESS)
	{
		TRACE1("quit at line %d\r\n", (UINT16)__LINE__);

		return (s);
	}
	UH_RX_CTRL = UH_TX_CTRL = bUH_R_TOG | bUH_R_AUTO_TOG | bUH_T_TOG | bUH_T_AUTO_TOG; // Ĭ��DATA1
	UH_TX_LEN = 0x01;																   // Ĭ�������ݹ�״̬�׶�ΪIN
	RemLen = (pSetupReq->wLengthH << 8) | (pSetupReq->wLengthL);
	if (RemLen && pBuf) // ��Ҫ�շ�����
	{
		if (pSetupReq->bRequestType & USB_REQ_TYP_IN) // ��
		{
			while (RemLen)
			{
				mDelayuS(200);
				s = USBHostTransact(USB_PID_IN << 4, UH_RX_CTRL, 10000); // IN����
				if (s != ERR_SUCCESS)
				{
					TRACE1("quit at line %d\r\n", (UINT16)__LINE__);

					return (s);
				}
				RxLen = USB_RX_LEN < RemLen ? USB_RX_LEN : RemLen;
				RemLen -= RxLen;
				if (pLen)
				{
					*pLen += RxLen; // ʵ�ʳɹ��շ����ܳ���
				}

				for (RxCnt = 0; RxCnt != RxLen; RxCnt++)
				{
					*pBuf = RxBuffer[RxCnt];
					pBuf++;
				}
				if (USB_RX_LEN == 0 || (USB_RX_LEN < MaxPacketSize0))
				{
					break; // �̰�
				}
			}
			UH_TX_LEN = 0x00; // ״̬�׶�ΪOUT
		}
		else // ��
		{
			while (RemLen)
			{
				mDelayuS(200);
				UH_TX_LEN = RemLen >= MaxPacketSize0 ? MaxPacketSize0 : RemLen;

				for (TxCnt = 0; TxCnt != UH_TX_LEN; TxCnt++)
				{
					TxBuffer[TxCnt] = *pBuf;
					pBuf++;
				}
				s = USBHostTransact(USB_PID_OUT << 4 | 0x00, UH_TX_CTRL, 200000 / 20); // OUT����
				if (s != ERR_SUCCESS)
				{
					TRACE1("quit at line %d\r\n", (UINT16)__LINE__);

					return (s);
				}
				RemLen -= UH_TX_LEN;
				if (pLen)
				{
					*pLen += UH_TX_LEN; // ʵ�ʳɹ��շ����ܳ���
				}
			}

			// ״̬�׶�ΪIN
		}
	}

	mDelayuS(200);
	s = USBHostTransact((UH_TX_LEN ? USB_PID_IN << 4 | 0x00 : USB_PID_OUT << 4 | 0x00), bUH_R_TOG | bUH_T_TOG, 200000 / 20); // STATUS�׶�
	if (s != ERR_SUCCESS)
	{
		TRACE1("quit at line %d\r\n", (UINT16)__LINE__);

		return (s);
	}
	if (UH_TX_LEN == 0)
	{
		return (ERR_SUCCESS); // ״̬OUT
	}
	if (USB_RX_LEN == 0)
	{
		return (ERR_SUCCESS); // ״̬IN,���IN״̬�������ݳ���
	}
	return (ERR_USB_BUF_OVER); // IN״̬�׶δ���
}

UINT8 TransferReceive(ENDPOINT *pEndPoint, UINT8 *pData, UINT16 *pRetLen, UINT16 timeout)
{
	UINT8 s;
	UINT8 len;

	s = USBHostTransact(USB_PID_IN << 4 | (pEndPoint->EndpointAddr & 0x7F), pEndPoint->TOG ? bUH_R_TOG | bUH_T_TOG : 0, timeout); // CH559��������,��ȡ����,NAK������
	if (s == ERR_SUCCESS)
	{
		UINT8 i;
		len = USB_RX_LEN;

		for (i = 0; i < len; i++)
		{
			*pData++ = RxBuffer[i];
		}

		if (pRetLen != NULL)
		{
			*pRetLen = len;
		}

		pEndPoint->TOG = pEndPoint->TOG ? FALSE : TRUE;
	}

	return (s);
}