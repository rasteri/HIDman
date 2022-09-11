#ifndef __USBHOST_H__
#define __USBHOST_H__

#include "type.h"

#define ROOT_HUB_COUNT  2

#define MAX_EXHUB_PORT_COUNT    4
#define EXHUB_PORT_NONE         0xff
#define MAX_INTERFACE_COUNT     4
#define MAX_ENDPOINT_COUNT      4
#define MAX_EXHUB_LEVEL         1
#define ENDPOINT_OUT            0
#define ENDPOINT_IN             1


#define ERR_SUCCESS         0x00
#define ERR_USB_CONNECT     0x15
#define ERR_USB_DISCON      0x16
#define ERR_USB_BUF_OVER    0x17
#define ERR_USB_DISK_ERR    0x1F
#define ERR_USB_TRANSFER    0x20 
#define ERR_USB_UNSUPPORT   0xFB
#define ERR_USB_UNKNOWN     0xFE

#define ROOT_DEVICE_DISCONNECT  0
#define ROOT_DEVICE_CONNECTED   1
#define ROOT_DEVICE_FAILED      2
#define ROOT_DEVICE_SUCCESS     3

/*#define DEV_TYPE_KEYBOARD   ( USB_DEV_CLASS_HID | 0x20 )
#define DEV_TYPE_MOUSE      ( USB_DEV_CLASS_HID | 0x30 )
#define DEV_TYPE_JOYSTICK      ( USB_DEV_CLASS_HID | 0x40 )
#define DEV_TYPE_GAMEPAD      ( USB_DEV_CLASS_HID | 0x50 )*/

#define HID_SEG_KEYBOARD_MODIFIER_INDEX 0
#define HID_SEG_KEYBOARD_VAL_INDEX      1
#define HID_SEG_BUTTON_INDEX            2
#define HID_SEG_X_INDEX                 3
#define HID_SEG_Y_INDEX                 4
#define HID_SEG_WHEEL_INDEX             5
#define HID_SEG_NUM                   6

#define REPORT_USAGE_PAGE 		0x04
#define REPORT_USAGE 			0x08
#define REPORT_LOCAL_MINIMUM 	0x14
#define REPORT_LOCAL_MAXIMUM 	0x24
#define REPORT_PHYSICAL_MINIMUM 0x34
#define REPORT_PHYSICAL_MAXIMUM 0x44
#define REPORT_USAGE_MINIMUM	0x18
#define REPORT_USAGE_MAXIMUM	0x28

#define TYPE_MAIN         0
#define TYPE_GLOBAL       1
#define TYPE_LOCAL        2

#define REPORT_UNIT				0x64
#define REPORT_INPUT			0x80
#define REPORT_OUTPUT 			0x90
#define REPORT_FEATURE			0xB0

#define REPORT_REPORT_SIZE		0x74
#define REPORT_REPORT_ID		0x84
#define REPORT_REPORT_COUNT		0x94

#define REPORT_COLLECTION		0xA0
#define REPORT_COLLECTION_END	0xC0

#define REPORT_USAGE_UNKNOWN	0x00
#define REPORT_USAGE_POINTER	0x01
#define REPORT_USAGE_MOUSE		0x02
#define REPORT_USAGE_RESERVED	0x03
#define REPORT_USAGE_JOYSTICK	0x04
#define REPORT_USAGE_GAMEPAD	0x05
#define REPORT_USAGE_KEYBOARD	0x06
#define REPORT_USAGE_KEYPAD		0x07
#define REPORT_USAGE_MULTI_AXIS	0x08
#define REPORT_USAGE_SYSTEM		0x09

#define REPORT_USAGE_X			0x30
#define REPORT_USAGE_Y			0x31
#define REPORT_USAGE_Z			0x32
#define REPORT_USAGE_Rx			0x33
#define REPORT_USAGE_Ry			0x34
#define REPORT_USAGE_Rz			0x35
#define REPORT_USAGE_WHEEL		0x38

#define REPORT_USAGE_PAGE_GENERIC	0x01
#define REPORT_USAGE_PAGE_KEYBOARD 	0x07
#define REPORT_USAGE_PAGE_LEDS		0x08
#define REPORT_USAGE_PAGE_BUTTON	0x09
#define REPORT_USAGE_PAGE_VENDOR	0xff00

#define HID_LOCAL_ITEM_TAG_USAGE         0x00

#define HID_GLOBAL_ITEM_TAG_USAGE_PAGE       0x00
#define HID_GLOBAL_ITEM_TAG_LOGICAL_MINIMUM  0x01
#define HID_GLOBAL_ITEM_TAG_LOGICAL_MAXIMUM  0x02
#define HID_GLOBAL_ITEM_TAG_PHYSICAL_MINIMUM 0x03
#define HID_GLOBAL_ITEM_TAG_PHYSICAL_MAXIMUM 0x04
#define HID_GLOBAL_ITEM_TAG_REPORT_SIZE      0x07
#define HID_GLOBAL_ITEM_TAG_REPORT_ID        0x08
#define HID_GLOBAL_ITEM_TAG_REPORT_COUNT     0x09

#define HID_MAIN_ITEM_TAG_INPUT          0x08

#define HID_ITEM_FORMAT_SHORT      0
#define HID_ITEM_FORMAT_LONG       1

#define HID_ITEM_TAG_LONG          15

#define MAX_HID_DEVICES 8

typedef struct _EndPoint
{
	unsigned char EndpointAddr;
	unsigned short MaxPacketSize;
	
	unsigned char EndpointDir : 1;
	unsigned char TOG : 1;
} EndPoint, *PEndPoint;

//hid seg struct
typedef struct _HID_SEG_STRUCT
{
	UINT8 KeyboardReportId;
	UINT8 MouseReportId;
	
	struct
	{
		UINT8 start;
		UINT8 size;
		UINT8 count;
	} HIDSeg[HID_SEG_NUM];
} HID_SEG_STRUCT;

#define HID_KEYBOARD_VAL_LEN           6
#define MAX_HID_KEYBOARD_BIT_VAL_LEN   15

typedef struct _KEYBOARD_PARSE_STRUCT
{
	UINT8     KeyboardVal[HID_KEYBOARD_VAL_LEN];
	UINT8     KeyboardBitVal[MAX_HID_KEYBOARD_BIT_VAL_LEN]; //for bit value keyboard data
} KEYBOARD_PARSE_STRUCT;

typedef struct _Interface
{
	unsigned char       InterfaceClass;
	unsigned char       InterfaceProtocol;
	
	unsigned char       EndpointCount;
	EndPoint            endpoint[MAX_ENDPOINT_COUNT];
	
	HID_SEG_STRUCT HidSegStruct;	

	KEYBOARD_PARSE_STRUCT KeyboardParseStruct;
	
} Interface, *PInterface;

typedef struct _UsbDevice
{
	unsigned char   DeviceClass;
	unsigned char   MaxPacketSize0;
	
	unsigned short  VendorID;
	unsigned short  ProductID;
	unsigned short  bcdDevice;

	unsigned char   DeviceAddress;
	unsigned char   DeviceSpeed;
	unsigned char   InterfaceCount;
	Interface       interface[MAX_INTERFACE_COUNT];

	unsigned char   HubPortNum;
} UsbDevice, *PUsbDevice;

typedef struct _HID_ITEM_INFO
{
	unsigned int format;
	UINT8 size;
	UINT8 type;
	UINT8 tag;

	union
	{
		UINT8  u8;
		INT8   s8;
		UINT16 u16;
		INT16  s16;
		UINT32 u32;
		INT32  s32;

		const UINT8 *longdata;
	} value;
	
} HID_ITEM;

typedef struct _HID_GLOBAL
{
	UINT32   usagePage;
	INT32    logicalMinimum;
	INT32    logicalMaximum;
	INT32    physicalMinimum;
	INT32    physicalMaximum;
	UINT32   unitExponent;
	UINT32   unit;
	UINT32   reportID;
	UINT32   reportSize;
	UINT32   reportCount;
} HID_GLOBAL;

#define MAX_USAGE_NUM      10

typedef struct _USAGE
{
	UINT32  usage;

	UINT8  usageLen;
}USAGE;

void resetRootHub(unsigned char i);
void initUSB_Host();
unsigned char checkRootHubConnections();

void resetHubDevices(unsigned char hubindex);
void pollHIDdevice();

#endif