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
#include "linkedlist.h"
// enumeration done in stages

/*

1. EnumerateRootHubPort does both root hubs
	EnumerateHubPort for each port
		This Fills "RootHubPort" Array, setting SUCCESS to each port if it has device attached
		And assigns addresses
		And parses/fills interface descriptors
	Then another EnumerateHubPort for each non-root hub
		This fills the "SubHubPort" array, setting SUCCESS to each port if it has device attached
		also assigning addresses
		And parses/fills interface descriptors
	
2. RegrabDeviceReports traverses these arrays and parses the report descriptors

3. InterruptProcessRootHubPort(uint8 rootport) then actually polls the devices (per root port)
	For each entry in the RootHubPort and SubHubPort :
		SelectHubPort selects the root port and address
		HIDDataTransferReceive actually queries the device
	 

hubs are essentially transparent once you get all the addresses assigned
So enumeratehubport needs to be made recursive, but apart from that no real need to store hub tree structure

However, to correctly detect changes in hub port status, we need to keep a list of all the hubs themselves, who get addresses like everything else

So maybe : 

ditch the whole idea of RootHubPort/SubHubPort 
just have a flat list of all devices that need polled, 
maybe the last time they *were* polled (add a global clock)
Then hubs and devices can be polled in one big loop at whatever rate we want
*/


#define RECEIVE_BUFFER_LEN 512
UINT8X ReceiveDataBuffer[RECEIVE_BUFFER_LEN];

__xdata bool DumpReport = 0;

void DumpHex(uint8_t *buffa, uint16_t len)
{
	static __xdata uint16_t cnt;

	for (cnt = 0; cnt < len; cnt++)
	{
		DEBUGOUT("%02X ", buffa[cnt]);
		if ((cnt & 0x000F) == 0x000F)
			DEBUGOUT("\n");
	}

	DEBUGOUT("\n\n");
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
	static __xdata UINT8 s;
	static __xdata UINT16 len;

	static __xdata USB_SETUP_REQ SetupReq;
	FillSetupReq(&SetupReq, USB_REQ_TYP_IN | USB_REQ_TYP_STANDARD | USB_REQ_RECIP_DEVICE,
				 USB_GET_DESCRIPTOR, USB_DESCR_TYP_DEVICE << 8, 0, reqLen);


	s = HostCtrlTransfer(&SetupReq, pUsbDevice == NULL ? DEFAULT_ENDP0_SIZE : pUsbDevice->MaxPacketSize0, pDevDescr, &len);

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
	static __xdata UINT8 s;
	static __xdata UINT16 len;

	static __xdata USB_SETUP_REQ SetupReq;
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
	static __xdata UINT8 s;

	static __xdata USB_SETUP_REQ SetupReq;
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
	static __xdata UINT8 s;

	static __xdata USB_SETUP_REQ SetupReq;
	FillSetupReq(&SetupReq, USB_REQ_TYP_OUT | USB_REQ_TYP_STANDARD | USB_REQ_RECIP_DEVICE,
				 USB_SET_CONFIGURATION, cfg, 0, 0);

	s = HostCtrlTransfer(&SetupReq, pUsbDevice->MaxPacketSize0, NULL, NULL);

	return s;
}

//-----------------------------------------------------------------------------------------
UINT8 GetHubDescriptor(USB_HUB_PORT *pUsbDevice, UINT8 *pHubDescr, UINT16 reqLen, UINT16 *pRetLen)
{
	static __xdata UINT8 s;
	static __xdata UINT16 len;

	static __xdata USB_SETUP_REQ SetupReq;
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
	static __xdata UINT8 s;
	static __xdata UINT16 len;

	static __xdata UINT8 Ret[4];

	static __xdata USB_SETUP_REQ SetupReq;
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
	static __xdata UINT8 s;

	static __xdata USB_SETUP_REQ SetupReq;
	FillSetupReq(&SetupReq, USB_REQ_TYP_OUT | USB_REQ_TYP_CLASS | USB_REQ_RECIP_OTHER,
				 USB_SET_FEATURE, selector, (0 << 8) | HubPort, 0);

	s = HostCtrlTransfer(&SetupReq, pUsbDevice->MaxPacketSize0, NULL, NULL);

	return s;
}

UINT8 ClearHubPortFeature(USB_HUB_PORT *pUsbDevice, UINT8 HubPort, UINT8 selector)
{
	static __xdata UINT8 s;

	static __xdata USB_SETUP_REQ SetupReq;
	FillSetupReq(&SetupReq, USB_REQ_TYP_OUT | USB_REQ_TYP_CLASS | USB_REQ_RECIP_OTHER,
				 USB_CLEAR_FEATURE, selector, (0 << 8) | HubPort, 0);

	s = HostCtrlTransfer(&SetupReq, pUsbDevice->MaxPacketSize0, NULL, NULL);

	return s;
}

//-----------------------------------------------------------------------------------------
UINT8 GetReportDescriptor(USB_HUB_PORT *pUsbDevice, UINT8 interface, UINT8 *pReportDescr, UINT16 reqLen, UINT16 *pRetLen)
{
	static __xdata UINT8 s;
	static __xdata UINT16 len;

	static __xdata USB_SETUP_REQ SetupReq;
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

UINT8 SetBootProtocol(USB_HUB_PORT *pUsbDevice, UINT8 interface, uint16_t val)
{
	static __xdata UINT8 s;

	static __xdata USB_SETUP_REQ SetupReq;

	FillSetupReq(&SetupReq, 0b00100001, HID_SET_PROTOCOL, val, interface, 0);

	s = HostCtrlTransfer(&SetupReq, pUsbDevice->MaxPacketSize0, NULL, NULL);
	return s;
}

UINT8 GetBootProtocol(USB_HUB_PORT *pUsbDevice, UINT8 interface)
{
	static __xdata UINT8 s;
	static __xdata UINT8 ret;

	static __xdata USB_SETUP_REQ SetupReq;

	FillSetupReq(&SetupReq, 0b10100001, HID_GET_PROTOCOL, 0, interface, 1);

	s = HostCtrlTransfer(&SetupReq, pUsbDevice->MaxPacketSize0, &ret, (PUINT16)1);

	return ret;
}

//-----------------------------------------------------------------------------------------
UINT8 SetIdle(USB_HUB_PORT *pUsbDevice, UINT16 durationMs, UINT8 reportID, UINT8 interface)
{
	static __xdata UINT8 s;

	static __xdata USB_SETUP_REQ SetupReq;
	UINT8 duration = (UINT8)(durationMs / 4);

	FillSetupReq(&SetupReq, USB_REQ_TYP_OUT | USB_REQ_TYP_CLASS | USB_REQ_RECIP_INTERF,
				 HID_SET_IDLE, (duration << 8) | reportID, interface, 0);

	s = HostCtrlTransfer(&SetupReq, pUsbDevice->MaxPacketSize0, NULL, NULL);

	return s;
}

//-----------------------------------------------------------------------------------------------
UINT8 SetReport(USB_HUB_PORT *pUsbDevice, UINT8 interface, UINT8 *pReport, UINT16 ReportLen)
{
	static __xdata UINT8 s;
	static __xdata UINT16 len;

	static __xdata USB_SETUP_REQ SetupReq;
	FillSetupReq(&SetupReq, USB_REQ_TYP_OUT | USB_REQ_TYP_CLASS | USB_REQ_RECIP_INTERF,
				 HID_SET_REPORT, HID_REPORT_OUTPUT << 8, interface, ReportLen);

	s = HostCtrlTransfer(&SetupReq, pUsbDevice->MaxPacketSize0, pReport, &len);

	return s;
}

//-----------------------------------------------------------------------------------------------
void InitUsbData(void)
{
	PolledDevices = NULL;
}

//-------------------------------------------------------------------------------------------
UINT8 HIDDataTransferReceive(USB_HUB_PORT *pUsbDevice)
{
	static UINT8 s, p;
	static int i, j;
	static int interfaceNum;
	static int endpointNum;

	static UINT16 len;
				//P0 |= 0b00100000;
	s = 0;
	interfaceNum = pUsbDevice->InterfaceNum;
	for (i = 0; i < interfaceNum; i++)
	{
		//INTERFACE *pInterface = &pUsbDevice->Interface[i];
		__xdata INTERFACE *pInterface = (__xdata INTERFACE *)ListGetData(pUsbDevice->Interfaces, i);
		if (pInterface != NULL && pInterface->InterfaceClass == USB_DEV_CLASS_HID)
		{
			endpointNum = pInterface->EndpointNum;
			for (j = 0; j < endpointNum; j++)
			{
				ENDPOINT *pEndPoint = &pInterface->Endpoint[j];
				if (pEndPoint->EndpointDir == ENDPOINT_IN)
				{
					//P3 ^= 0b10000000;
					s = TransferReceive(pEndPoint, ReceiveDataBuffer, &len, 0);
					
					if (s == ERR_SUCCESS)
					{
						
						//TRACE1("interface %d data:", (UINT16)i);
						// HIS IS WHERE THE FUN STUFF GOES
						//ProcessHIDData(pInterface, ReceiveDataBuffer, len);
						
						ParseReport(pInterface, len * 8, ReceiveDataBuffer);
						

						if (KeyboardDebugOutput || FlashSettings->SerialDebugOutput) {
							DEBUGOUT("I%hX L%X- ", i, len);
							DumpHex(ReceiveDataBuffer, len);
						}
						
					}
					
				}
			}
		}
	}
		//P0 &= ~0b00100000;
	return (s);
}

uint8_t AddressCounter = 1;

// enum device
// should have already been selected using hub stuff
// returns pointer to USB_HUB_PORT if all is ok
USB_HUB_PORT * EnumerateHubPort(UINT8 speed, UINT8 level)
{
	UINT8 s;
	UINT16 len;
	UINT16 cfgDescLen;

	USB_HUB_PORT *pUsbDevice;
	USB_CFG_DESCR *pCfgDescr;
	UINT8 i;

	//get first 8 bytes of device descriptor to get maxpacketsize0
	s = GetDeviceDescr(NULL, ReceiveDataBuffer, 8, &len);

	if (s != ERR_SUCCESS)
	{
		DEBUGOUT("gdd.fail\n");
		return NULL;
	}

	DEBUGOUT("gdd len:%d\n", len);

	// device seems to be talking to us, so allocate it a USB_HUB_PORT
	uint8_t addr = AddressCounter++;

	PolledDevices = ListAdd(PolledDevices, sizeof(USB_HUB_PORT), addr);

	pUsbDevice = (USB_HUB_PORT *)PolledDevices->data;

	InitHubPortData(pUsbDevice);
	pUsbDevice->DeviceSpeed = speed;

	//set device address
	pUsbDevice->DeviceAddress = addr;

	s = SetUsbAddress(pUsbDevice, pUsbDevice->DeviceAddress);

	if (s != ERR_SUCCESS)
	{
		AddressCounter--;
		DEBUGOUT("addr fail\n");
		return NULL;
	}

	DEBUGOUT("addr ok %d\n", pUsbDevice->DeviceAddress);

	// parse first 8 bytes of device descriptor
	ParseDeviceDescriptor((USB_DEV_DESCR *)ReceiveDataBuffer, len, pUsbDevice);

	DEBUGOUT("mps %d\n", pUsbDevice->MaxPacketSize0);

	if (level > 0)
		pUsbDevice->IsRootHub = 0;
	else
		pUsbDevice->IsRootHub = 1;

	SelectHubPort(pUsbDevice);

	//get full bytes of device descriptor
	s = GetDeviceDescr(pUsbDevice, ReceiveDataBuffer, sizeof(USB_DEV_DESCR), &len);

	if (s != ERR_SUCCESS)
	{
		DEBUGOUT("gddfull fail\n");
		return NULL;
	}

	DEBUGOUT("gddfull ok %d\n", len);

	DEBUGOUT("Device Descriptor :\n")
	DumpHex(ReceiveDataBuffer, len);

	ParseDeviceDescriptor((USB_DEV_DESCR *)ReceiveDataBuffer, len, pUsbDevice);

	//get configure descriptor for the first time
	cfgDescLen = sizeof(USB_CFG_DESCR);

	s = GetConfigDescr(pUsbDevice, ReceiveDataBuffer, cfgDescLen, &len);
	if (s != ERR_SUCCESS)
	{
		DEBUGOUT("gcd1 fail\n");
		return NULL;
	}

	//get configure descriptor for the second time
	pCfgDescr = (USB_CFG_DESCR *)ReceiveDataBuffer;
	cfgDescLen = pCfgDescr->wTotalLengthL | (pCfgDescr->wTotalLengthH << 8);
	if (cfgDescLen > RECEIVE_BUFFER_LEN)
	{
		cfgDescLen = RECEIVE_BUFFER_LEN;
	}

	s = GetConfigDescr(pUsbDevice, ReceiveDataBuffer, cfgDescLen, &len);
	if (s != ERR_SUCCESS)
	{
		DEBUGOUT("gcd 2 fail\n");
		return NULL;
	}

	DEBUGOUT("Config Descriptor :\n")
	DumpHex(ReceiveDataBuffer, len);

	//parse config descriptor
	ParseConfigDescriptor((USB_CFG_DESCR *)ReceiveDataBuffer, len, pUsbDevice);

	DEBUGOUT("gcd ok %d\n", len);

	//set config
	s = SetUsbConfig(pUsbDevice, ((USB_CFG_DESCR *)ReceiveDataBuffer)->bConfigurationValue);
	if (s != ERR_SUCCESS)
	{
		DEBUGOUT("suc fail\n");
		return NULL;
	}

	TRACE("SetUsbConfig OK\r\n");
	TRACE1("configure=%bd\r\n", ((USB_CFG_DESCR *)ReceiveDataBuffer)->bConfigurationValue);

	TRACE1("pUsbDevice->InterfaceNum=%d\r\n", (UINT16)pUsbDevice->InterfaceNum);

	pUsbDevice->HubPortStatus = PORT_DEVICE_ENUM_SUCCESS;


	// OK let's try doing hubs
	
	if (pUsbDevice->DeviceClass == USB_DEV_CLASS_HUB)
	{
		SelectHubPort(pUsbDevice);
		DEBUGOUT("\nFound hub\n");

		//hub
		USB_HUB_DESCR *pHubDescr;

		UINT8 hubPortNum;
		UINT16 hubPortStatus, hubPortChange;

		//hub
		s = GetHubDescriptor(pUsbDevice, ReceiveDataBuffer, sizeof(USB_HUB_DESCR), &len);
		if (s != ERR_SUCCESS)
		{
			//DisableRootHubPort(port);

			pUsbDevice->HubPortStatus = PORT_DEVICE_ENUM_FAILED;
			DEBUGOUT("hub desc err\n");
			return FALSE;
		}

		TRACE("GetHubDescriptor OK\r\n");
		TRACE1("len=%d\r\n", len);

		pHubDescr = (USB_HUB_DESCR *)ReceiveDataBuffer;
		hubPortNum = pHubDescr->bNbrPorts;

		TRACE1("hubPortNum=%bd\r\n", hubPortNum);
		DEBUGOUT("%d port hub\n", hubPortNum);

		if (hubPortNum > MAX_EXHUB_PORT_NUM)
		{
			hubPortNum = MAX_EXHUB_PORT_NUM;
		}

		pUsbDevice->HubPortNum = hubPortNum;
		
		//supply power for each port
		for (UINT8 i = 0; i < hubPortNum; i++)
		{
			s = SetHubPortFeature(pUsbDevice, i + 1, HUB_PORT_POWER);
			if (s != ERR_SUCCESS)
			{
				TRACE1("SetHubPortFeature %d failed\r\n", (UINT16)i);

				continue;
			}

			TRACE("SetHubPortFeature OK\r\n");
		}

		
		INTERFACE *pInterface = (INTERFACE *)ListGetData(pUsbDevice->Interfaces, 0);

		ENDPOINT *pEndPoint = &pInterface->Endpoint[0];

		// "new thing" means wait for a change bitmap before clearing ports
		// this is how other USB stacks seem to do it
		DEBUGOUT("Doing new thing - %x\n", pInterface->Endpoint[0].EndpointAddr);

		uint8_t newthing = 1;

		s = TransferReceive(pEndPoint, ReceiveDataBuffer, &len, 20000);

		if (s != ERR_SUCCESS){
			DEBUGOUT("new enum. failed\n");
			// if "new thing" failed just clear all ports without waiting
			// seems to be required for some hubs
			newthing = 0;
			//return FALSE;
		}

		uint8_t changebitmap = ReceiveDataBuffer[0];

		DEBUGOUT("new enum. %x\n", changebitmap);

		for (i = 0; i < hubPortNum; i++)
		{
			if (!newthing || (changebitmap & (1 << (i+1)))) {
				DEBUGOUT("checkn port %d\n", i);
				mDelaymS(50);

				SelectHubPort(pUsbDevice);

				s = GetHubPortStatus(pUsbDevice, i + 1, &hubPortStatus, &hubPortChange);
				if (s != ERR_SUCCESS)
				{
					TRACE1("GetHubPortStatus port:%d failed\r\n", (UINT16)(i + 1));

					return NULL;
				}

				DEBUGOUT("ps- 0x%02X pc- 0x%02X\n", hubPortStatus, hubPortChange);

				TRACE2("hubPortStatus:0x%02X,hubPortChange:0x%02X\r\n", hubPortStatus, hubPortChange);

				if ((hubPortStatus & 0x0001) && (hubPortChange & 0x0001))
				{
					//device attached
					TRACE1("hubPort=%d\r\n", (UINT16)i);

					TRACE("device attached\r\n");

					DEBUGOUT("port %d attached\n", i);

					s = ClearHubPortFeature(pUsbDevice, i + 1, HUB_C_PORT_CONNECTION);
					if (s != ERR_SUCCESS)
					{
						TRACE("ClearHubPortFeature failed\r\n");
						return NULL;
					}

					TRACE("ClearHubPortFeature OK\r\n");


					s = SetHubPortFeature(pUsbDevice, i + 1, HUB_PORT_RESET); //reset the port device
					if (s != ERR_SUCCESS)
					{
						TRACE1("SetHubPortFeature port:%d failed\r\n", (UINT16)(i + 1));

						return NULL;
					}

					mDelaymS(100);
					do
					{
						s = GetHubPortStatus(pUsbDevice, i + 1, &hubPortStatus, &hubPortChange);
						if (s != ERR_SUCCESS)
						{
							TRACE1("GetHubPortStatus port:%d failed\r\n", (UINT16)(i + 1));
							return NULL;
						}

						mDelaymS(20);
					} while (hubPortStatus & 0x0010);

					if ((hubPortChange & 0x10) == 0x10) //reset over success
					{
						TRACE("reset complete\r\n");

						if (hubPortStatus & 0x0200)
						{
							//speed low
							speed = LOW_SPEED;
							DEBUGOUT("lowspeed\n");
							TRACE("low speed device\r\n");
						}
						else
						{
							//full speed device
							speed = FULL_SPEED;
							DEBUGOUT("fullspeed\n");
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


						// new devices always start at addr zero
						SetHostUsbAddr(0);
						if (speed == LOW_SPEED)
						{
							UH_SETUP |= bUH_PRE_PID_EN;
						}
						SetUsbSpeed(speed);

						if (EnumerateHubPort(speed, level + 1))
						{
							DEBUGOUT("enum.OK\n");
						}
						else
						{
							DEBUGOUT("enum.fail\n");
						}
					}
				}
			}
		}
	}

	return pUsbDevice;
}



BOOL EnumerateRootHubPort(UINT8 port)
{
	static __xdata UINT8 i, s;
	static __xdata UINT16 len;

	static __xdata UINT8 retry;

	static __xdata UINT8 addr;
	static USB_HUB_PORT * __xdata pHubPort;

	retry = 0;

	ResetRootHubPort(port);
	static __xdata uint8_t speed = 0;

	for (i = 0, s = 0; i < 100; i ++) // wait for the USB device to reset and reconnect, 100mS timeout
	{
		mDelaymS( 1 );
		if (EnableRootHubPort( port, &speed ) == ERR_SUCCESS) // Enable ROOT-HUB port
		{
			i = 0;
			break;
		}
	}

	if (i)
	{
		DisableRootHubPort(port);
		TRACE1("Disable root hub %1d# port because of disconnect\n", (UINT16)port);
		return;
	}

	mDelaymS(100);

	printf("ss%x\n", speed);

	// devices always start at addr zero
	SetHostUsbAddr(0);
	SetUsbSpeed(speed);

	pHubPort = EnumerateHubPort(speed, 0);

	if (pHubPort != NULL)
	{


		TRACE("EnumerateHubPort success\r\n");
	}
	else
	{
		DisableRootHubPort(port);
	}

	return TRUE;
}



void regrabinterfaces(__xdata USB_HUB_PORT *pUsbHubPort)
{
	// device will have already been selected at this point
	static __xdata UINT8 i, s, c;
	static __xdata UINT16 len, cnt;
	static __xdata USB_HUB_PORT *pUsbDevice;

	pUsbDevice = pUsbHubPort;
	
	if (pUsbDevice->DeviceClass != USB_DEV_CLASS_HUB)
	{
		for (i = 0; i < pUsbDevice->InterfaceNum; i++)
		{
#ifdef DEBUG
			int j;
#endif

			DEBUGOUT("\n\n");
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

				DEBUGOUT("Interface %x:\n", i);
				DEBUGOUT("InterfaceProtocol: %x\r\n", pInterface->InterfaceProtocol);

				TRACE1("Report Size:%d\r\n", pInterface->ReportSize);
				s = GetReportDescriptor(pUsbDevice, i, ReceiveDataBuffer, pInterface->ReportSize <= sizeof(ReceiveDataBuffer) ? pInterface->ReportSize : sizeof(ReceiveDataBuffer), &len);

				if (s != ERR_SUCCESS)
				{
					DEBUGOUT("rep descr fail %u\n", s);
					return; // FALSE;
				}

				TRACE("GetReportDescriptor OK\r\n");
				TRACE1("get report descr len:%d\r\n", len);

				DEBUGOUT("\n\nInterface %hx Report Descriptor - \n", i);
				DumpHex(ReceiveDataBuffer, len);



				// use default boot mode descriptors if a keyboard or mouse is detected and "advanced USB" is disabled in menu
				if (
					(!HMSettings.MouseReportMode && pInterface->InterfaceProtocol == HID_PROTOCOL_MOUSE) ||
					(!HMSettings.KeyboardReportMode && pInterface->InterfaceProtocol == HID_PROTOCOL_KEYBOARD)
					)
				{
					// if it supports boot mode, enable that and use the default descriptor
					if (pInterface->InterfaceSubClass == 0x01) {
						DEBUGOUT("set boot mode - %x - ", GetBootProtocol(pUsbDevice, i));
						SetBootProtocol(pUsbDevice, i, 0);
						ParseReportDescriptor(
							pInterface->InterfaceProtocol == HID_PROTOCOL_MOUSE ? StandardMouseDescriptor : StandardKeyboardDescriptor, 
							pInterface->InterfaceProtocol == HID_PROTOCOL_MOUSE ? 50 : 63, 
							pInterface
						);
						
						DEBUGOUT("%x\n", GetBootProtocol(pUsbDevice, i));
					}
					// Otherwise don't attempt to use this device at all unless advanced USB is enabled
					else{ 
						DEBUGOUT("No boot mode\n");
						continue;
					}
				}
				else {
					// only switch into report mode if boot mode is supported (yeah that's the way it works)
					if (pInterface->InterfaceSubClass == 0x01) {
						DEBUGOUT("set report mode - %x - ", GetBootProtocol(pUsbDevice, i));
						SetBootProtocol(pUsbDevice, i, 1);
						DEBUGOUT("%x\n", GetBootProtocol(pUsbDevice, i));
					}
					
					ParseReportDescriptor(ReceiveDataBuffer, len, pInterface);
					DumpHID(pInterface);
				}

				if (pInterface->InterfaceProtocol == HID_PROTOCOL_KEYBOARD)
				{
					UINT8 led;

					led = GetKeyboardLedStatus();
					SetReport(pUsbDevice, i, &led, sizeof(led));
					DEBUGOUT("SetReport\n");
				}
			}
		}
	}
}

void RegrabDeviceReports()
{
	static __xdata LinkedList * __xdata pnt;
	static __xdata USB_HUB_PORT * __xdata pUsbHubPort;
	pnt = PolledDevices;

	while (pnt != NULL) {
		pUsbHubPort = (USB_HUB_PORT *)pnt->data;
		printf("%d\n",pnt->index);

		if (pUsbHubPort->HubPortStatus == PORT_DEVICE_ENUM_SUCCESS)
		{
			SelectHubPort(pUsbHubPort);
			regrabinterfaces(pUsbHubPort);
		
		}
		pnt = pnt->next;
	}
}

void ReenumerateAllPorts(void) {
	UINT8 i;
	
	if (!KeyboardDebugOutput)
		OutputsEnabled = 0;

	DEBUGOUT("reenumerating all ports\n");
	mDelaymS(150);

	andyclearmem();
	InitUsbData();
	InitPresets();
	
	for (i = 0; i < ROOT_HUB_PORT_NUM; i++)
	{
		DEBUGOUT("port %d\n", i);

		EnumerateRootHubPort(i);
	}

	RegrabDeviceReports();

	DEBUGOUT("done reenumerating\n");
	OutputsEnabled = 1;
}

//----------------------------------------------------------------------------------
void DealUsbPort(void) //main function should use it at least 500ms
{
	// detect change
	// may have to be more complex than this
	if (UIF_DETECT)
	{
		ReenumerateAllPorts();
	}
}

void InterruptProcessRootHubPorts()
{

	static __xdata LinkedList * __xdata pnt;
	static __xdata USB_HUB_PORT * __xdata pUsbHubPort;
	pnt = PolledDevices;

	while (pnt != NULL) {
		pUsbHubPort = (USB_HUB_PORT *)pnt->data;

		if (pUsbHubPort->HubPortStatus == PORT_DEVICE_ENUM_SUCCESS)
		{
			SelectHubPort(pUsbHubPort);
			HIDDataTransferReceive(pUsbHubPort);
		}
		pnt = pnt->next;
	}

	return;

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

/*	for (i = 0; i < ROOT_HUB_PORT_NUM; i++)
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
	}*/
}

__xdata BOOL volatile s_CheckUsbPort0 = FALSE;
__xdata BOOL volatile s_CheckUsbPort1 = FALSE;

void ProcessUsbHostPort(void)
{	

	DealUsbPort();
	if (s_CheckUsbPort0)
	{
		s_CheckUsbPort0 = FALSE;
		
		InterruptProcessRootHubPorts();
	}
}
