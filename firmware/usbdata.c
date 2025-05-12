
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

void InitHubPortData(USB_HUB_PORT *pUsbHubPort)
{
	pUsbHubPort->HubPortStatus = PORT_DEVICE_NONE;
	pUsbHubPort->DeviceClass = USB_DEV_CLASS_RESERVED;
	pUsbHubPort->MaxPacketSize0 = DEFAULT_ENDP0_SIZE;

	pUsbHubPort->VendorID = 0x0000;
	pUsbHubPort->ProductID = 0x0000;
	pUsbHubPort->bcdDevice = 0x0000;

	pUsbHubPort->DeviceAddress = 0;
	pUsbHubPort->DeviceSpeed = FULL_SPEED;
	pUsbHubPort->InterfaceNum = 0;
	pUsbHubPort->Interfaces = NULL;

	pUsbHubPort->HubPortNum = 0;
}
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