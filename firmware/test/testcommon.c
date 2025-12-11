#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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
#include "ps2.h"
#include "testdata.h"
#include "preset.h"
#include "test.h"
#include "usbll.h"
#include "linkedlist.h"
#include "testcommon.h"
#include "scancode.h"
#include "processreport.h"
#define TESTVERBOSE 1

/*
Things we might want to test:
- HID report parsing. Rather than counting segs, search for segs that are the correct type, in the correct startbit, etc
- HID report processing. Can actually dig the bits out and get a sensible result at the end
- Ring buffer functions (when we write them lol)
- Presskey/releasekey actually gives a sensible result out of the ringbuffer at the end
- Test entire thing - parse a report, send some events, make sure they come out of ringbuffer at end

Probably gonna need seperate executables for different tests because of limited code space
 
*/

__xdata bool MenuActive = 0;

__xdata USB_HUB_PORT UsbDev;

// mask off the number of bits
__code uint32_t bitMasks32[] = {
    0x0000, 
    0x0001, 0x0003, 0x0007, 0x000f, 0x001F, 0x003F, 0x007F, 0x00FF,
    0x01FF, 0x03FF, 0x07FF, 0x0fFF, 0x1fFF, 0x3fFF, 0x7fFF, 0xFFFF,
    0x0001FFFF, 0x0003FFFF, 0x0007FFFF, 0x000fFFFF, 0x001FFFFF, 0x003FFFFF, 0x007FFFFF, 0x00FFFFFF,
    0x01FFFFFF, 0x03FFFFFF, 0x07FFFFFF, 0x0fFFFFFF, 0x1fFFFFFF, 0x3fFFFFFF, 0x7fFFFFFF, 0xFFFFFFFF
 };

__at(0xF000) __code Settings DefSettings = {
    0x54178008, //magic
    0, //KeyboardReportMode
    0, //MouseReportMode
    0, //EnableAUXPS2
    0, //Intellimouse
    0, //XT83Keys
    1,//GameControllerAsMouse
    0, //SerialDebugOutput
    0, //USBFilter
    MODE_PS2, //KeyboardMode
    0 //MenuRateLimit
};

__xdata volatile uint16_t SoftWatchdog = 0;

void Menu_Press_Key(uint8_t key)
{

}

void SetKeyboardLedStatusFromPS2(UINT8 ps2led)
{

}

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
    printf("Not a HID interface\n");
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

uint8_t * GetNextChunk(){
    if (ports[PORT_KEY].sendBuffEnd == ports[PORT_MOUSE].sendBuffStart)
        return NULL;
    uint8_t * chunk = ports[PORT_KEY].sendBuff.chunky[ports[PORT_KEY].sendBuffStart];
    ports[PORT_KEY].sendBuffStart = (ports[PORT_KEY].sendBuffStart + 1) & 0x3F;
    return chunk;
}

uint8_t * GetNextChonk(){
    if (ports[PORT_MOUSE].sendBuffEnd == ports[PORT_MOUSE].sendBuffStart)
        return NULL;
    uint8_t * chonk = ports[PORT_MOUSE].sendBuff.chonky[ports[PORT_MOUSE].sendBuffStart];
    ports[PORT_MOUSE].sendBuffStart = (ports[PORT_MOUSE].sendBuffStart + 1) & 0x07;
    return chonk;
}


void InitTest (
    USB_HUB_PORT *pUsbDevice,
    uint8_t *Dev, uint16_t DevLen, 
    uint8_t *Config, uint16_t ConfigLen
) {

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

#define SIF_ADDRESS_SPACE_NAME	"xram"
#define SIF_ADDRESS_SPACE	__xdata
#define SIF_ADDRESS		0xffff

volatile unsigned char SIF_ADDRESS_SPACE * sif;

enum sif_command {
  DETECT_SIGN	    = '!',	// answer to detect command
  SIFCM_DETECT		= '_',	// command used to detect the interface
  SIFCM_COMMANDS	= 'i',	// get info about commands
  SIFCM_IFVER		= 'v',	// interface version
  SIFCM_SIMVER		= 'V',	// simulator version
  SIFCM_IFRESET		= '@',	// reset the interface
  SIFCM_CMDINFO		= 'I',	// info about a command
  SIFCM_CMDHELP		= 'h',	// help about a command
  SIFCM_STOP		= 's',	// stop simulation
  SIFCM_PRINT		= 'p',	// print character
  SIFCM_FIN_CHECK	= 'f',	// check input file for input
  SIFCM_READ		= 'r',	// read from input file
  SIFCM_WRITE		= 'w',	// write to output file
};

char
detect(void)
{
  *sif= SIFCM_DETECT;
  printf("%x det\n", *sif);
}

void halt(void)
{
  *sif= SIFCM_STOP;
}

void __assert(const char *expression, const char *functionname, const char *filename, unsigned int linenumber)
{
	printf("Assert(%s) failed in function %s at line %u in file %s.\n",
		expression, functionname, linenumber, filename);
    halt();
}


void TestSetup()
{
    sif= (unsigned char SIF_ADDRESS_SPACE *) SIF_ADDRESS;

    // timer0 setup
	TMOD = (TMOD & 0xf0) | 0x02; // mode 1 (8bit auto relo    printf("Standard Keyboard Parser Test - PASS\n");ad)
	TH0 = 0x00;					 // I dunno

	TR0 = 1; // start timer0
	ET0 = 1; //enable timer0 interrupt;

	EA = 1;	 // enable all interrupts

    UART_Init();

    //printstackpointer();
}