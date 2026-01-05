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
	static int i;

	for (i = 0; i < ROOT_HUB_PORT_NUM; i++)
	{
		InitRootHubPortData(i);
	}
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

//enum device
BOOL EnumerateHubPort(__xdata USB_HUB_PORT *pUsbHubPort, UINT8 addr)
{
	static __xdata UINT8 s;
	static __xdata UINT16 len;
	static __xdata UINT16 cfgDescLen;

	static __xdata USB_HUB_PORT *pUsbDevice;
	static __xdata USB_CFG_DESCR *pCfgDescr;

	pUsbDevice = pUsbHubPort;

	//get first 8 bytes of device descriptor to get maxpacketsize0
	s = GetDeviceDescr(pUsbDevice, ReceiveDataBuffer, 8, &len);

	if (s != ERR_SUCCESS)
	{
		DEBUGOUT("gdd.fail\n");
		return (FALSE);
	}

	DEBUGOUT("gdd len:%d\n", len);

	ParseDeviceDescriptor((USB_DEV_DESCR *)ReceiveDataBuffer, len, pUsbDevice);

	DEBUGOUT("mps %d\n", pUsbDevice->MaxPacketSize0);

	//set device address
	s = SetUsbAddress(pUsbDevice, addr);
	if (s != ERR_SUCCESS)
	{
		DEBUGOUT("addr fail\n");
		return (FALSE);
	}

	DEBUGOUT("addr ok %d\n", pUsbDevice->DeviceAddress);
	pUsbDevice->DeviceAddress = addr;
	//get full bytes of device descriptor
	s = GetDeviceDescr(pUsbDevice, ReceiveDataBuffer, sizeof(USB_DEV_DESCR), &len);

	if (s != ERR_SUCCESS)
	{
		DEBUGOUT("gddfull fail\n");
		return (FALSE);
	}

	DEBUGOUT("gddfull ok %d\n", len);

	DEBUGOUT("Device Descriptor :\n")
	DumpHex(ReceiveDataBuffer, len);

	ParseDeviceDescriptor((USB_DEV_DESCR *)ReceiveDataBuffer, len, pUsbDevice);

	DEBUGOUT("0x%04X 0x%04X 0x%04X\n", pUsbDevice->VendorID, pUsbDevice->ProductID, pUsbDevice->bcdDevice);

	//get configure descriptor for the first time
	cfgDescLen = sizeof(USB_CFG_DESCR);

	s = GetConfigDescr(pUsbDevice, ReceiveDataBuffer, cfgDescLen, &len);
	if (s != ERR_SUCCESS)
	{
		DEBUGOUT("gcd1 fail\n");
		return (FALSE);
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
		return (FALSE);
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
		return (FALSE);
	}

	TRACE("SetUsbConfig OK\r\n");
	TRACE1("configure=%bd\r\n", ((USB_CFG_DESCR *)ReceiveDataBuffer)->bConfigurationValue);

	TRACE1("pUsbDevice->InterfaceNum=%d\r\n", (UINT16)pUsbDevice->InterfaceNum);

	return (TRUE);
}

// Global address counter for nested hub support
static UINT8 g_NextUsbAddress = 1;

// Reset address counter (called during initialization)
void ResetAddressAllocation(void)
{
	g_NextUsbAddress = 1;
}

// Assign unique address for any device (supports nested hubs)
UINT8 AssignUniqueAddress(UINT8 RootHubIndex, UINT8 HubPortIndex)
{
	static UINT8 address;
	
	// Use sequential addressing for better nested hub support
	address = g_NextUsbAddress++;
	
	// Limit to valid USB address range (1-127)
	if (address > 127)
	{
		// Reset if we run out of addresses (shouldn't happen with typical setups)
		g_NextUsbAddress = 1;
		address = 1;
	}
	
	return address;
}

// Recursive function to initialize hub and its child ports
// Returns TRUE if successful
BOOL InitializeHubPorts(__xdata USB_HUB_PORT *pHubDevice, UINT8 rootHubIndex)
{
	UINT8 i, s;
	UINT16 len;
	__xdata USB_HUB_DESCR *pHubDescr;
	UINT8 hubPortNum;
	UINT16 hubPortStatus, hubPortChange;
	UINT8 addr;
	__xdata USB_HUB_PORT *pChildPort;
	
	if (pHubDevice == NULL || pHubDevice->DeviceClass != USB_DEV_CLASS_HUB)
	{
		return TRUE; // Not a hub, nothing to do
	}
	
	// Check hub level doesn't exceed maximum
	if (pHubDevice->HubLevel >= MAX_EXHUB_LEVEL)
	{
		DEBUGOUT("Max hub level reached\n");
		return FALSE;
	}
	
	DEBUGOUT("\nInitializing hub at level %d\n", pHubDevice->HubLevel);
	
	// Select this hub for communication
	SelectHubPortByDevice(pHubDevice);
	
	// Get hub descriptor
	s = GetHubDescriptor(pHubDevice, ReceiveDataBuffer, sizeof(USB_HUB_DESCR), &len);
	if (s != ERR_SUCCESS)
	{
		DEBUGOUT("hub desc err\n");
		return FALSE;
	}
	
	TRACE("GetHubDescriptor OK\r\n");
	
	pHubDescr = (USB_HUB_DESCR *)ReceiveDataBuffer;
	hubPortNum = pHubDescr->bNbrPorts;
	
	DEBUGOUT("%d port hub\n", hubPortNum);
	
	if (hubPortNum > MAX_EXHUB_PORT_NUM)
	{
		hubPortNum = MAX_EXHUB_PORT_NUM;
	}
	
	pHubDevice->HubPortNum = hubPortNum;
	
	// Note: We don't pre-allocate child ports here to save memory
	// Child ports are allocated on-demand when devices are detected
	
	// Supply power to all ports
	for (i = 0; i < hubPortNum; i++)
	{
		s = SetHubPortFeature(pHubDevice, i + 1, HUB_PORT_POWER);
		if (s != ERR_SUCCESS)
		{
			TRACE1("SetHubPortFeature %d failed\r\n", (UINT16)i);
			// Don't mark as failed yet - port may not have a device
			continue;
		}
		TRACE("SetHubPortFeature OK\r\n");
	}
	
	// Enumerate each port (without checking change bitmap - simpler approach)
	// The original code had an optimization to check status change endpoint,
	// but that requires hub interfaces which aren't parsed by ParseConfigDescriptor
	for (i = 0; i < hubPortNum; i++)
	{
		DEBUGOUT("Checking port %d\n", i);
		mDelaymS(50);
		
		SelectHubPortByDevice(pHubDevice);
		
		s = GetHubPortStatus(pHubDevice, i + 1, &hubPortStatus, &hubPortChange);
		if (s != ERR_SUCCESS)
		{
			TRACE1("GetHubPortStatus port:%d failed\r\n", (UINT16)(i + 1));
			continue;
		}
		
		DEBUGOUT("ps- 0x%02X pc- 0x%02X\n", hubPortStatus, hubPortChange);
		
		if ((hubPortStatus & 0x0001) && (hubPortChange & 0x0001))
		{
			// Device attached - allocate port structure now
			DEBUGOUT("port %d attached\n", i);
			
			pChildPort = AllocateSingleChildPort();
			if (pChildPort == NULL)
			{
				DEBUGOUT("Failed to allocate child port %d\n", i);
				continue;
			}
			
			// Set up parent-child relationship
			pHubDevice->ChildHubPorts[i] = pChildPort;
			pChildPort->ParentHub = pHubDevice;
			pChildPort->ParentHubPortIndex = i;
			pChildPort->HubLevel = pHubDevice->HubLevel + 1;
				
				s = ClearHubPortFeature(pHubDevice, i + 1, HUB_C_PORT_CONNECTION);
				if (s != ERR_SUCCESS)
				{
					pChildPort->HubPortStatus = PORT_DEVICE_ENUM_FAILED;
					TRACE("ClearHubPortFeature failed\r\n");
					continue;
				}
				
				// Reset the port
				s = SetHubPortFeature(pHubDevice, i + 1, HUB_PORT_RESET);
				if (s != ERR_SUCCESS)
				{
					pChildPort->HubPortStatus = PORT_DEVICE_ENUM_FAILED;
					TRACE1("SetHubPortFeature port:%d failed\r\n", (UINT16)(i + 1));
					continue;
				}
				
				mDelaymS(100);
				
				// Wait for reset to complete
				do
				{
					s = GetHubPortStatus(pHubDevice, i + 1, &hubPortStatus, &hubPortChange);
					if (s != ERR_SUCCESS)
					{
						pChildPort->HubPortStatus = PORT_DEVICE_ENUM_FAILED;
						TRACE1("GetHubPortStatus port:%d failed\r\n", (UINT16)(i + 1));
						break;
					}
					mDelaymS(20);
				} while (hubPortStatus & 0x0010);
				
				if ((hubPortChange & 0x10) == 0x10) // Reset complete
				{
					TRACE("reset complete\r\n");
					
					// Determine speed
					if (hubPortStatus & 0x0200)
					{
						pChildPort->DeviceSpeed = LOW_SPEED;
						DEBUGOUT("lowspeed\n");
					}
					else
					{
						pChildPort->DeviceSpeed = FULL_SPEED;
						DEBUGOUT("fullspeed\n");
					}
					
					s = ClearHubPortFeature(pHubDevice, i + 1, HUB_C_PORT_RESET);
					if (s != ERR_SUCCESS)
					{
						TRACE("ClearHubPortFeature failed\r\n");
					}
					
					// Select this child port for enumeration
					SelectHubPortByDevice(pChildPort);
					
					// Assign address and enumerate
					addr = AssignUniqueAddress(rootHubIndex, i);
					if (EnumerateHubPort(pChildPort, addr))
					{
						DEBUGOUT("enum.OK\n");
						pChildPort->HubPortStatus = PORT_DEVICE_ENUM_SUCCESS;
						
						// If this device is also a hub, recursively initialize it
						if (pChildPort->DeviceClass == USB_DEV_CLASS_HUB)
						{
							DEBUGOUT("Found nested hub\n");
							if (!InitializeHubPorts(pChildPort, rootHubIndex))
							{
								DEBUGOUT("Nested hub init failed\n");
								pChildPort->HubPortStatus = PORT_DEVICE_ENUM_FAILED;
							}
						}
					}
					else
					{
						DEBUGOUT("enum.fail\n");
						pChildPort->HubPortStatus = PORT_DEVICE_ENUM_FAILED;
					}
				}
		}
	}
	
	return TRUE;
}

BOOL EnumerateRootHubPort(UINT8 port)
{
	static __xdata UINT8 i, s;
	static __xdata UINT16 len;

	static __xdata UINT8 retry;

	static __xdata UINT8 addr;
	retry = 0;
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

		// If this is a hub, recursively initialize its ports
		if (RootHubPort[port].DeviceClass == USB_DEV_CLASS_HUB)
		{
			DEBUGOUT("\nFound root hub\n");
			
			if (!InitializeHubPorts(&RootHubPort[port], port))
			{
				DEBUGOUT("Hub initialization failed\n");
				DisableRootHubPort(port);
				RootHubPort[port].HubPortStatus = PORT_DEVICE_ENUM_FAILED;
				return FALSE;
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
	static __xdata BOOL res;

	static __xdata UINT8 s;

	res = FALSE; s = ERR_SUCCESS;

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


void regrabinterfaces(__xdata USB_HUB_PORT *pUsbHubPort)
{
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

// Recursive function to regrab interfaces for all devices in hub tree
void RegrabInterfacesRecursive(__xdata USB_HUB_PORT *pUsbHubPort)
{
	UINT8 i;
	
	if (pUsbHubPort == NULL || pUsbHubPort->HubPortStatus != PORT_DEVICE_ENUM_SUCCESS)
	{
		return;
	}
	
	if (pUsbHubPort->DeviceClass != USB_DEV_CLASS_HUB)
	{
		// This is a leaf device, regrab its interfaces
		SelectHubPortByDevice(pUsbHubPort);
		regrabinterfaces(pUsbHubPort);
	}
	else
	{
		// This is a hub, recursively process its children
		for (i = 0; i < pUsbHubPort->HubPortNum; i++)
		{
			if (pUsbHubPort->ChildHubPorts[i] != NULL)
			{
				RegrabInterfacesRecursive(pUsbHubPort->ChildHubPorts[i]);
			}
		}
	}
}

// Recursive function to process HID data transfers for all devices in hub tree
void ProcessHIDDataRecursive(__xdata USB_HUB_PORT *pUsbHubPort)
{
	UINT8 i;
	
	if (pUsbHubPort == NULL || pUsbHubPort->HubPortStatus != PORT_DEVICE_ENUM_SUCCESS)
	{
		return;
	}
	
	if (pUsbHubPort->DeviceClass != USB_DEV_CLASS_HUB)
	{
		// This is a leaf device, process its HID data
		SelectHubPortByDevice(pUsbHubPort);
		HIDDataTransferReceive(pUsbHubPort);
	}
	else
	{
		// This is a hub, recursively process its children
		for (i = 0; i < pUsbHubPort->HubPortNum; i++)
		{
			if (pUsbHubPort->ChildHubPorts[i] != NULL)
			{
				ProcessHIDDataRecursive(pUsbHubPort->ChildHubPorts[i]);
			}
		}
	}
}

void RegrabDeviceReports(UINT8 port)
{
	static __xdata USB_HUB_PORT *pUsbHubPort;
	pUsbHubPort = &RootHubPort[port];

	// Use recursive function to handle all devices including nested hubs
	RegrabInterfacesRecursive(pUsbHubPort);
}

void ReenumerateAllPorts(void) {
	UINT8 i;
	
	if (!KeyboardDebugOutput)
		OutputsEnabled = 0;

	DEBUGOUT("reenumerating all ports\n");
	mDelaymS(150);

	InitUsbData();
	andyclearmem();
	InitPresets();
	ResetAddressAllocation();  // Reset address counter for new enumeration
	
	for (i = 0; i < ROOT_HUB_PORT_NUM; i++)
	{
		DEBUGOUT("port %d\n", i);

		EnumerateRootHubPort(i);
		RegrabDeviceReports(i);
	}
	DEBUGOUT("done reenumerating\n");
	OutputsEnabled = 1;
	
	// Rebuild the flat hub list for efficient polling
	RebuildHubList();
}

//----------------------------------------------------------------------------------
void DealUsbPort(void) //main function should use it at least 500ms
{
	static __xdata UINT8 s;
	UINT8 i;
	UINT16 hubPortStatus, hubPortChange;
	__xdata USB_HUB_PORT *pHub;

	s = QueryHubPortAttach();
	if (s == ERR_USB_CONNECT)
	{
		ReenumerateAllPorts();
		return;
	}
	
	// Check one external hub per cycle for hotplug detection (rotating through all hubs)
	if (HubListCount > 0)
	{
		// Get the current hub to check
		pHub = HubList[HubPollIndex];
		
		// Move to next hub for the next cycle
		HubPollIndex++;
		if (HubPollIndex >= HubListCount)
		{
			HubPollIndex = 0;
		}
		
		// Check this hub's ports for changes
		if (pHub != NULL && pHub->HubPortStatus == PORT_DEVICE_ENUM_SUCCESS)
		{
			// Select this hub for communication
			SelectHubPortByDevice(pHub);
			
			// Check each port for connection changes
			for (i = 0; i < pHub->HubPortNum; i++)
			{
				s = GetHubPortStatus(pHub, i + 1, &hubPortStatus, &hubPortChange);
				if (s == ERR_SUCCESS)
				{
					// Check for connection change bit
					if (hubPortChange & 0x0001)
					{
						DEBUGOUT("Hub port %d change detected, re-enumerating\n", i);
						ReenumerateAllPorts();
						return;
					}
				}
			}
		}
	}
}

void InterruptProcessRootHubPort(UINT8 port)
{
	static __xdata USB_HUB_PORT *pUsbHubPort;
	pUsbHubPort = &RootHubPort[port];

	// Use recursive function to handle all devices including nested hubs
	ProcessHIDDataRecursive(pUsbHubPort);
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

// Recursive function to update keyboard LEDs for all devices in hub tree
void UpdateKeyboardLedRecursive(__xdata USB_HUB_PORT *pUsbHubPort, UINT8 led)
{
	UINT8 i;
	
	if (pUsbHubPort == NULL || pUsbHubPort->HubPortStatus != PORT_DEVICE_ENUM_SUCCESS)
	{
		return;
	}
	
	if (pUsbHubPort->DeviceClass != USB_DEV_CLASS_HUB)
	{
		// This is a leaf device, update its keyboard LEDs
		SelectHubPortByDevice(pUsbHubPort);
		UpdateUsbKeyboardLedInternal(pUsbHubPort, led);
	}
	else
	{
		// This is a hub, recursively process its children
		for (i = 0; i < pUsbHubPort->HubPortNum; i++)
		{
			if (pUsbHubPort->ChildHubPorts[i] != NULL)
			{
				UpdateKeyboardLedRecursive(pUsbHubPort->ChildHubPorts[i], led);
			}
		}
	}
}

//-----------------------------------------------------------------------------
void UpdateUsbKeyboardLed(UINT8 led)
{
	UINT8 i;

	for (i = 0; i < ROOT_HUB_PORT_NUM; i++)
	{
		UpdateKeyboardLedRecursive(&RootHubPort[i], led);
	}
}

__xdata BOOL volatile s_CheckUsbPort0 = FALSE;
__xdata BOOL volatile s_CheckUsbPort1 = FALSE;

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
