
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
#include "util.h"
#include "settings.h"

#include "keyboardled.h"
#include "parsedescriptor.h"

#include "trace.h"

#define WAIT_USB_TOUT_200US 800 // �ȴ�USB�жϳ�ʱʱ��200uS

__at(0x0000) unsigned char __xdata RxBuffer[MAX_PACKET_SIZE];
__at(0x0100) unsigned char __xdata TxBuffer[MAX_PACKET_SIZE];

#define RECEIVE_BUFFER_LEN 512
static UINT8X ReceiveDataBuffer[RECEIVE_BUFFER_LEN];

//root hub port
static USB_HUB_PORT __xdata RootHubPort[ROOT_HUB_PORT_NUM];

//sub hub port
static USB_HUB_PORT __xdata SubHubPort[ROOT_HUB_PORT_NUM][MAX_EXHUB_PORT_NUM];
static INTERFACE __xdata sInterfacePool[MAX_GLOBAL_INTERFACE_NUM];
static UINT8 sInterfacePoolPos = 0;

static void InitInterface(INTERFACE* Interface)
{
	Interface->InterfaceClass = USB_DEV_CLASS_RESERVED;
	Interface->InterfaceProtocol = USB_PROTOCOL_NONE;
	Interface->ReportSize = 0;
	Interface->EndpointNum = 0;

	for (int j = 0; j < MAX_ENDPOINT_NUM; j++)
	{
		Interface->Endpoint[j].EndpointAddr = 0;
		Interface->Endpoint[j].MaxPacketSize = 0;
		Interface->Endpoint[j].EndpointDir = ENDPOINT_IN;
		Interface->Endpoint[j].TOG = FALSE;
	}
}

INTERFACE* AllocInterface(UINT8 count)
{
	if (count + sInterfacePoolPos > MAX_GLOBAL_INTERFACE_NUM)
		return 0;

	INTERFACE* ptr = sInterfacePool + sInterfacePoolPos;
	sInterfacePoolPos += count;

	for (UINT8 i = 0; i < count; ++i)
		InitInterface(ptr + i);

	return ptr;
}

static void InitHubPortData(USB_HUB_PORT *pUsbHubPort)
{
	pUsbHubPort->HubPortStatus = PORT_DEVICE_NONE;
	pUsbHubPort->UsbDevice.DeviceClass = USB_DEV_CLASS_RESERVED;
	pUsbHubPort->UsbDevice.MaxPacketSize0 = DEFAULT_ENDP0_SIZE;

	pUsbHubPort->UsbDevice.VendorID = 0x0000;
	pUsbHubPort->UsbDevice.ProductID = 0x0000;
	pUsbHubPort->UsbDevice.bcdDevice = 0x0000;

	pUsbHubPort->UsbDevice.DeviceAddress = 0;
	pUsbHubPort->UsbDevice.DeviceSpeed = FULL_SPEED;
	pUsbHubPort->UsbDevice.InterfaceNum = 0;
	pUsbHubPort->UsbDevice.Interface = 0;

	pUsbHubPort->UsbDevice.HubPortNum = 0;
}

static void InitRootHubPortData(UINT8 rootHubIndex)
{
	UINT8 i;

	InitHubPortData(&RootHubPort[rootHubIndex]);

	for (i = 0; i < MAX_EXHUB_PORT_NUM; i++)
	{
		InitHubPortData(&SubHubPort[rootHubIndex][i]);
	}
}

static UINT8 EnableRootHubPort(UINT8 rootHubIndex)
{
	if (rootHubIndex == 0)
	{
		if (USB_HUB_ST & bUHS_H0_ATTACH)
		{
			if ((UHUB0_CTRL & bUH_PORT_EN) == 0x00)
			{
				if (USB_HUB_ST & bUHS_DM_LEVEL)
				{
					RootHubPort[0].UsbDevice.DeviceSpeed = LOW_SPEED;

					TRACE("low speed device on hub 0\r\n");
				}
				else
				{
					RootHubPort[0].UsbDevice.DeviceSpeed = FULL_SPEED;

					TRACE("full speed device on hub 0\r\n");
				}

				if (RootHubPort[0].UsbDevice.DeviceSpeed == LOW_SPEED)
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
					RootHubPort[1].UsbDevice.DeviceSpeed = LOW_SPEED;

					TRACE("low speed device on hub 1\r\n");
				}
				else
				{
					RootHubPort[1].UsbDevice.DeviceSpeed = FULL_SPEED;

					TRACE("full speed device on hub 1\r\n");
				}

				if (RootHubPort[1].UsbDevice.DeviceSpeed == LOW_SPEED)
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

static void DisableRootHubPort(UINT8 RootHubIndex)
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

static void SetHostUsbAddr(UINT8 addr)
{
	USB_DEV_AD = USB_DEV_AD & bUDA_GP_BIT | addr & 0x7F;
}

static void SetUsbSpeed(UINT8 FullSpeed) // set current speed
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

static void ResetRootHubPort(UINT8 RootHubIndex)
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

static void SelectHubPort(UINT8 RootHubIndex, UINT8 HubPortIndex)
{
	if (HubPortIndex == EXHUB_PORT_NONE)
	{
		//normal device
		SetHostUsbAddr(RootHubPort[RootHubIndex].UsbDevice.DeviceAddress);
		SetUsbSpeed(RootHubPort[RootHubIndex].UsbDevice.DeviceSpeed);
	}
	else
	{
		USB_DEVICE *pUsbDevice = &SubHubPort[RootHubIndex][HubPortIndex].UsbDevice;
		SetHostUsbAddr(pUsbDevice->DeviceAddress);
		if (pUsbDevice->DeviceSpeed == LOW_SPEED)
		{
			UH_SETUP |= bUH_PRE_PID_EN;
		}

		SetUsbSpeed(pUsbDevice->DeviceSpeed);
	}
}

void InitUsbHost()
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

static UINT8 USBHostTransact(UINT8 endp_pid, UINT8 tog, UINT16 timeout)
{
	UINT8 TransRetry;
	UINT8 r;
	UINT16 i;
	UH_RX_CTRL = UH_TX_CTRL = tog;
	TransRetry = 0;

	do
	{
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
	} while (++TransRetry < 60);

	TRACE1("quit at line %d\r\n", (UINT16)__LINE__);

	return (ERR_USB_TRANSFER); // Ӧ��ʱ
}

static UINT8 HostCtrlTransfer(USB_SETUP_REQ *pSetupReq, UINT8 MaxPacketSize0, PUINT8 DataBuf, PUINT16 RetLen)
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

static void FillSetupReq(USB_SETUP_REQ *pSetupReq, UINT8 type, UINT8 req, UINT16 value, UINT16 index, UINT16 length)
{
	pSetupReq->bRequestType = type;
	pSetupReq->bRequest = req;
	pSetupReq->wValueL = value & 0xff;
	pSetupReq->wValueH = (value >> 8) & 0xff;
	pSetupReq->wIndexL = index & 0xff;
	pSetupReq->wIndexH = (index >> 8) & 0xff;
	pSetupReq->wLengthL = length & 0xff;
	pSetupReq->wLengthH = (length >> 8) & 0xff;
}

//-----------------------------------------------------------------------------------------
static UINT8 GetDeviceDescr(USB_DEVICE *pUsbDevice, UINT8 *pDevDescr, UINT16 reqLen, UINT16 *pRetLen) //get device describtion
{
	UINT8 s;
	UINT16 len;

	USB_SETUP_REQ SetupReq;
	FillSetupReq(&SetupReq, USB_REQ_TYP_IN | USB_REQ_TYP_STANDARD | USB_REQ_RECIP_DEVICE,
				 USB_GET_DESCRIPTOR, USB_DESCR_TYP_DEVICE << 8, 0, reqLen);

	s = HostCtrlTransfer(&SetupReq, pUsbDevice->MaxPacketSize0, pDevDescr, &len);

	if (s == ERR_SUCCESS)
	{
		if (pRetLen != NULL)
		{
			*pRetLen = len;
		}
	}

	return s;
}

//----------------------------------------------------------------------------------------
static UINT8 GetConfigDescr(USB_DEVICE *pUsbDevice, UINT8 *pCfgDescr, UINT16 reqLen, UINT16 *pRetLen)
{
	UINT8 s;
	UINT16 len;

	USB_SETUP_REQ SetupReq;
	FillSetupReq(&SetupReq, USB_REQ_TYP_IN | USB_REQ_TYP_STANDARD | USB_REQ_RECIP_DEVICE,
				 USB_GET_DESCRIPTOR, USB_DESCR_TYP_CONFIG << 8, 0, reqLen);
	s = HostCtrlTransfer(&SetupReq, pUsbDevice->MaxPacketSize0, pCfgDescr, &len);
	if (s == ERR_SUCCESS)
	{
		if (pRetLen != NULL)
		{
			*pRetLen = len;
		}
	}

	return s;
}

//-------------------------------------------------------------------------------------
static UINT8 SetUsbAddress(USB_DEVICE *pUsbDevice, UINT8 addr)
{
	UINT8 s;

	USB_SETUP_REQ SetupReq;
	FillSetupReq(&SetupReq, USB_REQ_TYP_OUT | USB_REQ_TYP_STANDARD | USB_REQ_RECIP_DEVICE,
				 USB_SET_ADDRESS, addr, 0, 0);

	s = HostCtrlTransfer(&SetupReq, pUsbDevice->MaxPacketSize0, NULL, NULL);
	if (s == ERR_SUCCESS)
	{
		SetHostUsbAddr(addr);
	}

	return s;
}

//-------------------------------------------------------------------------------------
static UINT8 SetUsbConfig(USB_DEVICE *pUsbDevice, UINT8 cfg)
{
	UINT8 s;

	USB_SETUP_REQ SetupReq;
	FillSetupReq(&SetupReq, USB_REQ_TYP_OUT | USB_REQ_TYP_STANDARD | USB_REQ_RECIP_DEVICE,
				 USB_SET_CONFIGURATION, cfg, 0, 0);

	s = HostCtrlTransfer(&SetupReq, pUsbDevice->MaxPacketSize0, NULL, NULL);

	return s;
}

//-----------------------------------------------------------------------------------------
static UINT8 GetHubDescriptor(USB_DEVICE *pUsbDevice, UINT8 *pHubDescr, UINT16 reqLen, UINT16 *pRetLen)
{
	UINT8 s;
	UINT16 len;

	USB_SETUP_REQ SetupReq;
	FillSetupReq(&SetupReq, USB_REQ_TYP_IN | USB_REQ_TYP_CLASS | USB_REQ_RECIP_DEVICE,
				 USB_GET_DESCRIPTOR, USB_DESCR_TYP_HUB << 8, 0, reqLen);
	s = HostCtrlTransfer(&SetupReq, pUsbDevice->MaxPacketSize0, (UINT8 *)pHubDescr, &len);

	if (s == ERR_SUCCESS)
	{
		if (pRetLen != NULL)
		{
			*pRetLen = len;
		}
	}

	return s;
}

//-----------------------------------------------------------------------------------------
static UINT8 GetHubPortStatus(USB_DEVICE *pUsbDevice, UINT8 HubPort, UINT16 *pPortStatus, UINT16 *pPortChange)
{
	UINT8 s;
	UINT16 len;

	UINT8 Ret[4];

	USB_SETUP_REQ SetupReq;
	FillSetupReq(&SetupReq, USB_REQ_TYP_IN | USB_REQ_TYP_CLASS | USB_REQ_RECIP_OTHER,
				 USB_GET_STATUS, 0, HubPort, 4);

	s = HostCtrlTransfer(&SetupReq, pUsbDevice->MaxPacketSize0, Ret, &len);
	if (s == ERR_SUCCESS)
	{
		if (pPortStatus != NULL)
		{
			*pPortStatus = (Ret[1] << 8) | Ret[0];
		}

		if (pPortChange != NULL)
		{
			*pPortChange = (Ret[3] << 8) | Ret[2];
		}
	}

	return s;
}

//------------------------------------------------------------------------------------------
static UINT8 SetHubPortFeature(USB_DEVICE *pUsbDevice, UINT8 HubPort, UINT8 selector) //this function set feature for port						//this funciton set
{
	UINT8 s;

	USB_SETUP_REQ SetupReq;
	FillSetupReq(&SetupReq, USB_REQ_TYP_OUT | USB_REQ_TYP_CLASS | USB_REQ_RECIP_OTHER,
				 USB_SET_FEATURE, selector, (0 << 8) | HubPort, 0);

	s = HostCtrlTransfer(&SetupReq, pUsbDevice->MaxPacketSize0, NULL, NULL);

	return s;
}

static UINT8 ClearHubPortFeature(USB_DEVICE *pUsbDevice, UINT8 HubPort, UINT8 selector)
{
	UINT8 s;

	USB_SETUP_REQ SetupReq;
	FillSetupReq(&SetupReq, USB_REQ_TYP_OUT | USB_REQ_TYP_CLASS | USB_REQ_RECIP_OTHER,
				 USB_CLEAR_FEATURE, selector, (0 << 8) | HubPort, 0);

	s = HostCtrlTransfer(&SetupReq, pUsbDevice->MaxPacketSize0, NULL, NULL);

	return s;
}

//-----------------------------------------------------------------------------------------
static UINT8 GetReportDescriptor(USB_DEVICE *pUsbDevice, UINT8 interface, UINT8 *pReportDescr, UINT16 reqLen, UINT16 *pRetLen)
{
	UINT8 s;
	UINT16 len;

	USB_SETUP_REQ SetupReq;
	FillSetupReq(&SetupReq, USB_REQ_TYP_IN | USB_REQ_TYP_STANDARD | USB_REQ_RECIP_INTERF,
				 USB_GET_DESCRIPTOR, USB_DESCR_TYP_REPORT << 8, interface, reqLen);

	s = HostCtrlTransfer(&SetupReq, pUsbDevice->MaxPacketSize0, pReportDescr, &len);

	if (s == ERR_SUCCESS)
	{
		if (pRetLen != NULL)
		{
			*pRetLen = len;
		}
	}

	return s;
}

static UINT8 SetBootProtocol(USB_DEVICE *pUsbDevice, UINT8 interface)
{
	UINT8 s;

	USB_SETUP_REQ SetupReq;

	FillSetupReq(&SetupReq, 0b00100001, HID_SET_PROTOCOL, 0, interface, 0);

	s = HostCtrlTransfer(&SetupReq, pUsbDevice->MaxPacketSize0, NULL, NULL);
	return s;
}

static UINT8 GetBootProtocol(USB_DEVICE *pUsbDevice, UINT8 interface)
{
	UINT8 s;
	UINT8 ret;

	USB_SETUP_REQ SetupReq;

	FillSetupReq(&SetupReq, 0b10100001, HID_GET_PROTOCOL, 0, interface, 1);

	s = HostCtrlTransfer(&SetupReq, pUsbDevice->MaxPacketSize0, &ret, 1);

	return ret;
}

//-----------------------------------------------------------------------------------------
static UINT8 SetIdle(USB_DEVICE *pUsbDevice, UINT16 durationMs, UINT8 reportID, UINT8 interface)
{
	UINT8 s;

	USB_SETUP_REQ SetupReq;
	UINT8 duration = (UINT8)(durationMs / 4);

	FillSetupReq(&SetupReq, USB_REQ_TYP_OUT | USB_REQ_TYP_CLASS | USB_REQ_RECIP_INTERF,
				 HID_SET_IDLE, (duration << 8) | reportID, interface, 0);

	s = HostCtrlTransfer(&SetupReq, pUsbDevice->MaxPacketSize0, NULL, NULL);

	return s;
}

//-----------------------------------------------------------------------------------------------
static UINT8 SetReport(USB_DEVICE *pUsbDevice, UINT8 interface, UINT8 *pReport, UINT16 ReportLen)
{
	UINT8 s;
	UINT16 len;

	USB_SETUP_REQ SetupReq;
	FillSetupReq(&SetupReq, USB_REQ_TYP_OUT | USB_REQ_TYP_CLASS | USB_REQ_RECIP_INTERF,
				 HID_SET_REPORT, HID_REPORT_OUTPUT << 8, interface, ReportLen);

	s = HostCtrlTransfer(&SetupReq, pUsbDevice->MaxPacketSize0, pReport, &len);

	return s;
}

//-----------------------------------------------------------------------------------------------
void InitUsbData(void)
{
	int i;

	for (i = 0; i < ROOT_HUB_PORT_NUM; i++)
	{
		InitRootHubPortData(i);
	}
}

static UINT8 TransferReceive(ENDPOINT *pEndPoint, UINT8 *pData, UINT16 *pRetLen)
{
	UINT8 s;
	UINT8 len;

	s = USBHostTransact(USB_PID_IN << 4 | (pEndPoint->EndpointAddr & 0x7F), pEndPoint->TOG ? bUH_R_TOG | bUH_T_TOG : 0, 0); // CH559��������,��ȡ����,NAK������
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

//-------------------------------------------------------------------------------------------
static UINT8 HIDDataTransferReceive(USB_DEVICE *pUsbDevice)
{
	UINT8 s, p;
	int i, j;
	int interfaceNum;
	int endpointNum;

	UINT16 len;
	interfaceNum = pUsbDevice->InterfaceNum;
	for (i = 0; i < interfaceNum; i++)
	{
		INTERFACE *pInterface = &pUsbDevice->Interface[i];
		if (pInterface->InterfaceClass == USB_DEV_CLASS_HID)
		{
			endpointNum = pInterface->EndpointNum;
			for (j = 0; j < endpointNum; j++)
			{
				ENDPOINT *pEndPoint = &pInterface->Endpoint[j];
				if (pEndPoint->EndpointDir == ENDPOINT_IN)
				{
					s = TransferReceive(pEndPoint, ReceiveDataBuffer, &len);
					if (s == ERR_SUCCESS)
					{
						//TRACE1("interface %d data:", (UINT16)i);
						// HIS IS WHERE THE FUN STUFF GOES
						//ProcessHIDData(pInterface, ReceiveDataBuffer, len);
						ParseReport(&pInterface->HidSegStruct, len * 8, ReceiveDataBuffer);
						if (DumpReport)
						{
							SendKeyboardString("I%hX L%X- ", i, len);
							for (p = 0; p < len; p++)
							{
								SendKeyboardString("%hX ", ReceiveDataBuffer[p]);
							}
							SendKeyboardString("\n");
						}
						DEBUG_OUT("I%h02X L%h02X- ", i, len);
						for (p = 0; p < len; p++)
						{
							DEBUG_OUT("%h02X ", ReceiveDataBuffer[p]);
						}
						DEBUG_OUT("\n");
					}
				}
			}
		}
	}

	return (s);
}

//enum device
static BOOL EnumerateHubPort(USB_HUB_PORT *pUsbHubPort, UINT8 addr)
{
	UINT8 i, s;
	UINT16 len;
	UINT16 cfgDescLen;

	USB_DEVICE *pUsbDevice;
	USB_CFG_DESCR *pCfgDescr;

	pUsbDevice = &pUsbHubPort->UsbDevice;

	//get first 8 bytes of device descriptor to get maxpacketsize0
	s = GetDeviceDescr(pUsbDevice, ReceiveDataBuffer, 8, &len);

	if (s != ERR_SUCCESS)
	{
		TRACE1("GetDeviceDescr failed,s:0x%02X\r\n", (UINT16)s);

		return (FALSE);
	}
	TRACE("GetDeviceDescr OK\r\n");
	TRACE1("len=%d\r\n", len);

	ParseDeviceDescriptor((USB_DEV_DESCR *)ReceiveDataBuffer, len, pUsbDevice);

	TRACE1("MaxPacketSize0=%bd\r\n", pUsbDevice->MaxPacketSize0);

	//set device address
	s = SetUsbAddress(pUsbDevice, addr);
	if (s != ERR_SUCCESS)
	{
		return (FALSE);
	}

	pUsbDevice->DeviceAddress = addr;
	TRACE("SetUsbAddress OK\r\n");
	TRACE1("address=%bd\r\n", pUsbDevice->DeviceAddress);

	//get full bytes of device descriptor
	s = GetDeviceDescr(pUsbDevice, ReceiveDataBuffer, sizeof(USB_DEV_DESCR), &len);

	if (s != ERR_SUCCESS)
	{
		TRACE("GetDeviceDescr failed\r\n");

		return (FALSE);
	}

	TRACE("GetDeviceDescr OK\r\n");
	TRACE1("len=%d\r\n", len);

	ParseDeviceDescriptor((USB_DEV_DESCR *)ReceiveDataBuffer, len, pUsbDevice);
	TRACE3("VendorID=0x%04X,ProductID=0x%04X,bcdDevice=0x%04X\r\n", pUsbDevice->VendorID, pUsbDevice->ProductID, pUsbDevice->bcdDevice);

	//get configure descriptor for the first time
	cfgDescLen = sizeof(USB_CFG_DESCR);
	TRACE1("GetConfigDescr with cfgDescLen=%d\r\n", cfgDescLen);
	s = GetConfigDescr(pUsbDevice, ReceiveDataBuffer, cfgDescLen, &len);
	if (s != ERR_SUCCESS)
	{
		TRACE("GetConfigDescr 1 failed\r\n");

		return (FALSE);
	}

	//get configure descriptor for the second time
	pCfgDescr = (USB_CFG_DESCR *)ReceiveDataBuffer;
	cfgDescLen = pCfgDescr->wTotalLengthL | (pCfgDescr->wTotalLengthH << 8);
	if (cfgDescLen > RECEIVE_BUFFER_LEN)
	{
		cfgDescLen = RECEIVE_BUFFER_LEN;
	}
	TRACE1("GetConfigDescr with cfgDescLen=%d\r\n", cfgDescLen);

	s = GetConfigDescr(pUsbDevice, ReceiveDataBuffer, cfgDescLen, &len);
	if (s != ERR_SUCCESS)
	{
		TRACE("GetConfigDescr 2 failed\r\n");

		return (FALSE);
	}

	//parse config descriptor
	ParseConfigDescriptor((USB_CFG_DESCR *)ReceiveDataBuffer, len, pUsbDevice);

	TRACE("GetConfigDescr OK\r\n");
	TRACE1("len=%d\r\n", len);

	//set config
	s = SetUsbConfig(pUsbDevice, ((USB_CFG_DESCR *)ReceiveDataBuffer)->bConfigurationValue);
	if (s != ERR_SUCCESS)
	{
		return (FALSE);
	}

	TRACE("SetUsbConfig OK\r\n");
	TRACE1("configure=%bd\r\n", ((USB_CFG_DESCR *)ReceiveDataBuffer)->bConfigurationValue);

	TRACE1("pUsbDevice->InterfaceNum=%d\r\n", (UINT16)pUsbDevice->InterfaceNum);

	return (TRUE);
}

static UINT8 AssignUniqueAddress(UINT8 RootHubIndex, UINT8 HubPortIndex)
{
	UINT8 address;
	if (HubPortIndex == EXHUB_PORT_NONE)
	{
		address = (MAX_EXHUB_PORT_NUM + 1) * RootHubIndex + 1;
	}
	else
	{
		address = (MAX_EXHUB_PORT_NUM + 1) * RootHubIndex + 1 + HubPortIndex + 1;
	}

	return address;
}

static BOOL EnumerateRootHubPort(UINT8 port)
{
	UINT8 i, s;
	UINT16 len;

	UINT8 retry = 0;

	UINT8 addr;

	if (RootHubPort[port].HubPortStatus != PORT_DEVICE_INSERT)
	{
		return FALSE;
	}

	TRACE1("enumerate port:%bd\r\n", port);

	ResetRootHubPort(port);

#if 1
	for (i = 0, s = 0; i < 10; i++) // �ȴ�USB�豸��λ����������,100mS��ʱ
	{
		EnableRootHubPort(port);

		mDelaymS(1);
	}

#else
	for (i = 0, s = 0; i < 100; i++) // �ȴ�USB�豸��λ����������,100mS��ʱ
	{
		mDelaymS(1);
		if (EnableRootHubPort(port) == ERR_SUCCESS) // ʹ��ROOT-HUB�˿�
		{
			i = 0;
			s++; // ��ʱ�ȴ�USB�豸���Ӻ��ȶ�
			if (s > 10 * retry)
			{
				break; // �Ѿ��ȶ�����15mS
			}
		}
	}

	TRACE1("i:%d\r\n", (UINT16)i);

	if (i) // ��λ���豸û������
	{
		DisableRootHubPort(port);
		TRACE1("Disable root hub %1d# port because of disconnect\n", (UINT16)port);

		return (ERR_USB_DISCON);
	}
#endif

	mDelaymS(100);

	SelectHubPort(port, EXHUB_PORT_NONE);

	addr = AssignUniqueAddress(port, EXHUB_PORT_NONE);
	if (EnumerateHubPort(&RootHubPort[port], addr))
	{
		RootHubPort[port].HubPortStatus = PORT_DEVICE_ENUM_SUCCESS;

		TRACE("EnumerateHubPort success\r\n");

		//SelectHubPort(port, EXHUB_PORT_NONE);
		if (RootHubPort[port].UsbDevice.DeviceClass == USB_DEV_CLASS_HUB)
		{
			//hub
			USB_HUB_DESCR *pHubDescr;

			UINT8 hubPortNum;
			UINT16 hubPortStatus, hubPortChange;
			USB_DEVICE *pUsbDevice = &RootHubPort[port].UsbDevice;

			//hub
			s = GetHubDescriptor(pUsbDevice, ReceiveDataBuffer, sizeof(USB_HUB_DESCR), &len);
			if (s != ERR_SUCCESS)
			{
				DisableRootHubPort(port);

				RootHubPort[port].HubPortStatus = PORT_DEVICE_ENUM_FAILED;

				return (FALSE);
			}

			TRACE("GetHubDescriptor OK\r\n");
			TRACE1("len=%d\r\n", len);

			pHubDescr = (USB_HUB_DESCR *)ReceiveDataBuffer;
			hubPortNum = pHubDescr->bNbrPorts;

			TRACE1("hubPortNum=%bd\r\n", hubPortNum);

			if (hubPortNum > MAX_EXHUB_PORT_NUM)
			{
				hubPortNum = MAX_EXHUB_PORT_NUM;
			}

			pUsbDevice->HubPortNum = hubPortNum;

			//supply power for each port
			for (i = 0; i < hubPortNum; i++)
			{
				s = SetHubPortFeature(pUsbDevice, i + 1, HUB_PORT_POWER);
				if (s != ERR_SUCCESS)
				{
					TRACE1("SetHubPortFeature %d failed\r\n", (UINT16)i);

					SubHubPort[port][i].HubPortStatus = PORT_DEVICE_ENUM_FAILED;

					continue;
				}

				TRACE("SetHubPortFeature OK\r\n");
			}

			/*
			for (i = 0; i < hubPortNum; i++)
            {
                s = ClearHubPortFeature(pUsbDevice, i + 1, HUB_C_PORT_CONNECTION );
                if ( s != ERR_SUCCESS )
                {
                    TRACE1("ClearHubPortFeature %d failed\r\n", (UINT16)i);

                    continue;
                }

                TRACE("ClearHubPortFeature OK\r\n");
            }
*/
			for (i = 0; i < hubPortNum; i++)
			{
				mDelaymS(50);

				SelectHubPort(port, EXHUB_PORT_NONE); //�л���hub��ַ

				s = GetHubPortStatus(pUsbDevice, i + 1, &hubPortStatus, &hubPortChange);
				if (s != ERR_SUCCESS)
				{
					SubHubPort[port][i].HubPortStatus = PORT_DEVICE_ENUM_FAILED;

					TRACE1("GetHubPortStatus port:%d failed\r\n", (UINT16)(i + 1));

					return FALSE;
				}

				TRACE2("hubPortStatus:0x%02X,hubPortChange:0x%02X\r\n", hubPortStatus, hubPortChange);

				if ((hubPortStatus & 0x0001) && (hubPortChange & 0x0001))
				{
					//device attached
					TRACE1("hubPort=%d\r\n", (UINT16)i);

					TRACE("device attached\r\n");

					s = ClearHubPortFeature(pUsbDevice, i + 1, HUB_C_PORT_CONNECTION);
					if (s != ERR_SUCCESS)
					{
						SubHubPort[port][i].HubPortStatus = PORT_DEVICE_ENUM_FAILED;
						TRACE("ClearHubPortFeature failed\r\n");

						return FALSE;
					}

					TRACE("ClearHubPortFeature OK\r\n");

					s = SetHubPortFeature(pUsbDevice, i + 1, HUB_PORT_RESET); //reset the port device
					if (s != ERR_SUCCESS)
					{
						SubHubPort[port][i].HubPortStatus = PORT_DEVICE_ENUM_FAILED;

						TRACE1("SetHubPortFeature port:%d failed\r\n", (UINT16)(i + 1));

						return FALSE;
					}

					mDelaymS(100);
					do
					{
						s = GetHubPortStatus(pUsbDevice, i + 1, &hubPortStatus, &hubPortChange);
						if (s != ERR_SUCCESS)
						{
							SubHubPort[port][i].HubPortStatus = PORT_DEVICE_ENUM_FAILED;

							TRACE1("GetHubPortStatus port:%d failed\r\n", (UINT16)(i + 1));

							return FALSE;
						}

						mDelaymS(20);
					} while (hubPortStatus & 0x0010);

					if ((hubPortChange & 0x10) == 0x10) //reset over success
					{
						TRACE("reset complete\r\n");

						if (hubPortStatus & 0x0200)
						{
							//speed low
							SubHubPort[port][i].UsbDevice.DeviceSpeed = LOW_SPEED;

							TRACE("low speed device\r\n");
						}
						else
						{
							//full speed device
							SubHubPort[port][i].UsbDevice.DeviceSpeed = FULL_SPEED;

							TRACE("full speed device\r\n");
						}

						s = ClearHubPortFeature(pUsbDevice, i + 1, HUB_PORT_RESET);
						if (s != ERR_SUCCESS)
						{
							TRACE("ClearHubPortFeature failed\r\n");
						}
						else
						{
							TRACE("ClearHubPortFeature OK\r\n");
						}

						s = ClearHubPortFeature(pUsbDevice, i + 1, HUB_PORT_SUSPEND);
						if (s != ERR_SUCCESS)
						{
							SubHubPort[port][i].HubPortStatus = PORT_DEVICE_ENUM_FAILED;
							TRACE("ClearHubPortFeature failed\r\n");

							return FALSE;
						}

						TRACE("ClearHubPortFeature OK\r\n");

						mDelaymS(500);

						SelectHubPort(port, i);

						addr = AssignUniqueAddress(port, i);
						if (EnumerateHubPort(&SubHubPort[port][i], addr))
						{
							TRACE("EnumerateHubPort success\r\n");
							SubHubPort[port][i].HubPortStatus = PORT_DEVICE_ENUM_SUCCESS;
						}
						else
						{
							TRACE("EnumerateHubPort failed\r\n");
							SubHubPort[port][i].HubPortStatus = PORT_DEVICE_ENUM_FAILED;
						}
					}
				}
			}
		}
	}
	else
	{
		DisableRootHubPort(port);

		RootHubPort[port].HubPortStatus = PORT_DEVICE_ENUM_FAILED;
	}

	return TRUE;
}

static UINT8 QueryHubPortAttach(void)
{
	BOOL res = FALSE;

	UINT8 s = ERR_SUCCESS;

	if (UIF_DETECT)
	{
		//plug in or plug out
		UIF_DETECT = 0;

		if (USB_HUB_ST & bUHS_H0_ATTACH)
		{
			//port 0 plug in
			if (RootHubPort[0].HubPortStatus == PORT_DEVICE_NONE
				/*|| (UHUB0_CTRL & bUH_PORT_EN) == 0x00*/)
			{
				DisableRootHubPort(0);

				RootHubPort[0].HubPortStatus = PORT_DEVICE_INSERT;

				s = ERR_USB_CONNECT;

				TRACE("hub 0 dev in\r\n");
			}
		}
		else if (RootHubPort[0].HubPortStatus >= PORT_DEVICE_INSERT)
		{
			DisableRootHubPort(0);

			if (s == ERR_SUCCESS)
			{
				s = ERR_USB_DISCON;
			}

			TRACE("hub 0 dev out\r\n");
		}

		if (USB_HUB_ST & bUHS_H1_ATTACH)
		{
			//port 1 plug in
			if (RootHubPort[1].HubPortStatus == PORT_DEVICE_NONE
				/*|| ( UHUB1_CTRL & bUH_PORT_EN ) == 0x00*/)
			{
				DisableRootHubPort(1);

				RootHubPort[1].HubPortStatus = PORT_DEVICE_INSERT;

				s = ERR_USB_CONNECT;

				TRACE("hub 1 dev in\r\n");
			}
		}
		else if (RootHubPort[1].HubPortStatus >= PORT_DEVICE_INSERT)
		{
			DisableRootHubPort(1);

			if (s == ERR_SUCCESS)
			{
				s = ERR_USB_DISCON;
			}

			TRACE("hub 1 dev out\r\n");
		}
	}

	return s;
}

void regrabinterfaces(USB_HUB_PORT *pUsbHubPort)
{
	UINT8 i, s, c;
	UINT16 len;
	USB_DEVICE *pUsbDevice = &pUsbHubPort->UsbDevice;
	if (pUsbDevice->DeviceClass != USB_DEV_CLASS_HUB)
	{
		for (i = 0; i < pUsbDevice->InterfaceNum; i++)
		{
#ifdef DEBUG
			int j;
#endif
			INTERFACE *pInterface = &pUsbDevice->Interface[i];

			TRACE1("InterfaceClass=0x%02X\r\n", (UINT16)pInterface->InterfaceClass);
			TRACE1("InterfaceProtocol=0x%02X\r\n", (UINT16)pInterface->InterfaceProtocol);

			if (pInterface->InterfaceClass == USB_DEV_CLASS_HID)
			{
				s = SetIdle(pUsbDevice, 0, 0, i);

				if (s != ERR_SUCCESS)
				{
					TRACE("SetIdle failed\r\n");
				}
				else
				{
					TRACE("SetIdle OK\r\n");
				}

				DEBUG_OUT("Interface %x:\n", i);
				DEBUG_OUT("InterfaceProtocol: %x\r\n", pInterface->InterfaceProtocol);

				if (pInterface->InterfaceSubClass == 0x01)
				{
					SetBootProtocol(pUsbDevice, i);
				}

				TRACE1("Report Size:%d\r\n", pInterface->ReportSize);
				s = GetReportDescriptor(pUsbDevice, i, ReceiveDataBuffer, pInterface->ReportSize <= sizeof(ReceiveDataBuffer) ? pInterface->ReportSize : sizeof(ReceiveDataBuffer), &len);

				if (s != ERR_SUCCESS)
				{
					return; // FALSE;
				}

				TRACE("GetReportDescriptor OK\r\n");
				TRACE1("get report descr len:%d\r\n", len);

#ifdef DEBUG
				{
					UINT16 k;
					for (k = 0; k < len; k++)
					{
						TRACE1("0x%02X ", (UINT16)ReceiveDataBuffer[k]);
					}
					TRACE("\r\n");
				}
#endif

				if (DumpReport)
				{
					SendKeyboardString("\n\nInterface %hx Report Descriptor - \n", i);
					for (c = 0; c < len; c++)
					{
						if (!(c & 0x000F))
							SendKeyboardString("\n");

						SendKeyboardString("%02X ", ReceiveDataBuffer[c]);
					}
					SendKeyboardString("\n");
					SendKeyboardString("\n");
				}

				// use default boot mode descriptors if a keyboard or mouse is detected and the option is enabled in menu
				// Mouse first
				if (!HMSettings.MouseReportMode && pInterface->InterfaceProtocol == HID_PROTOCOL_MOUSE && pInterface->InterfaceSubClass == 0x01)
					ParseReportDescriptor(StandardMouseDescriptor, 50, &pInterface->HidSegStruct, 0);
				// keyboard next
				else if (!HMSettings.KeyboardReportMode && pInterface->InterfaceProtocol == HID_PROTOCOL_KEYBOARD && pInterface->InterfaceSubClass == 0x01)
					ParseReportDescriptor(StandardKeyboardDescriptor, 63, &pInterface->HidSegStruct, 0);
				else
					ParseReportDescriptor(ReceiveDataBuffer, len, &pInterface->HidSegStruct, 0);

				HID_REPORT_DESC *bleh = &pInterface->HidSegStruct;
				HID_SEG *tmpseg;
				for (uint8_t x = 0; x < MAX_REPORTS; x++)
				{
					if (bleh->reports[x] != NULL)
					{
						tmpseg = bleh->reports[x]->firstHidSeg;

						DEBUG_OUT("Report %x, usage %x, length %u: \n", x, bleh->reports[x]->appUsage, bleh->reports[x]->length);
						while (tmpseg != NULL)
						{
							DEBUG_OUT("  startbit %u, it %hx, ip %x, chan %hx, cont %hx, size %hx, count %hx\n", tmpseg->startBit, tmpseg->InputType, tmpseg->InputParam, tmpseg->OutputChannel, tmpseg->OutputControl, tmpseg->reportSize, tmpseg->reportCount);
							tmpseg = tmpseg->next;
						}
					}
				}

				if (pInterface->InterfaceProtocol == HID_PROTOCOL_KEYBOARD)
				{
					UINT8 led;

					led = GetKeyboardLedStatus();
					SetReport(pUsbDevice, i, &led, sizeof(led));
					TRACE("SetReport\r\n");
				}
			}
		}
	}
}

void RegrabDeviceReports(UINT8 port)
{
	USB_HUB_PORT *pUsbHubPort = &RootHubPort[port];

	if (pUsbHubPort->HubPortStatus == PORT_DEVICE_ENUM_SUCCESS)
	{
		if (pUsbHubPort->UsbDevice.DeviceClass != USB_DEV_CLASS_HUB)
		{
			SelectHubPort(port, EXHUB_PORT_NONE);
			regrabinterfaces(pUsbHubPort);
		}
		else
		{
			UINT8 exHubPortNum = pUsbHubPort->UsbDevice.HubPortNum;
			UINT8 i;

			for (i = 0; i < exHubPortNum; i++)
			{
				pUsbHubPort = &SubHubPort[port][i];

				if (pUsbHubPort->HubPortStatus == PORT_DEVICE_ENUM_SUCCESS && pUsbHubPort->UsbDevice.DeviceClass != USB_DEV_CLASS_HUB)
				{
					SelectHubPort(port, i);
					regrabinterfaces(pUsbHubPort);
				}
			}
		}
	}
}

//----------------------------------------------------------------------------------
void DealUsbPort(void) //main function should use it at least 500ms
{
	UINT8 s = QueryHubPortAttach();
	if (s == ERR_USB_CONNECT)
	{
		UINT8 i;

		mDelaymS(150);
		TR0 = 0;
		andyclearmem();
		sInterfacePoolPos = 0;
		for (i = 0; i < ROOT_HUB_PORT_NUM; i++)
		{
			EnumerateRootHubPort(i);
			RegrabDeviceReports(i);
		}
		TR0 = 1;
	}
}

void InterruptProcessRootHubPort(UINT8 port)
{
	USB_HUB_PORT *pUsbHubPort = &RootHubPort[port];

	if (pUsbHubPort->HubPortStatus == PORT_DEVICE_ENUM_SUCCESS)
	{
		if (pUsbHubPort->UsbDevice.DeviceClass != USB_DEV_CLASS_HUB)
		{
			SelectHubPort(port, EXHUB_PORT_NONE);

			HIDDataTransferReceive(&pUsbHubPort->UsbDevice);
		}
		else
		{
			UINT8 exHubPortNum = pUsbHubPort->UsbDevice.HubPortNum;
			UINT8 i;

			for (i = 0; i < exHubPortNum; i++)
			{
				pUsbHubPort = &SubHubPort[port][i];

				if (pUsbHubPort->HubPortStatus == PORT_DEVICE_ENUM_SUCCESS && pUsbHubPort->UsbDevice.DeviceClass != USB_DEV_CLASS_HUB)
				{
					SelectHubPort(port, i);

					HIDDataTransferReceive(&pUsbHubPort->UsbDevice);
				}
			}
		}
	}
}

static void UpdateUsbKeyboardLedInternal(USB_DEVICE *pUsbDevice, UINT8 led)
{
	UINT8 i;
	for (i = 0; i < pUsbDevice->InterfaceNum; i++)
	{
		INTERFACE *pInterface = &pUsbDevice->Interface[i];
		if (pInterface->InterfaceClass == USB_DEV_CLASS_HID)
		{
			if (pInterface->InterfaceProtocol == HID_PROTOCOL_KEYBOARD)
			{
				SetReport(pUsbDevice, i, &led, 1);

				TRACE1("led=0x%x\r\n", led);
			}
		}
	}
}

//-----------------------------------------------------------------------------
void UpdateUsbKeyboardLed(UINT8 led)
{
	UINT8 i, j;

	for (i = 0; i < ROOT_HUB_PORT_NUM; i++)
	{
		USB_HUB_PORT *pUsbHubPort = &RootHubPort[i];
		if (pUsbHubPort->HubPortStatus == PORT_DEVICE_ENUM_SUCCESS)
		{
			if (pUsbHubPort->UsbDevice.DeviceClass != USB_DEV_CLASS_HUB)
			{
				SelectHubPort(i, EXHUB_PORT_NONE);

				UpdateUsbKeyboardLedInternal(&pUsbHubPort->UsbDevice, led);
			}
			else
			{
				int exHubPortNum = pUsbHubPort->UsbDevice.HubPortNum;

				for (j = 0; j < exHubPortNum; j++)
				{
					pUsbHubPort = &SubHubPort[i][j];

					if (pUsbHubPort->HubPortStatus == PORT_DEVICE_ENUM_SUCCESS && pUsbHubPort->UsbDevice.DeviceClass != USB_DEV_CLASS_HUB)
					{
						SelectHubPort(i, j);

						UpdateUsbKeyboardLedInternal(&pUsbHubPort->UsbDevice, led);
					}
				}
			}
		}
	}
}

/*void DumpyTown()
{
	HID_REPORT_DESC *bleh;
	HID_SEG *tmpseg;
	DEBUG_OUT("\033[2J\033[H");
	for (uint8_t y = 0; y < MAX_HID_DEVICES; y++)
	{

		if (HIDdevice[y].connected)
		{
			DEBUG_OUT("---- DEVICE %d ----\n", y);
			bleh = &(HIDdevice[y].HidSegStruct);
			for (uint8_t x = 0; x < MAX_REPORTS; x++)
			{
				if (bleh->reports[x] != NULL)
				{
					tmpseg = bleh->reports[x]->firstHidSeg;

					DEBUG_OUT("Report %x, usage %x, length %u: \n", x, bleh->reports[x]->appUsage, bleh->reports[x]->length);
					while (tmpseg != NULL)
					{
						DEBUG_OUT("  startbit %u, it %hx, ip %x, chan %hx, cont %hx, size %hx\n", tmpseg->startBit, tmpseg->InputType, tmpseg->InputParam, tmpseg->OutputChannel, tmpseg->OutputControl, tmpseg->reportSize);
						tmpseg = tmpseg->next;
					}
				}
			}
		}
	}
}*/