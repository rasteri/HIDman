#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#include "preset.h"

#define RECEIVE_BUFFER_LEN 512
UINT8X ReceiveDataBuffer[RECEIVE_BUFFER_LEN];

void InitInterface(INTERFACE* Interface)
{
	memset(Interface, 0, sizeof(INTERFACE));

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

	Interface->usesReports = 0;
}


void FillSetupReq(USB_SETUP_REQ *pSetupReq, UINT8 type, UINT8 req, UINT16 value, UINT16 index, UINT16 length)
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
UINT8 GetDeviceDescr(USB_HUB_PORT *pUsbDevice, UINT8 *pDevDescr, UINT16 reqLen, UINT16 *pRetLen) //get device describtion
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
UINT8 GetConfigDescr(USB_HUB_PORT *pUsbDevice, UINT8 *pCfgDescr, UINT16 reqLen, UINT16 *pRetLen)
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
UINT8 SetUsbAddress(USB_HUB_PORT *pUsbDevice, UINT8 addr)
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
UINT8 SetUsbConfig(USB_HUB_PORT *pUsbDevice, UINT8 cfg)
{
	UINT8 s;

	USB_SETUP_REQ SetupReq;
	FillSetupReq(&SetupReq, USB_REQ_TYP_OUT | USB_REQ_TYP_STANDARD | USB_REQ_RECIP_DEVICE,
				 USB_SET_CONFIGURATION, cfg, 0, 0);

	s = HostCtrlTransfer(&SetupReq, pUsbDevice->MaxPacketSize0, NULL, NULL);

	return s;
}

//-----------------------------------------------------------------------------------------
UINT8 GetHubDescriptor(USB_HUB_PORT *pUsbDevice, UINT8 *pHubDescr, UINT16 reqLen, UINT16 *pRetLen)
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
UINT8 GetHubPortStatus(USB_HUB_PORT *pUsbDevice, UINT8 HubPort, UINT16 *pPortStatus, UINT16 *pPortChange)
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
UINT8 SetHubPortFeature(USB_HUB_PORT *pUsbDevice, UINT8 HubPort, UINT8 selector) //this function set feature for port						//this funciton set
{
	UINT8 s;

	USB_SETUP_REQ SetupReq;
	FillSetupReq(&SetupReq, USB_REQ_TYP_OUT | USB_REQ_TYP_CLASS | USB_REQ_RECIP_OTHER,
				 USB_SET_FEATURE, selector, (0 << 8) | HubPort, 0);

	s = HostCtrlTransfer(&SetupReq, pUsbDevice->MaxPacketSize0, NULL, NULL);

	return s;
}

UINT8 ClearHubPortFeature(USB_HUB_PORT *pUsbDevice, UINT8 HubPort, UINT8 selector)
{
	UINT8 s;

	USB_SETUP_REQ SetupReq;
	FillSetupReq(&SetupReq, USB_REQ_TYP_OUT | USB_REQ_TYP_CLASS | USB_REQ_RECIP_OTHER,
				 USB_CLEAR_FEATURE, selector, (0 << 8) | HubPort, 0);

	s = HostCtrlTransfer(&SetupReq, pUsbDevice->MaxPacketSize0, NULL, NULL);

	return s;
}

//-----------------------------------------------------------------------------------------
UINT8 GetReportDescriptor(USB_HUB_PORT *pUsbDevice, UINT8 interface, UINT8 *pReportDescr, UINT16 reqLen, UINT16 *pRetLen)
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

UINT8 SetBootProtocol(USB_HUB_PORT *pUsbDevice, UINT8 interface)
{
	UINT8 s;

	USB_SETUP_REQ SetupReq;

	FillSetupReq(&SetupReq, 0b00100001, HID_SET_PROTOCOL, 0, interface, 0);

	s = HostCtrlTransfer(&SetupReq, pUsbDevice->MaxPacketSize0, NULL, NULL);
	return s;
}

UINT8 GetBootProtocol(USB_HUB_PORT *pUsbDevice, UINT8 interface)
{
	UINT8 s;
	UINT8 ret;

	USB_SETUP_REQ SetupReq;

	FillSetupReq(&SetupReq, 0b10100001, HID_GET_PROTOCOL, 0, interface, 1);

	s = HostCtrlTransfer(&SetupReq, pUsbDevice->MaxPacketSize0, &ret, (PUINT16)1);

	return ret;
}

//-----------------------------------------------------------------------------------------
UINT8 SetIdle(USB_HUB_PORT *pUsbDevice, UINT16 durationMs, UINT8 reportID, UINT8 interface)
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
UINT8 SetReport(USB_HUB_PORT *pUsbDevice, UINT8 interface, UINT8 *pReport, UINT16 ReportLen)
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

//-------------------------------------------------------------------------------------------
UINT8 HIDDataTransferReceive(USB_HUB_PORT *pUsbDevice)
{
	UINT8 s = 0, p;
	int i, j;
	int interfaceNum;
	int endpointNum;

	UINT16 len;
	interfaceNum = pUsbDevice->InterfaceNum;
	for (i = 0; i < interfaceNum; i++)
	{
		//INTERFACE *pInterface = &pUsbDevice->Interface[i];
		INTERFACE *pInterface = (INTERFACE *)ListGetData(pUsbDevice->Interfaces, i);
		if (pInterface->InterfaceClass == USB_DEV_CLASS_HID)
		{
			endpointNum = pInterface->EndpointNum;
			for (j = 0; j < endpointNum; j++)
			{
				ENDPOINT *pEndPoint = &pInterface->Endpoint[j];
				if (pEndPoint->EndpointDir == ENDPOINT_IN)
				{
					s = TransferReceive(pEndPoint, ReceiveDataBuffer, &len, 0);
					if (s == ERR_SUCCESS)
					{
						//TRACE1("interface %d data:", (UINT16)i);
						// HIS IS WHERE THE FUN STUFF GOES
						//ProcessHIDData(pInterface, ReceiveDataBuffer, len);
						ParseReport(pInterface, len * 8, ReceiveDataBuffer);
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
BOOL EnumerateHubPort(USB_HUB_PORT *pUsbHubPort, UINT8 addr)
{
	UINT8 s;
	UINT16 len;
	UINT16 cfgDescLen;

	USB_HUB_PORT *pUsbDevice;
	USB_CFG_DESCR *pCfgDescr;

	pUsbDevice = pUsbHubPort;

	//get first 8 bytes of device descriptor to get maxpacketsize0
	s = GetDeviceDescr(pUsbDevice, ReceiveDataBuffer, 8, &len);

	if (s != ERR_SUCCESS)
	{
		TRACE1("GetDeviceDescr failed,s:0x%02X\r\n", (UINT16)s);
		if (DumpReport) SendKeyboardString("gdd.fail\n");
		return (FALSE);
	}
	TRACE("GetDeviceDescr OK\r\n");
	TRACE1("len=%d\r\n", len);

	if (DumpReport) SendKeyboardString("gdd ok %d\n", len);

	ParseDeviceDescriptor((USB_DEV_DESCR *)ReceiveDataBuffer, len, pUsbDevice);

	TRACE1("MaxPacketSize0=%bd\r\n", pUsbDevice->MaxPacketSize0);
	if (DumpReport) SendKeyboardString("mps %d\n", pUsbDevice->MaxPacketSize0);

	//set device address
	s = SetUsbAddress(pUsbDevice, addr);
	if (s != ERR_SUCCESS)
	{
		if (DumpReport) SendKeyboardString("addr fail\n");
		return (FALSE);
	}

	pUsbDevice->DeviceAddress = addr;
	TRACE("SetUsbAddress OK\r\n");
	TRACE1("address=%bd\r\n", pUsbDevice->DeviceAddress);

	if (DumpReport) SendKeyboardString("addr ok %d\n", pUsbDevice->DeviceAddress);

	//get full bytes of device descriptor
	s = GetDeviceDescr(pUsbDevice, ReceiveDataBuffer, sizeof(USB_DEV_DESCR), &len);

	if (s != ERR_SUCCESS)
	{
		if (DumpReport) SendKeyboardString("gddfull fail\n");
		TRACE("GetDeviceDescr failed\r\n");

		return (FALSE);
	}

	TRACE("GetDeviceDescr OK\r\n");
	TRACE1("len=%d\r\n", len);

	if (DumpReport) SendKeyboardString("gddfull ok %d\n", len);

	ParseDeviceDescriptor((USB_DEV_DESCR *)ReceiveDataBuffer, len, pUsbDevice);
	TRACE3("VendorID=0x%04X,ProductID=0x%04X,bcdDevice=0x%04X\r\n", pUsbDevice->VendorID, pUsbDevice->ProductID, pUsbDevice->bcdDevice);

	if (DumpReport) SendKeyboardString("0x%04X 0x%04X 0x%04X\n", pUsbDevice->VendorID, pUsbDevice->ProductID, pUsbDevice->bcdDevice);

	//get configure descriptor for the first time
	cfgDescLen = sizeof(USB_CFG_DESCR);
	TRACE1("GetConfigDescr with cfgDescLen=%d\r\n", cfgDescLen);
	s = GetConfigDescr(pUsbDevice, ReceiveDataBuffer, cfgDescLen, &len);
	if (s != ERR_SUCCESS)
	{
		TRACE("GetConfigDescr 1 failed\r\n");
		if (DumpReport) SendKeyboardString("gcd1 fail\n");
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
		if (DumpReport) SendKeyboardString("gcd 2 fail\n");
		return (FALSE);
	}

	//parse config descriptor
	ParseConfigDescriptor((USB_CFG_DESCR *)ReceiveDataBuffer, len, pUsbDevice);

	TRACE("GetConfigDescr OK\r\n");
	TRACE1("len=%d\r\n", len);

	if (DumpReport) SendKeyboardString("gcd ok %d\n", len);

	//set config
	s = SetUsbConfig(pUsbDevice, ((USB_CFG_DESCR *)ReceiveDataBuffer)->bConfigurationValue);
	if (s != ERR_SUCCESS)
	{
		if (DumpReport) SendKeyboardString("suc fail\n");
		return (FALSE);
	}

	TRACE("SetUsbConfig OK\r\n");
	TRACE1("configure=%bd\r\n", ((USB_CFG_DESCR *)ReceiveDataBuffer)->bConfigurationValue);

	TRACE1("pUsbDevice->InterfaceNum=%d\r\n", (UINT16)pUsbDevice->InterfaceNum);

	return (TRUE);
}

UINT8 AssignUniqueAddress(UINT8 RootHubIndex, UINT8 HubPortIndex)
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

BOOL EnumerateRootHubPort(UINT8 port)
{
	UINT8 i, s;
	UINT16 len;

	UINT8 retry = 0;

	UINT8 addr;

	/*if (RootHubPort[port].HubPortStatus != PORT_DEVICE_INSERT)
	{
		return FALSE;
	}*/

	TRACE1("enumerate port:%bd\r\n", port);

//	DisableRootHubPort(port);

//	mDelaymS(500);

	ResetRootHubPort(port);
//	mDelaymS(500);

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
		if (RootHubPort[port].DeviceClass == USB_DEV_CLASS_HUB)
		{
			if (DumpReport) SendKeyboardString("\nFound hub\n");

			//hub
			USB_HUB_DESCR *pHubDescr;

			UINT8 hubPortNum;
			UINT16 hubPortStatus, hubPortChange;
			USB_HUB_PORT *pUsbDevice = &RootHubPort[port];

			//hub
			s = GetHubDescriptor(pUsbDevice, ReceiveDataBuffer, sizeof(USB_HUB_DESCR), &len);
			if (s != ERR_SUCCESS)
			{
				DisableRootHubPort(port);

				RootHubPort[port].HubPortStatus = PORT_DEVICE_ENUM_FAILED;
				if (DumpReport) SendKeyboardString("hub desc err\n");
				return (FALSE);
			}

			TRACE("GetHubDescriptor OK\r\n");
			TRACE1("len=%d\r\n", len);

			pHubDescr = (USB_HUB_DESCR *)ReceiveDataBuffer;
			hubPortNum = pHubDescr->bNbrPorts;

			TRACE1("hubPortNum=%bd\r\n", hubPortNum);
			if (DumpReport) SendKeyboardString("%d port hub\n", hubPortNum);

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
			INTERFACE *pInterface = (INTERFACE *)ListGetData(pUsbDevice->Interfaces, 0);

			ENDPOINT *pEndPoint = &pInterface->Endpoint[0];

			printf("Doing new thing - %x\n", pInterface->Endpoint[0].EndpointAddr);


			s = TransferReceive(pEndPoint, ReceiveDataBuffer, &len, 20000);

			if (s != ERR_SUCCESS){
				if (DumpReport) SendKeyboardString("new enum. failed\n");
				return FALSE;
			}

			uint8_t changebitmap = ReceiveDataBuffer[0];

			if (DumpReport) SendKeyboardString("new enum. %x\n", changebitmap);

			for (i = 0; i < hubPortNum; i++)
			{
				if (changebitmap & (1 << (i+1))) {
					if (DumpReport) SendKeyboardString("checkn port %d\n", i);
					mDelaymS(50);

					SelectHubPort(port, EXHUB_PORT_NONE); //�л���hub��ַ

					s = GetHubPortStatus(pUsbDevice, i + 1, &hubPortStatus, &hubPortChange);
					if (s != ERR_SUCCESS)
					{
						SubHubPort[port][i].HubPortStatus = PORT_DEVICE_ENUM_FAILED;

						TRACE1("GetHubPortStatus port:%d failed\r\n", (UINT16)(i + 1));

						return FALSE;
					}

					if (DumpReport) SendKeyboardString("ps- 0x%02X pc- 0x%02X\n", hubPortStatus, hubPortChange);

					TRACE2("hubPortStatus:0x%02X,hubPortChange:0x%02X\r\n", hubPortStatus, hubPortChange);

					if ((hubPortStatus & 0x0001) && (hubPortChange & 0x0001))
					{
						//device attached
						TRACE1("hubPort=%d\r\n", (UINT16)i);

						TRACE("device attached\r\n");

						if (DumpReport) SendKeyboardString("port %d attached\n", i);

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
								SubHubPort[port][i].DeviceSpeed = LOW_SPEED;
								if (DumpReport) SendKeyboardString("lowspeed\n");
								TRACE("low speed device\r\n");
							}
							else
							{
								//full speed device
								SubHubPort[port][i].DeviceSpeed = FULL_SPEED;
								if (DumpReport) SendKeyboardString("fullspeed\n");
								TRACE("full speed device\r\n");
							}

							s = ClearHubPortFeature(pUsbDevice, i + 1, HUB_C_PORT_RESET);
							if (s != ERR_SUCCESS)
							{
								TRACE("ClearHubPortFeature failed\r\n");
							}
							else
							{
								TRACE("ClearHubPortFeature OK\r\n");
							}

							/*s = ClearHubPortFeature(pUsbDevice, i + 1, HUB_PORT_SUSPEND);
							if (s != ERR_SUCCESS)
							{
								SubHubPort[port][i].HubPortStatus = PORT_DEVICE_ENUM_FAILED;
								TRACE("ClearHubPortFeature failed\r\n");

								return FALSE;
							}

							TRACE("ClearHubPortFeature OK\r\n");

							mDelaymS(500);*/

							SelectHubPort(port, i);

							addr = AssignUniqueAddress(port, i);
							if (EnumerateHubPort(&SubHubPort[port][i], addr))
							{
								if (DumpReport) SendKeyboardString("enum.OK\n");
								TRACE("EnumerateHubPort success\r\n");
								SubHubPort[port][i].HubPortStatus = PORT_DEVICE_ENUM_SUCCESS;
							}
							else
							{
								if (DumpReport) SendKeyboardString("enum.fail\n");
								TRACE("EnumerateHubPort failed\r\n");
								SubHubPort[port][i].HubPortStatus = PORT_DEVICE_ENUM_FAILED;
							}
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

UINT8 QueryHubPortAttach(void)
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
	UINT16 len, cnt;
	USB_HUB_PORT *pUsbDevice = pUsbHubPort;
	if (pUsbDevice->DeviceClass != USB_DEV_CLASS_HUB)
	{
		for (i = 0; i < pUsbDevice->InterfaceNum; i++)
		{
#ifdef DEBUG
			int j;
#endif
			//INTERFACE *pInterface = &pUsbDevice->Interface[i];		
			INTERFACE *pInterface = (INTERFACE *)ListGetData(pUsbDevice->Interfaces, i);

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
					for (cnt = 0; cnt < len; cnt++)
					{
						if (!(cnt & 0x000F))
							SendKeyboardString("\n");

						SendKeyboardString("%02X ", ReceiveDataBuffer[cnt]);
					}
					SendKeyboardString("\n");
					SendKeyboardString("\n");
				}

				// use default boot mode descriptors if a keyboard or mouse is detected and "advanced USB" is disabled in menu
				if (
					(!HMSettings.MouseReportMode && pInterface->InterfaceProtocol == HID_PROTOCOL_MOUSE) ||
					(!HMSettings.KeyboardReportMode && pInterface->InterfaceProtocol == HID_PROTOCOL_KEYBOARD)
					)
				{
					// if it supports boot mode, enable that and use the default descriptor
					if (pInterface->InterfaceSubClass == 0x01) {
						SetBootProtocol(pUsbDevice, i);
						ParseReportDescriptor(
							pInterface->InterfaceProtocol == HID_PROTOCOL_MOUSE ? StandardMouseDescriptor : StandardKeyboardDescriptor, 
							pInterface->InterfaceProtocol == HID_PROTOCOL_MOUSE ? 50 : 63, 
							pInterface
						);
					}
					// Otherwise don't attempt to use this device at all unless advanced USB is enabled
					else continue;
				}
				else
					ParseReportDescriptor(ReceiveDataBuffer, len, pInterface);

				static HID_SEG * __xdata tmpseg;
				/*for (uint8_t x = 0; x < MAX_REPORTS; x++)
				{
					static HID_REPORT * __xdata prnreport;
					prnreport = ListGetData(pInterface->Reports, x);
					if (prnreport != NULL)
					{
						tmpseg = prnreport->firstHidSeg;

						DEBUG_OUT("Report %x, usage %x, length %u: \n", x, prnreport->appUsage, prnreport->length);
						while (tmpseg != NULL)
						{
							DEBUG_OUT("  startbit %u, it %hx, ip %x, chan %hx, cont %hx, size %hx, count %hx\n", tmpseg->startBit, tmpseg->InputType, tmpseg->InputParam, tmpseg->OutputChannel, tmpseg->OutputControl, tmpseg->reportSize, tmpseg->reportCount);
							tmpseg = tmpseg->next;
						}
					}
				}*/

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
		if (pUsbHubPort->DeviceClass != USB_DEV_CLASS_HUB)
		{
			SelectHubPort(port, EXHUB_PORT_NONE);
			regrabinterfaces(pUsbHubPort);
		}
		else
		{
			UINT8 exHubPortNum = pUsbHubPort->HubPortNum;
			UINT8 i;

			for (i = 0; i < exHubPortNum; i++)
			{
				pUsbHubPort = &SubHubPort[port][i];

				if (pUsbHubPort->HubPortStatus == PORT_DEVICE_ENUM_SUCCESS && pUsbHubPort->DeviceClass != USB_DEV_CLASS_HUB)
				{
					SelectHubPort(port, i);
					regrabinterfaces(pUsbHubPort);
				}
			}
		}
	}
}

void ReenumerateAllPorts(void){
	UINT8 i;
	
	if (!DumpReport)
		OutputsEnabled = 0;

	if (DumpReport) SendKeyboardString("reenumerating all ports\n");
	mDelaymS(150);

	InitUsbData();
	andyclearmem();
	InitPresets();
	for (i = 0; i < ROOT_HUB_PORT_NUM; i++)
	{
		if (DumpReport) SendKeyboardString("port %d\n", i);

		EnumerateRootHubPort(i);
		RegrabDeviceReports(i);
	}
	OutputsEnabled = 1;
}

//----------------------------------------------------------------------------------
void DealUsbPort(void) //main function should use it at least 500ms
{
	UINT8 s = QueryHubPortAttach();

	if (s == ERR_USB_CONNECT)
	{
		ReenumerateAllPorts();
	}
}

void InterruptProcessRootHubPort(UINT8 port)
{
	USB_HUB_PORT *pUsbHubPort = &RootHubPort[port];

	if (pUsbHubPort->HubPortStatus == PORT_DEVICE_ENUM_SUCCESS)
	{
		if (pUsbHubPort->DeviceClass != USB_DEV_CLASS_HUB)
		{
			SelectHubPort(port, EXHUB_PORT_NONE);

			HIDDataTransferReceive(pUsbHubPort);
		}
		else
		{
			UINT8 exHubPortNum = pUsbHubPort->HubPortNum;
			UINT8 i;

			for (i = 0; i < exHubPortNum; i++)
			{
				pUsbHubPort = &SubHubPort[port][i];

				if (pUsbHubPort->HubPortStatus == PORT_DEVICE_ENUM_SUCCESS && pUsbHubPort->DeviceClass != USB_DEV_CLASS_HUB)
				{
					SelectHubPort(port, i);
					HIDDataTransferReceive(pUsbHubPort);
				}
			}
		}
	}
}

void UpdateUsbKeyboardLedInternal(USB_HUB_PORT *pUsbDevice, UINT8 led)
{
	UINT8 i;
	for (i = 0; i < pUsbDevice->InterfaceNum; i++)
	{
		//INTERFACE *pInterface = &pUsbDevice->Interface[i];
		INTERFACE *pInterface = (INTERFACE *)ListGetData(pUsbDevice->Interfaces, i);
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
			if (pUsbHubPort->DeviceClass != USB_DEV_CLASS_HUB)
			{
				SelectHubPort(i, EXHUB_PORT_NONE);

				UpdateUsbKeyboardLedInternal(pUsbHubPort, led);
			}
			else
			{
				int exHubPortNum = pUsbHubPort->HubPortNum;

				for (j = 0; j < exHubPortNum; j++)
				{
					pUsbHubPort = &SubHubPort[i][j];

					if (pUsbHubPort->HubPortStatus == PORT_DEVICE_ENUM_SUCCESS && pUsbHubPort->DeviceClass != USB_DEV_CLASS_HUB)
					{
						SelectHubPort(i, j);

						UpdateUsbKeyboardLedInternal(pUsbHubPort, led);
					}
				}
			}
		}
	}
}

BOOL volatile s_CheckUsbPort0 = FALSE;
BOOL volatile s_CheckUsbPort1 = FALSE;

void ProcessUsbHostPort(void)
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
