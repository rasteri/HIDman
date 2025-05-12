#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

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
#include "preset.h"
#include "test.h"
#include "usbll.h"
#include "linkedlist.h"
#include "testcommon.h"
//#define TESTVERBOSE 1

/*
Things we might want to test:
- HID report parsing. Rather than counting segs, search for segs that are the correct type, in the correct startbit, etc
- HID report processing. Can actually dig the bits out and get a sensible result at the end
- Ring buffer functions (when we write them lol)
- Presskey/releasekey actually gives a sensible result out of the ringbuffer at the end
- Test entire thing - parse a report, send some events, make sure they come out of ringbuffer at end

Probably gonna need seperate executables for different tests because of limited code space
 
*/

int putchar(int c)
{
    while (!TI)
        ;
    TI = 0;
    SBUF = c & 0xFF;
}

void UART_Init(void)
{
    SCON = 0x50; /* configure serial */
    TMOD = 0x20; /* configure timer */
    TH1 = 0xE6;  /* baud rate 1200 */
    TL1 = 0xE6;  /* baud rate 1200 */
    TR1 = 1;     /* enable timer */
    TI = 1;      /* enable transmitting */
    RI = 0;      /* waiting to receive */
}

bool InterfaceParseReportDescriptor (__xdata INTERFACE * pInterface, uint8_t *Report, uint16_t ReportLen) {

    #ifdef TESTVERBOSE 
        printf("InterfaceClass=0x%02X - ", (UINT16)pInterface->InterfaceClass);
        printf("InterfaceProtocol=0x%02X\n", (UINT16)pInterface->InterfaceProtocol);
    #endif

    if (pInterface->InterfaceClass == USB_DEV_CLASS_HID)
    {
        if (!ParseReportDescriptor(Report, ReportLen, pInterface)) {
            printf("Can't parse Report Descriptor\n");
            return 1;
        }
        return 0;
    }
    return 1;
}

bool InterfaceParseDeviceDescriptors (
    USB_HUB_PORT * pUsbDevice,
    uint8_t *Dev, uint16_t DevLen, 
    uint8_t *Config, uint16_t ConfigLen) {

    if (!ParseDeviceDescriptor((USB_DEV_DESCR *)Dev, DevLen, pUsbDevice)) {
        printf("Can't parse Device Descriptor\n");
        return 1;
    }

    if (!ParseConfigDescriptor((USB_CFG_DESCR *)Config, ConfigLen, pUsbDevice)) {
        printf("Can't parse Config Descriptor\n");
        return 1;
    }

    #ifdef TESTVERBOSE 
        printf("Num Interfaces=%d\n", pUsbDevice->InterfaceNum);
        printf("VendorID=0x%04X,ProductID=0x%04X,bcdDevice=0x%04X\n", pUsbDevice->VendorID, pUsbDevice->ProductID, pUsbDevice->bcdDevice);
    #endif 


    return 0;

}


void InitTest (
    USB_HUB_PORT *pUsbDevice,
    uint8_t *Dev, uint16_t DevLen, 
    uint8_t *Config, uint16_t ConfigLen
){

    andyclearmem();
    InitPresets();
    InitHubPortData(pUsbDevice);

    assert(
        !InterfaceParseDeviceDescriptors(
            pUsbDevice,
            Dev, DevLen,
            Config, ConfigLen)
    );

}

__xdata HID_SEG * FindSegByStartBit(__xdata HID_REPORT * descReport, uint16_t StartBit) {

    assert(descReport != NULL);

	__xdata LinkedList *currSegNode = descReport->HidSegments;

	while (currSegNode != NULL)
	{
		if (((__xdata HID_SEG *)(currSegNode->data))->startBit == StartBit)
            return (__xdata HID_SEG *)(currSegNode->data);
		currSegNode = currSegNode->next;
	}
    return NULL;
}

uint32_t rand32(void) {
    return ((uint32_t)(rand()) << 16) | (uint32_t)rand();
}