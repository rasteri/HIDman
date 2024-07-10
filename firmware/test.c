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
#include "testdata.h"

void UART_Init()
{
    SCON = 0x50; /* configure serial */
    TMOD = 0x20; /* configure timer */
    TH1 = 0xE6;  /* baud rate 1200 */
    TL1 = 0xE6;  /* baud rate 1200 */
    TR1 = 1;     /* enable timer */
    TI = 1;      /* enable transmitting */
    RI = 0;      /* waiting to receive */
}

static USB_HUB_PORT __xdata TestPort;

void test(){

    INTERFACE *pInterface;

    andyclearmem();
	sInterfacePoolPos = 0;

    USB_HUB_PORT *pUsbDevice = &TestPort;
    HID_SEG *tmpseg;

    ParseDeviceDescriptor((USB_DEV_DESCR *)PS4DeviceDescriptor, 18, pUsbDevice);
    
    printf("VendorID=0x%04X,ProductID=0x%04X,bcdDevice=0x%04X\n", pUsbDevice->VendorID, pUsbDevice->ProductID, pUsbDevice->bcdDevice);

    ParseConfigDescriptor((USB_CFG_DESCR *)PS4ConfigDescriptor, 225, pUsbDevice);

    printf("Num Interfaces=%d\n", pUsbDevice->InterfaceNum);



    for (uint8_t i = 0; i < pUsbDevice->InterfaceNum; i++)
    {
        pInterface = &pUsbDevice->Interface[i];
        printf("\n\nInterface %d - ", i);
        printf("InterfaceClass=0x%02X - ", (UINT16)pInterface->InterfaceClass);
		printf("InterfaceProtocol=0x%02X\n", (UINT16)pInterface->InterfaceProtocol);

		if (pInterface->InterfaceClass == USB_DEV_CLASS_HID)
		{
            printf("Oooh that's HID\n");
            ParseReportDescriptor(PS4ReportDescriptor, 507, pInterface);

            for (uint8_t x = 0; x < MAX_REPORTS; x++)
            {
                if (pInterface->reports[x] != NULL)
                {
                    tmpseg = pInterface->reports[x]->firstHidSeg;

                    printf("Report %x, usage %x, length %u: \n", x, pInterface->reports[x]->appUsage, pInterface->reports[x]->length);
                    while (tmpseg != NULL)
                    {
                        printf("  startbit %u, it %hx, ip %x, chan %hx, cont %hx, size %hx, count %hx\n", tmpseg->startBit, tmpseg->InputType, tmpseg->InputParam, tmpseg->OutputChannel, tmpseg->OutputControl, tmpseg->reportSize, tmpseg->reportCount);
                        tmpseg = tmpseg->next;
                    }
                }
            }
        }
    }

    /*EnumerateRootHubPort(i);
    RegrabDeviceReports(i);*/
}

INTERFACE funky;

void main()
{
    UART_Init();

    test();

    /*ParseReportDescriptor(StandardKeyboardDescriptor, 63, &funky);

    INTERFACE *bleh = &funky;

    HID_SEG *tmpseg;
    for (uint8_t x = 0; x < MAX_REPORTS; x++)
    {
        if (bleh->reports[x] != NULL)
        {
            tmpseg = bleh->reports[x]->firstHidSeg;

            printf("Report %x, usage %x, length %u: \n", x, bleh->reports[x]->appUsage, bleh->reports[x]->length);
            while (tmpseg != NULL)
            {
                printf("  startbit %u, it %hx, ip %x, chan %hx, cont %hx, size %hx, count %hx\n", tmpseg->startBit, tmpseg->InputType, tmpseg->InputParam, tmpseg->OutputChannel, tmpseg->OutputControl, tmpseg->reportSize, tmpseg->reportCount);
                tmpseg = tmpseg->next;
            }
        }
    }

    printf("\n");*/

    for (;;)
    {
        //printf("v");
    }
}