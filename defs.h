#ifndef __DEFS_H__
#define __DEFS_H__
#include <stdint.h>
#include <stdbool.h>
#include "type.h"
#define ROOT_HUB_COUNT 2

#define MAX_EXHUB_PORT_COUNT 4
#define EXHUB_PORT_NONE 0xff
#define MAX_INTERFACE_COUNT 4
#define MAX_ENDPOINT_COUNT 4
#define MAX_EXHUB_LEVEL 1
#define ENDPOINT_OUT 0
#define ENDPOINT_IN 1

#define ERR_SUCCESS 0x00
#define ERR_USB_CONNECT 0x15
#define ERR_USB_DISCON 0x16
#define ERR_USB_BUF_OVER 0x17
#define ERR_USB_DISK_ERR 0x1F
#define ERR_USB_TRANSFER 0x20
#define ERR_USB_UNSUPPORT 0xFB
#define ERR_USB_UNKNOWN 0xFE

#define ROOT_DEVICE_DISCONNECT 0
#define ROOT_DEVICE_CONNECTED 1
#define ROOT_DEVICE_FAILED 2
#define ROOT_DEVICE_SUCCESS 3

/*#define DEV_TYPE_KEYBOARD   ( USB_DEV_CLASS_HID | 0x20 )
#define DEV_TYPE_MOUSE      ( USB_DEV_CLASS_HID | 0x30 )
#define DEV_TYPE_JOYSTICK      ( USB_DEV_CLASS_HID | 0x40 )
#define DEV_TYPE_GAMEPAD      ( USB_DEV_CLASS_HID | 0x50 )*/

#define HID_SEG_KEYBOARD_MODIFIER_INDEX 0
#define HID_SEG_KEYBOARD_VAL_INDEX 1
#define HID_SEG_BUTTON_INDEX 2
#define HID_SEG_X_INDEX 3
#define HID_SEG_Y_INDEX 4
#define HID_SEG_WHEEL_INDEX 5

#define REPORT_USAGE_PAGE 0x04
#define REPORT_USAGE 0x08
#define REPORT_LOCAL_MINIMUM 0x14
#define REPORT_LOCAL_MAXIMUM 0x24
#define REPORT_PHYSICAL_MINIMUM 0x34
#define REPORT_PHYSICAL_MAXIMUM 0x44
#define REPORT_USAGE_MINIMUM 0x18
#define REPORT_USAGE_MAXIMUM 0x28

#define TYPE_MAIN 0
#define TYPE_GLOBAL 1
#define TYPE_LOCAL 2

#define REPORT_UNIT 0x64
#define REPORT_INPUT 0x80
#define REPORT_OUTPUT 0x90
#define REPORT_FEATURE 0xB0

#define REPORT_REPORT_SIZE 0x74
#define REPORT_REPORT_ID 0x84
#define REPORT_REPORT_COUNT 0x94

#define REPORT_COLLECTION 0xA0
#define REPORT_COLLECTION_END 0xC0

#define REPORT_USAGE_UNKNOWN 0x00
#define REPORT_USAGE_POINTER 0x01
#define REPORT_USAGE_MOUSE 0x02
#define REPORT_USAGE_RESERVED 0x03
#define REPORT_USAGE_JOYSTICK 0x04
#define REPORT_USAGE_GAMEPAD 0x05
#define REPORT_USAGE_KEYBOARD 0x06
#define REPORT_USAGE_KEYPAD 0x07
#define REPORT_USAGE_MULTI_AXIS 0x08
#define REPORT_USAGE_SYSTEM 0x09

#define REPORT_USAGE_X 0x30
#define REPORT_USAGE_Y 0x31
#define REPORT_USAGE_Z 0x32
#define REPORT_USAGE_Rx 0x33
#define REPORT_USAGE_Ry 0x34
#define REPORT_USAGE_Rz 0x35
#define REPORT_USAGE_WHEEL 0x38

#define REPORT_USAGE_PAGE_GENERIC 0x01
#define REPORT_USAGE_PAGE_KEYBOARD 0x07
#define REPORT_USAGE_PAGE_LEDS 0x08
#define REPORT_USAGE_PAGE_BUTTON 0x09
#define REPORT_USAGE_PAGE_VENDOR 0xff00

#define HID_LOCAL_ITEM_TAG_USAGE 0x00
#define HID_LOCAL_ITEM_TAG_USAGE_MIN 0x01
#define HID_LOCAL_ITEM_TAG_USAGE_MAX 0x02

#define HID_GLOBAL_ITEM_TAG_USAGE_PAGE 0x00
#define HID_GLOBAL_ITEM_TAG_LOGICAL_MINIMUM 0x01
#define HID_GLOBAL_ITEM_TAG_LOGICAL_MAXIMUM 0x02
#define HID_GLOBAL_ITEM_TAG_PHYSICAL_MINIMUM 0x03
#define HID_GLOBAL_ITEM_TAG_PHYSICAL_MAXIMUM 0x04
#define HID_GLOBAL_ITEM_TAG_REPORT_SIZE 0x07
#define HID_GLOBAL_ITEM_TAG_REPORT_ID 0x08
#define HID_GLOBAL_ITEM_TAG_REPORT_COUNT 0x09

#define HID_MAIN_ITEM_TAG_INPUT 0x08
#define HID_MAIN_ITEM_TAG_COLLECTION_START 0x0A
#define HID_MAIN_ITEM_TAG_COLLECTION_END 0x0C

#define HID_INPUT_VARIABLE 0x02

#define HID_COLLECTION_APPLICATION 0x01

#define HID_ITEM_FORMAT_SHORT 0
#define HID_ITEM_FORMAT_LONG 1

#define HID_ITEM_TAG_LONG 15

#define MAX_HID_DEVICES 8

typedef struct _EndPoint
{
	unsigned char EndpointAddr;
	unsigned short MaxPacketSize;

	unsigned char EndpointDir : 1;
	unsigned char TOG : 1;
} EndPoint, *PEndPoint;

typedef struct _HID_GLOBAL
{
	uint8_t usagePage;
	INT8 logicalMinimum;
	INT8 logicalMaximum;
	INT8 physicalMinimum;
	INT8 physicalMaximum;
	uint8_t unitExponent;
	uint8_t unit;
	uint8_t reportID;
	uint8_t reportSize;
	uint8_t reportCount;
} HID_GLOBAL;

typedef struct _HID_LOCAL
{
	uint32_t usage;
	uint32_t usageMin;
	uint32_t usageMax;

} HID_LOCAL;

// maps a joystick control to a keyboard or mouse output

#define MAP_KEYBOARD 0
#define MAP_MOUSE 1

#define MAP_MOUSE_BUTTON1 1
#define MAP_MOUSE_BUTTON2 2
#define MAP_MOUSE_BUTTON3 3
#define MAP_MOUSE_X 4
#define MAP_MOUSE_Y 5
#define MAP_MOUSE_WHEEL 6

#define MAP_TYPE_NONE 0
#define MAP_TYPE_THRESHOLD_BELOW 1
#define MAP_TYPE_THRESHOLD_ABOVE 2
#define MAP_TYPE_SCALE 3
#define MAP_TYPE_ARRAY 4
#define MAP_TYPE_BITFIELD 5

#define JOYPRESETCOUNT 18

typedef struct JoyPreset
{
	uint8_t Number;

	uint8_t InputUsagePage;

	uint8_t InputUsage;

	// Mouse or keyboard
	uint8_t OutputChannel;

	// for keyboard, this is the HID scancode of the key associated with this control
	// for mouse, this is one of the values of MAP_MOUSE_x
	uint8_t OutputControl;

	// How this control gets interpreted - MAP_TYPE_x
	uint8_t InputType;

	// Param has different meanings depending on InputType
	uint16_t InputParam;
} JoyPreset;

// defines a mapping between a HID segment and a PS/2 event
typedef struct HID_SEG
{
	uint16_t startBit;

	// Mouse or keyboard
	uint8_t OutputChannel;

	// for keyboard, this is the HID scancode of the key associated with this control
	// for mouse, this is one of the values of MAP_MOUSE_x
	uint8_t OutputControl;

	// How this control gets interpreted - MAP_TYPE_x
	uint8_t InputType;

	// Param has different meanings depending on InputType
	uint16_t InputParam;

	uint8_t reportSize;

	//uint8_t oldValue;
	uint8_t value;#

	struct HID_SEG *next;
} HID_SEG;

#define MAX_USAGE_NUM 10

/*typedef struct HID_SEG
{

	uint8_t usage;
	uint8_t usageMin;
	uint8_t usageMax;
	HID_GLOBAL *global;

	uint8_t oldValue;
	uint8_t value;

	uint8_t inputField;

	// for joysticks, which control this is mapped to
	JoyMap *map;

	struct HID_SEG *next;
} HID_SEG;*/

typedef struct _HID_REPORT
{
	uint16_t appUsage;
	uint16_t appUsagePage;
	uint16_t length;

	bool mouseUpdated;
	bool keyboardUpdated;

	// bit map for currently pressed keys (0-256)
	uint8_t KeyboardKeyMap[32];
	uint8_t oldKeyboardKeyMap[32];

	unsigned char nextMousePacket[3];
	HID_SEG *firstHidSeg;
} HID_REPORT;

#define MAX_REPORTS 10

typedef struct _HID_REPORT_DESC
{
	bool usesReports;
	HID_REPORT *reports[MAX_REPORTS];
} HID_REPORT_DESC;

#endif