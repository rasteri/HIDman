#include "CH559.h"
#include "USBHost.h"
#include "util.h"
#include "ps2.h"
#include "uart.h"
#include <string.h>
#include <stdbool.h>
#include "protocol.h"
SBIT(LED, 0x90, 6);

typedef const unsigned char __code *PUINT8C;

__code unsigned char GetDeviceDescriptorRequest[] = 		{USB_REQ_TYP_IN, USB_GET_DESCRIPTOR, 0, USB_DESCR_TYP_DEVICE , 0, 0, sizeof(USB_DEV_DESCR), 0};
__code unsigned char GetConfigurationDescriptorRequest[] = 	{USB_REQ_TYP_IN, USB_GET_DESCRIPTOR, 0, USB_DESCR_TYP_CONFIG, 0, 0, sizeof(USB_DEV_DESCR), 0};
__code unsigned char GetInterfaceDescriptorRequest[] = 		{USB_REQ_TYP_IN | USB_REQ_RECIP_INTERF, USB_GET_DESCRIPTOR, 0, USB_DESCR_TYP_INTERF, 0, 0, sizeof(USB_ITF_DESCR), 0};
__code unsigned char SetUSBAddressRequest[] = 				{USB_REQ_TYP_OUT, USB_SET_ADDRESS, USB_DEVICE_ADDR, 0, 0, 0, 0, 0};
__code unsigned char GetDeviceStringRequest[] = 			{USB_REQ_TYP_IN, USB_GET_DESCRIPTOR, 2, 3, 9, 4, 2, 4};	//todo change language
__code unsigned char SetupSetUsbConfig[] = { USB_REQ_TYP_OUT, USB_SET_CONFIGURATION, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

__code unsigned char  SetHIDIdleRequest[] = {USB_REQ_TYP_CLASS | USB_REQ_RECIP_INTERF, HID_SET_IDLE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
__code unsigned char  GetHIDReport[] = {USB_REQ_TYP_IN | USB_REQ_RECIP_INTERF, USB_GET_DESCRIPTOR, 0x00, USB_DESCR_TYP_REPORT, 0 /*interface*/, 0x00, 0xff, 0x00};

__at(0x0000) unsigned char __xdata RxBuffer[MAX_PACKET_SIZE];
__at(0x0100) unsigned char __xdata TxBuffer[MAX_PACKET_SIZE];

__xdata uint8_t endpoint0Size;	//todo rly global?
unsigned char SetPort = 0;	//todo really global?

#define RECEIVE_BUFFER_LEN    512
__xdata unsigned char receiveDataBuffer[RECEIVE_BUFFER_LEN];

struct _RootHubDevice
{
	unsigned char status;
	unsigned char address;
	unsigned char speed;
} __xdata rootHubDevice[ROOT_HUB_COUNT];

void disableRootHubPort(unsigned char index)
{
	rootHubDevice[index].status = ROOT_DEVICE_DISCONNECT;
	rootHubDevice[index].address = 0;
	if (index)
	UHUB1_CTRL = 0;
	else
	UHUB0_CTRL = 0;
}

void initUSB_Host()
{
	IE_USB = 0;
	USB_CTRL = bUC_HOST_MODE;
	USB_DEV_AD = 0x00;
	UH_EP_MOD = bUH_EP_TX_EN | bUH_EP_RX_EN ;
	UH_RX_DMA = 0x0000;
	UH_TX_DMA = 0x0001;
	UH_RX_CTRL = 0x00;
	UH_TX_CTRL = 0x00;
	USB_CTRL = bUC_HOST_MODE | bUC_INT_BUSY | bUC_DMA_EN;
	UH_SETUP = bUH_SOF_EN;
	USB_INT_FG = 0xFF;

	disableRootHubPort(0);
	disableRootHubPort(1);
	USB_INT_EN = bUIE_TRANSFER | bUIE_DETECT;
}

void setHostUsbAddr(unsigned char addr)
{
	USB_DEV_AD = USB_DEV_AD & bUDA_GP_BIT | addr & 0x7F;
}

void setUsbSpeed(unsigned char fullSpeed)
{
	if (fullSpeed)
	{
		USB_CTRL &= ~ bUC_LOW_SPEED;
		UH_SETUP &= ~ bUH_PRE_PID_EN;
	}
	else
		USB_CTRL |= bUC_LOW_SPEED;
}

void resetRootHubPort(unsigned char rootHubIndex)
{
	endpoint0Size = DEFAULT_ENDP0_SIZE; //todo what's that?                    
	setHostUsbAddr(0);
	setUsbSpeed(1);
	 if (rootHubIndex == 0)    
    {
        UHUB0_CTRL = UHUB0_CTRL & ~ bUH_LOW_SPEED | bUH_BUS_RESET;
        delay(15);
        UHUB0_CTRL = UHUB0_CTRL & ~ bUH_BUS_RESET;
    }
    else if (rootHubIndex == 1)
    {
        UHUB1_CTRL = UHUB1_CTRL & ~ bUH_LOW_SPEED | bUH_BUS_RESET;
        delay(15);
        UHUB1_CTRL = UHUB1_CTRL & ~ bUH_BUS_RESET;
    }
	delayUs(250);
	UIF_DETECT = 0; //todo test if redundant                                       
}

unsigned char enableRootHubPort(unsigned char rootHubIndex)
{
    if ( rootHubDevice[ rootHubIndex ].status < 1 )
    {
        rootHubDevice[ rootHubIndex ].status = 1;
    }
	if (rootHubIndex == 0)
	{
		if (USB_HUB_ST & bUHS_H0_ATTACH)
		{
			if ((UHUB0_CTRL & bUH_PORT_EN) == 0x00)
			{
				if (USB_HUB_ST & bUHS_DM_LEVEL)
				{
					rootHubDevice[rootHubIndex].speed = 0;
					UHUB0_CTRL |= bUH_LOW_SPEED;
				}
				else rootHubDevice[rootHubIndex].speed = 1;
			}
			UHUB0_CTRL |= bUH_PORT_EN;
			return ERR_SUCCESS;
		}
	}
	else if (rootHubIndex == 1)
	{
		if (USB_HUB_ST & bUHS_H1_ATTACH)
		{
			if ((UHUB1_CTRL & bUH_PORT_EN ) == 0x00)
			{
				if (USB_HUB_ST & bUHS_HM_LEVEL)
				{
					rootHubDevice[rootHubIndex].speed = 0;
					UHUB1_CTRL |= bUH_LOW_SPEED;
				}
				else rootHubDevice[rootHubIndex].speed = 1;
			}
			UHUB1_CTRL |= bUH_PORT_EN;
			return ERR_SUCCESS;
		}
	}
	return ERR_USB_DISCON;
}

void selectHubPort(unsigned char rootHubIndex, unsigned char HubPortIndex)
{
	unsigned char temp = HubPortIndex;
        setHostUsbAddr(rootHubDevice[rootHubIndex].address); //todo ever != 0
        setUsbSpeed(rootHubDevice[rootHubIndex].speed); //isn't that set before?
}

unsigned char hostTransfer(unsigned char endp_pid, unsigned char tog, unsigned short timeout )
{
    unsigned short retries;
    unsigned char	r;
    unsigned short	i;
    UH_RX_CTRL = tog;
    UH_TX_CTRL = tog;
    retries = 0;
    do
    {
        UH_EP_PID = endp_pid;                               
        UIF_TRANSFER = 0;            
        for (i = 200; i != 0 && UIF_TRANSFER == 0; i--)
            delayUs(1);
        UH_EP_PID = 0x00;                                         
        if ( UIF_TRANSFER == 0 )
        {
            return ERR_USB_UNKNOWN;
        }
        if ( UIF_TRANSFER )                                    
        {
            if ( U_TOG_OK )
            {
                return( ERR_SUCCESS );
            }
            r = USB_INT_ST & MASK_UIS_H_RES;               
            if ( r == USB_PID_STALL )
            {
                return( r | ERR_USB_TRANSFER );
            }
            if ( r == USB_PID_NAK )
            {
                if ( timeout == 0 )
                {
                    return( r | ERR_USB_TRANSFER );
                }
                if ( timeout < 0xFFFF )
                {
                    timeout --;
                }
                retries--;
            }
            else switch ( endp_pid >> 4 )	//todo no return.. compare to other guy
                {
                case USB_PID_SETUP:
                case USB_PID_OUT:
                    if ( U_TOG_OK )
                    {
                        return( ERR_SUCCESS );
                    }
                    if ( r == USB_PID_ACK )
                    {
                        return( ERR_SUCCESS );
                    }
                    if ( r == USB_PID_STALL || r == USB_PID_NAK )
                    {
                        return( r | ERR_USB_TRANSFER );
                    }
                    if ( r )
                    {
                        return( r | ERR_USB_TRANSFER );          
                    }
                    break;                                    
                case USB_PID_IN:
                    if ( U_TOG_OK )
                    {
                        return( ERR_SUCCESS );
                    }
                    if ( tog ? r == USB_PID_DATA1 : r == USB_PID_DATA0 )
                    {
                        return( ERR_SUCCESS );
                    }
                    if ( r == USB_PID_STALL || r == USB_PID_NAK )
                    {
                        return( r | ERR_USB_TRANSFER );
                    }
                    if ( r == USB_PID_DATA0 && r == USB_PID_DATA1 )
                    {
                    }                                        
                    else if ( r )
                    {
                        return( r | ERR_USB_TRANSFER );     
                    }
                    break;                                     
                default:
                    return( ERR_USB_UNKNOWN );                  
                    break;
                }
        }
        else                                                    
        {
            USB_INT_FG = 0xFF;                               
        }
        delayUs(15);
    }
    while ( ++retries < 200 );
    return( ERR_USB_TRANSFER );                              
}

//todo request buffer
unsigned char hostCtrlTransfer(unsigned char __xdata *DataBuf, unsigned short *RetLen, unsigned short maxLenght)
{
	unsigned char temp = maxLenght;
	unsigned short RemLen;
	unsigned char s, RxLen, i;
	unsigned char __xdata *pBuf;
	unsigned short *pLen;
	DEBUG_OUT("hostCtrlTransfer\n");
	PXUSB_SETUP_REQ pSetupReq = ((PXUSB_SETUP_REQ)TxBuffer);
	pBuf = DataBuf;
	pLen = RetLen;
	delayUs(200);
	if (pLen)
		*pLen = 0;
	UH_TX_LEN = sizeof(USB_SETUP_REQ);
	s = hostTransfer((unsigned char)(USB_PID_SETUP << 4), 0, 10000);
	if (s != ERR_SUCCESS)
		return (s);
	UH_RX_CTRL = UH_TX_CTRL = bUH_R_TOG | bUH_R_AUTO_TOG | bUH_T_TOG | bUH_T_AUTO_TOG;
	UH_TX_LEN = 0x01;
	RemLen = (pSetupReq->wLengthH << 8) | (pSetupReq->wLengthL);
	if (RemLen && pBuf)
	{ 
		if (pSetupReq->bRequestType & USB_REQ_TYP_IN)
		{
			DEBUG_OUT("Remaining bytes to read %d\n", RemLen);
			while (RemLen)
			{
				delayUs(300);
				s = hostTransfer((unsigned char)(USB_PID_IN << 4), UH_RX_CTRL, 10000); 
				if (s != ERR_SUCCESS)
					return (s);
				RxLen = USB_RX_LEN < RemLen ? USB_RX_LEN : RemLen;
				RemLen -= RxLen;
				if (pLen)
					*pLen += RxLen;
				for(i = 0; i < RxLen; i++)
					pBuf[i] = RxBuffer[i];
				pBuf += RxLen;
				DEBUG_OUT("Received %i bytes\n", (uint16_t)USB_RX_LEN);
				if (USB_RX_LEN == 0 || (USB_RX_LEN < endpoint0Size ))
					break; 
			}
			UH_TX_LEN = 0x00;
		}
		else
		{
			DEBUG_OUT("Remaining bytes to write %i", RemLen);
			//todo rework this TxBuffer overwritten
			while (RemLen)
			{
				delayUs(200);
				UH_TX_LEN = RemLen >= endpoint0Size ? endpoint0Size : RemLen;
				//memcpy(TxBuffer, pBuf, UH_TX_LEN);
				pBuf += UH_TX_LEN;
				if (pBuf[1] == 0x09)
				{
					SetPort = SetPort ^ 1 ? 1 : 0;
					*pBuf = SetPort;

					DEBUG_OUT("SET_PORT  %02X  %02X ", *pBuf, SetPort);
				}
				DEBUG_OUT("Sending %i bytes\n", (uint16_t)UH_TX_LEN);
				s = hostTransfer(USB_PID_OUT << 4, UH_TX_CTRL, 10000);
				if (s != ERR_SUCCESS)
					return (s);
				RemLen -= UH_TX_LEN;
				if (pLen)
					*pLen += UH_TX_LEN;
			}
		}
	}
	delayUs(200);
	s = hostTransfer((UH_TX_LEN ? USB_PID_IN << 4 : USB_PID_OUT << 4), bUH_R_TOG | bUH_T_TOG, 10000);
	if (s != ERR_SUCCESS)
		return (s);
	if (UH_TX_LEN == 0)
		return (ERR_SUCCESS);
	if (USB_RX_LEN == 0)
		return (ERR_SUCCESS);
	return (ERR_USB_BUF_OVER);
}

void fillTxBuffer(PUINT8C data, unsigned char len)
{
	unsigned char i;
	DEBUG_OUT("fillTxBuffer %i bytes\n", len);
	for(i = 0; i < len; i++)
		TxBuffer[i] = data[i];
	DEBUG_OUT("fillTxBuffer done\n", len);
}

unsigned char getDeviceDescriptor()
{
    unsigned char s;
    unsigned short len;
    endpoint0Size = DEFAULT_ENDP0_SIZE;		//TODO again?
	DEBUG_OUT("getDeviceDescriptor\n");
	fillTxBuffer(GetDeviceDescriptorRequest, sizeof(GetDeviceDescriptorRequest));
    s = hostCtrlTransfer(receiveDataBuffer, &len, RECEIVE_BUFFER_LEN);          
    if (s != ERR_SUCCESS)
        return s;

	DEBUG_OUT("Device descriptor request sent successfully\n");
    endpoint0Size = ((PXUSB_DEV_DESCR)receiveDataBuffer)->bMaxPacketSize0;
    if (len < ((PUSB_SETUP_REQ)GetDeviceDescriptorRequest)->wLengthL)
    {
		DEBUG_OUT("Received packet is smaller than expected\n")
        return ERR_USB_BUF_OVER;                
    }
    return ERR_SUCCESS;
}

unsigned char setUsbAddress(unsigned char addr)
{
    unsigned char s;
	PXUSB_SETUP_REQ pSetupReq = ((PXUSB_SETUP_REQ)TxBuffer);
    fillTxBuffer(SetUSBAddressRequest, sizeof(SetUSBAddressRequest));
    pSetupReq->wValueL = addr;          
    s = hostCtrlTransfer(0, 0, 0);   
    if (s != ERR_SUCCESS) return s;
    DEBUG_OUT( "SetAddress: %i\n" , addr);
    setHostUsbAddr(addr);
    delay(100);         
    return ERR_SUCCESS;
}

unsigned char setUsbConfig( unsigned char cfg )
{
	PXUSB_SETUP_REQ pSetupReq = ((PXUSB_SETUP_REQ)TxBuffer);
    fillTxBuffer(SetupSetUsbConfig, sizeof(SetupSetUsbConfig));
    pSetupReq->wValueL = cfg;                          
    return( hostCtrlTransfer(0, 0, 0) );            
}

unsigned char getDeviceString()
{
    fillTxBuffer(GetDeviceStringRequest, sizeof(GetDeviceStringRequest));                         
    return hostCtrlTransfer(receiveDataBuffer, 0, RECEIVE_BUFFER_LEN);
}

char convertStringDescriptor(unsigned char __xdata *usbBuffer, unsigned char __xdata *strBuffer, unsigned short bufferLength, unsigned char index)
{
	//supports using source as target buffer
	unsigned char i = 0, len = (usbBuffer[0] - 2) >> 1;
	if(usbBuffer[1] != 3) return 0;	//check if device string
	for(; (i < len) && (i < bufferLength - 1); i++)
		if(usbBuffer[2 + 1 + (i << 1)])
			strBuffer[i] = '?';
		else
			strBuffer[i] = usbBuffer[2 + (i << 1)];
	strBuffer[i] = 0;
	//sendProtocolMSG(MSG_TYPE_DEVICE_STRING,(unsigned short)len, index+1, 0x34, 0x56, strBuffer);
	return 1;
}

void DEBUG_OUT_USB_BUFFER(unsigned char __xdata *usbBuffer)
{
	int i;
	for(i = 0; i < usbBuffer[0]; i++)
	{
		DEBUG_OUT("0x%02X ", (uint16_t)(usbBuffer[i]));
	}
	DEBUG_OUT("\n");
}

unsigned char getConfigurationDescriptor()
{
    unsigned char s;
    unsigned short len, total;
	fillTxBuffer(GetConfigurationDescriptorRequest, sizeof(GetConfigurationDescriptorRequest));

    s = hostCtrlTransfer(receiveDataBuffer, &len, RECEIVE_BUFFER_LEN);             
    if(s != ERR_SUCCESS)
        return s;
	//todo didnt send reqest completely
    if(len < ((PUSB_SETUP_REQ)GetConfigurationDescriptorRequest)->wLengthL)
        return ERR_USB_BUF_OVER;

	//todo fix 16bits
    total = ((PXUSB_CFG_DESCR)receiveDataBuffer)->wTotalLengthL + (((PXUSB_CFG_DESCR)receiveDataBuffer)->wTotalLengthH << 8);
	fillTxBuffer(GetConfigurationDescriptorRequest, sizeof(GetConfigurationDescriptorRequest));
    ((PUSB_SETUP_REQ)TxBuffer)->wLengthL = (unsigned char)(total & 255);
    ((PUSB_SETUP_REQ)TxBuffer)->wLengthH = (unsigned char)(total >> 8);
    s = hostCtrlTransfer(receiveDataBuffer, &len, RECEIVE_BUFFER_LEN);             
    if(s != ERR_SUCCESS)
        return s;
	//todo 16bit and fix received length check
	//if (len < total || len < ((PXUSB_CFG_DESCR)receiveDataBuffer)->wTotalLengthL)
    //    return( ERR_USB_BUF_OVER );                             
    return ERR_SUCCESS;
}

unsigned char getInterfaceDescriptor(unsigned char index)
{
	unsigned char temp = index;
	unsigned char s;
    unsigned short len;
	fillTxBuffer(GetInterfaceDescriptorRequest, sizeof(GetInterfaceDescriptorRequest));
    s = hostCtrlTransfer(receiveDataBuffer, &len, RECEIVE_BUFFER_LEN);             
	return s;                          
}



#define MAX_HID_DEVICES 8
struct 
{
	unsigned char connected;
	unsigned char rootHub;
	unsigned char interface;
	unsigned char endPoint;
	unsigned long type;
}  __xdata HIDdevice[MAX_HID_DEVICES];

struct 
{
    unsigned long idVendorL;
    unsigned long idVendorH;
    unsigned long idProductL;
    unsigned long idProductH;
}  __xdata VendorProductID[2];

void resetHubDevices(unsigned char hubindex)
{
	 __xdata unsigned char hiddevice;
    VendorProductID[hubindex].idVendorL = 0;
    VendorProductID[hubindex].idVendorH = 0;
    VendorProductID[hubindex].idProductL = 0;
    VendorProductID[hubindex].idProductH = 0;
	for (hiddevice = 0; hiddevice < MAX_HID_DEVICES; hiddevice++)
	{
	if(HIDdevice[hiddevice].rootHub == hubindex){
	HIDdevice[hiddevice].connected  = 0;
	HIDdevice[hiddevice].rootHub  = 0;
	HIDdevice[hiddevice].interface  = 0;
	HIDdevice[hiddevice].endPoint  = 0;
	HIDdevice[hiddevice].type  = 0;
	}
	}
}

void pollHIDdevice()
{
	 __xdata unsigned char s, hiddevice, len;
	for (hiddevice = 0; hiddevice < MAX_HID_DEVICES; hiddevice++)
	{
		if(HIDdevice[hiddevice].connected){
		selectHubPort(HIDdevice[hiddevice].rootHub, 0);
		s = hostTransfer( USB_PID_IN << 4 | HIDdevice[hiddevice].endPoint & 0x7F, HIDdevice[hiddevice].endPoint & 0x80 ? bUH_R_TOG | bUH_T_TOG : 0, 0 );
		if ( s == ERR_SUCCESS )
   		{
    		HIDdevice[hiddevice].endPoint ^= 0x80;
			len = USB_RX_LEN;
			if ( len )
			{		
				LED = !LED;	
				//DEBUG_OUT("HID %lu, %i data %i : ", HIDdevice[hiddevice].type, hiddevice, HIDdevice[hiddevice].endPoint & 0x7F);
				/*DEBUG_OUT("MSG %x\n
				len %x\n
				HIDDEVICETYPE %x\n
				hiddevice %x\n
				hiddevice endpoint %x\n
				rxbuffer 0: %x %x %x %x %x %x %x %x\n
				idvendorl %x\n
				idvendorh %x\n
				idproductl %x\n
				idproducth %x\n
				", 
				MSG_TYPE_DEVICE_POLL,
				len, 
				HIDdevice[hiddevice].type, 
				hiddevice, 
				HIDdevice[hiddevice].endPoint & 0x7F, 
				RxBuffer[0], RxBuffer[1], RxBuffer[2], RxBuffer[3], RxBuffer[4], RxBuffer[5], RxBuffer[6], RxBuffer[7],
				VendorProductID[HIDdevice[hiddevice].rootHub].idVendorL,
				VendorProductID[HIDdevice[hiddevice].rootHub].idVendorH,
				VendorProductID[HIDdevice[hiddevice].rootHub].idProductL,
				VendorProductID[HIDdevice[hiddevice].rootHub].idProductH);*/

				SendHIDPS2(len, HIDdevice[hiddevice].type, RxBuffer);
				//sendHidPollMSG(MSG_TYPE_DEVICE_POLL,len, HIDdevice[hiddevice].type, hiddevice, HIDdevice[hiddevice].endPoint & 0x7F, RxBuffer,VendorProductID[HIDdevice[hiddevice].rootHub].idVendorL,VendorProductID[HIDdevice[hiddevice].rootHub].idVendorH,VendorProductID[HIDdevice[hiddevice].rootHub].idProductL,VendorProductID[HIDdevice[hiddevice].rootHub].idProductH);
			}
		}
		}
	}
}


void parseHIDDeviceReport(unsigned char __xdata *report, unsigned short length, unsigned char CurrentDevive)
{
	unsigned short i = 0;
	unsigned char level = 0;
	unsigned char isUsageSet = 0;
	while(i < length)
	{
		unsigned char j;
		unsigned char id = report[i] & 0b11111100;
		unsigned char size = report[i] & 0b00000011;
		unsigned long data = 0;
		if(size == 3) size++;
		for(j = 0; j < size; j++)
			data |= ((unsigned long)report[i + 1 + j]) << (j * 8);
		for(j = 0; j < level - (id == REPORT_COLLECTION_END ? 1 : 0); j++)
			DEBUG_OUT("    ");
		switch(id)
		{
			case REPORT_USAGE_PAGE:	//todo clean up defines (case)
			{
				unsigned long vd = data < REPORT_USAGE_PAGE_VENDOR ? data : REPORT_USAGE_PAGE_VENDOR;
				DEBUG_OUT("Usage page ");
				switch(vd)
				{
					case REPORT_USAGE_PAGE_LEDS:
						DEBUG_OUT("LEDs");
					break;
					case REPORT_USAGE_PAGE_KEYBOARD:
						DEBUG_OUT("Keyboard/Keypad");
					break;
					case REPORT_USAGE_PAGE_BUTTON:
						DEBUG_OUT("Button");
					break;
					case REPORT_USAGE_PAGE_GENERIC:
						DEBUG_OUT("generic desktop controls");
					break;
					case REPORT_USAGE_PAGE_VENDOR:
						DEBUG_OUT("vendor defined 0x%04lx", data);
					break;
					default:
						DEBUG_OUT("unknown 0x%02lx", data);
				}
				DEBUG_OUT("\n");
			}
			break;
			case REPORT_USAGE:
				if (!isUsageSet){
					HIDdevice[CurrentDevive].type = data;
					isUsageSet = 1;
				}
				DEBUG_OUT("Usage ");
				switch(data)
				{
					case REPORT_USAGE_UNKNOWN:
						DEBUG_OUT("Unknown");
					break;
					case REPORT_USAGE_POINTER:
						DEBUG_OUT("Pointer");
					break;
					case REPORT_USAGE_MOUSE:
						DEBUG_OUT("Mouse");
					break;
					case REPORT_USAGE_RESERVED:
						DEBUG_OUT("Reserved");
					break;
					case REPORT_USAGE_JOYSTICK:
						DEBUG_OUT("Joystick");
					break;
					case REPORT_USAGE_GAMEPAD:
						DEBUG_OUT("Gamepad");
					break;
					case REPORT_USAGE_KEYBOARD:
						DEBUG_OUT("Keyboard");
					break;
					case REPORT_USAGE_KEYPAD:
						DEBUG_OUT("Keypad");
					break;
					case REPORT_USAGE_MULTI_AXIS:
						DEBUG_OUT("Multi-Axis controller");
					break;
					case REPORT_USAGE_SYSTEM:
						DEBUG_OUT("Tablet system controls");
					break;

					case REPORT_USAGE_X:
						DEBUG_OUT("X");
					break;
					case REPORT_USAGE_Y:
						DEBUG_OUT("Y");
					break;
					case REPORT_USAGE_Z:
						DEBUG_OUT("Z");
					break;
					case REPORT_USAGE_WHEEL:
						DEBUG_OUT("Wheel");
					break;
					default:
						DEBUG_OUT("unknown 0x%02lx", data);
				}
				DEBUG_OUT("\n");
			break;
			case REPORT_LOCAL_MINIMUM:
				DEBUG_OUT("Logical min %lu\n", data);
			break;
			case REPORT_LOCAL_MAXIMUM:
				DEBUG_OUT("Logical max %lu\n", data);
			break;
			case REPORT_PHYSICAL_MINIMUM:
				DEBUG_OUT("Physical min %lu\n", data);
			break;
			case REPORT_PHYSICAL_MAXIMUM:
				DEBUG_OUT("Physical max %lu\n", data);
			break;
			case REPORT_USAGE_MINIMUM:
				DEBUG_OUT("Physical min %lu\n", data);
			break;
			case REPORT_USAGE_MAXIMUM:
				DEBUG_OUT("Physical max %lu\n", data);
			break;
			case REPORT_COLLECTION:
				DEBUG_OUT("Collection start %lu\n", data);
				level++;
			break;
			case REPORT_COLLECTION_END:
				DEBUG_OUT("Collection end %lu\n", data);
				level--;
			break;
			case REPORT_UNIT:
				DEBUG_OUT("Unit 0x%02lx\n", data);
			break;
			case REPORT_INPUT:
				DEBUG_OUT("Input 0x%02lx\n", data);
			break;
			case REPORT_OUTPUT:
				DEBUG_OUT("Output 0x%02lx\n", data);
			break;
			case REPORT_FEATURE:
				DEBUG_OUT("Feature 0x%02lx\n", data);
			break;
			case REPORT_REPORT_SIZE:
				DEBUG_OUT("Report size %lu\n", data);
			break;
			case REPORT_REPORT_ID:
				DEBUG_OUT("Report ID %lu\n", data);
			break;
			case REPORT_REPORT_COUNT:
				DEBUG_OUT("Report count %lu\n", data);
			break;
			default:
				DEBUG_OUT("Unknown HID report identifier: 0x%02x (%i bytes) data: 0x%02lx\n", id, size, data);
		};
		i += size + 1;
	}
}

unsigned char getHIDDeviceReport(unsigned char CurrentDevive)
{
 	unsigned char s;
	unsigned short len, i, reportLen = RECEIVE_BUFFER_LEN;
	DEBUG_OUT("Requesting report from interface %i\n", HIDdevice[CurrentDevive].interface);

	fillTxBuffer(SetHIDIdleRequest, sizeof(SetHIDIdleRequest));
	((PXUSB_SETUP_REQ)TxBuffer)->wIndexL = HIDdevice[CurrentDevive].interface;	
	s = hostCtrlTransfer(receiveDataBuffer, &len, 0);
	
	//todo really dont care if successful? 8bitdo faild here
	//if(s != ERR_SUCCESS)
	//	return s;

	fillTxBuffer(GetHIDReport, sizeof(GetHIDReport));
	((PXUSB_SETUP_REQ)TxBuffer)->wIndexL = HIDdevice[CurrentDevive].interface;
	((PXUSB_SETUP_REQ)TxBuffer)->wLengthL = (unsigned char)(reportLen & 255); 
	((PXUSB_SETUP_REQ)TxBuffer)->wLengthH = (unsigned char)(reportLen >> 8);
	s = hostCtrlTransfer(receiveDataBuffer, &len, RECEIVE_BUFFER_LEN);
	if(s != ERR_SUCCESS)
		return s;
	
	for (i = 0; i < len; i++)
	{
		DEBUG_OUT("0x%02X ", receiveDataBuffer[i]);
	}
	DEBUG_OUT("\n");
	//sendProtocolMSG(MSG_TYPE_HID_INFO, len, CurrentDevive, HIDdevice[CurrentDevive].interface, HIDdevice[CurrentDevive].rootHub, receiveDataBuffer);
	parseHIDDeviceReport(receiveDataBuffer, len, CurrentDevive);
	return (ERR_SUCCESS);
}

void readInterface(unsigned char rootHubIndex, PXUSB_ITF_DESCR interface)
{
	unsigned char temp = rootHubIndex;
	DEBUG_OUT("Interface %d\n", interface->bInterfaceNumber);
	DEBUG_OUT("  Class %d\n", interface->bInterfaceClass);
	DEBUG_OUT("  Sub Class %d\n", interface->bInterfaceSubClass);
	DEBUG_OUT("  Interface Protocol %d\n", interface->bInterfaceProtocol);
}

void readHIDInterface(PXUSB_ITF_DESCR interface, PXUSB_HID_DESCR descriptor)
{
	DEBUG_OUT("HID at Interface %d\n", interface->bInterfaceNumber);
	DEBUG_OUT("  USB %d.%d%d\n", (descriptor->bcdHIDH & 15), (descriptor->bcdHIDL >> 4), (descriptor->bcdHIDL & 15));
	DEBUG_OUT("  Country code 0x%02X\n", descriptor->bCountryCode);
	DEBUG_OUT("  TypeX 0x%02X\n", descriptor->bDescriptorTypeX);
}

void readEndpoint()
{
}

unsigned char initializeRootHubConnection(unsigned char rootHubIndex)
{
	unsigned char retry, i, s = ERR_SUCCESS, cfg, dv_cls, addr;
	unsigned char HIDDevice = 0;

	for(retry = 0; retry < 10; retry++) //todo test fewer retries
	{
	delay( 100 );
		delay(100); //todo test lower delay
		resetHubDevices(rootHubIndex);
		resetRootHubPort(rootHubIndex);                      
		for (i = 0; i < 100; i++) //todo test fewer retries
		{
			delay(1);
			if (enableRootHubPort(rootHubIndex) == ERR_SUCCESS)  
				break;
		}
		if (i == 100)                                              
		{
			disableRootHubPort(rootHubIndex);
			DEBUG_OUT("Failed to enable root hub port %i\n", rootHubIndex);
			continue;
		}

		selectHubPort(rootHubIndex, 0);
		DEBUG_OUT("root hub port %i enabled\n", rootHubIndex);
		s = getDeviceDescriptor();
                              
		if ( s == ERR_SUCCESS )
		{
			dv_cls = ((PXUSB_DEV_DESCR)receiveDataBuffer)->bDeviceClass;
			DEBUG_OUT( "Device class %i\n", dv_cls);
			DEBUG_OUT( "Max packet size %i\n", ((PXUSB_DEV_DESCR)receiveDataBuffer)->bMaxPacketSize0);
    		VendorProductID[rootHubIndex].idVendorL = ((PXUSB_DEV_DESCR)receiveDataBuffer)->idVendorL;
    		VendorProductID[rootHubIndex].idVendorH = ((PXUSB_DEV_DESCR)receiveDataBuffer)->idVendorH;
    		VendorProductID[rootHubIndex].idProductL = ((PXUSB_DEV_DESCR)receiveDataBuffer)->idProductL;
    		VendorProductID[rootHubIndex].idProductH = ((PXUSB_DEV_DESCR)receiveDataBuffer)->idProductH;
			DEBUG_OUT_USB_BUFFER(receiveDataBuffer);
			addr = rootHubIndex + ((PUSB_SETUP_REQ)SetUSBAddressRequest)->wValueL; //todo wValue always 2.. does another id work?
			s = setUsbAddress(addr);
			if ( s == ERR_SUCCESS )
			{
				rootHubDevice[rootHubIndex].address = addr;
				s = getDeviceString();
				{
					DEBUG_OUT_USB_BUFFER(receiveDataBuffer);
					if(convertStringDescriptor(receiveDataBuffer, receiveDataBuffer, RECEIVE_BUFFER_LEN,rootHubIndex))
					{
						DEBUG_OUT("Device String: %s\n", receiveDataBuffer);
					}
					s = getConfigurationDescriptor();
					if ( s == ERR_SUCCESS )
					{
						//sendProtocolMSG(MSG_TYPE_DEVICE_INFO, (receiveDataBuffer[2] + (receiveDataBuffer[3] << 8)), addr, rootHubIndex+1, 0xAA, receiveDataBuffer);
						unsigned short i, total;
						unsigned char __xdata temp[512];
						PXUSB_ITF_DESCR currentInterface = 0;
						int interfaces;
						//DEBUG_OUT_USB_BUFFER(receiveDataBuffer);
						for(i = 0; i < receiveDataBuffer[2] + (receiveDataBuffer[3] << 8); i++)
						{
							DEBUG_OUT("0x%02X ", (uint16_t)(receiveDataBuffer[i]));
						}
						DEBUG_OUT("\n");

						cfg = ((PXUSB_CFG_DESCR)receiveDataBuffer)->bConfigurationValue;
						DEBUG_OUT("Configuration value: %d\n", cfg);

						interfaces = ((PXUSB_CFG_DESCR_LONG)receiveDataBuffer)->cfg_descr.bNumInterfaces;
						DEBUG_OUT("Interface count: %d\n", interfaces);

    					s = setUsbConfig( cfg ); 
						//parse descriptors
						total = ((PXUSB_CFG_DESCR)receiveDataBuffer)->wTotalLengthL + (((PXUSB_CFG_DESCR)receiveDataBuffer)->wTotalLengthH << 8);
						for(i = 0; i < total; i++)
							temp[i] = receiveDataBuffer[i];
						i = ((PXUSB_CFG_DESCR)receiveDataBuffer)->bLength;
						while(i < total)
						{
							unsigned char __xdata *desc = &(temp[i]);
							switch(desc[1])
							{
								case USB_DESCR_TYP_INTERF:
									DEBUG_OUT("Interface descriptor found\n", desc[1]);
									//DEBUG_OUT_USB_BUFFER(desc);
									currentInterface = ((PXUSB_ITF_DESCR)desc);
									readInterface(rootHubIndex, currentInterface);
									break;
								case USB_DESCR_TYP_ENDP:
									DEBUG_OUT("Endpoint descriptor found\n", desc[1]);
									DEBUG_OUT_USB_BUFFER(desc);
									if(currentInterface->bInterfaceClass == USB_DEV_CLASS_HID)
									{
										PXUSB_ENDP_DESCR d = (PXUSB_ENDP_DESCR)desc;
										if(d->bEndpointAddress & 0x80){
											unsigned char hiddevice;
											for (hiddevice = 0; hiddevice < MAX_HID_DEVICES; hiddevice++)
											{
												if(HIDdevice[hiddevice].connected == 0)break;
											}
											DEBUG_OUT("Connected device at position: %i\n", hiddevice);
											HIDdevice[hiddevice].endPoint = d->bEndpointAddress;
											HIDdevice[hiddevice].connected = 1;										
											HIDdevice[hiddevice].interface = currentInterface->bInterfaceNumber;
											HIDdevice[hiddevice].rootHub = rootHubIndex;
											DEBUG_OUT("Got endpoint for the HIDdevice 0x%02x\n", HIDdevice[hiddevice].endPoint);
											getHIDDeviceReport(hiddevice);
										}
									}
									break;
								case USB_DESCR_TYP_HID:
									DEBUG_OUT("HID descriptor found\n", desc[1]);
									//DEBUG_OUT_USB_BUFFER(desc);
									if(currentInterface == 0) break;
									readHIDInterface(currentInterface, (PXUSB_HID_DESCR)desc);
									break;
								case USB_DESCR_TYP_CS_INTF:
									DEBUG_OUT("Class specific header descriptor found\n", desc[1]);
									DEBUG_OUT_USB_BUFFER(desc);
									//if(currentInterface == 0) break;
									//readHIDInterface(currentInterface, (PXUSB_HID_DESCR)desc);
									break;
								case USB_DESCR_TYP_CS_ENDP:
									DEBUG_OUT("Class specific endpoint descriptor found\n", desc[1]);
									DEBUG_OUT_USB_BUFFER(desc);
									//if(currentInterface == 0) break;
									//readHIDInterface(currentInterface, (PXUSB_HID_DESCR)desc);
									break;
								default:
									DEBUG_OUT("Unexpected descriptor type: %02X\n", desc[1]);
									DEBUG_OUT_USB_BUFFER(desc);
							}
							i += desc[0];
						}
						return ERR_SUCCESS;
					}			
				}
			}
		}
		DEBUG_OUT( "Error = %02X\n", s);
		//sendProtocolMSG(MSG_TYPE_ERROR,0, rootHubIndex+1, s, 0xEE, 0);
		rootHubDevice[rootHubIndex].status = ROOT_DEVICE_FAILED;
		setUsbSpeed(1);	//TODO define speeds
	}
	return s;
}

unsigned char checkRootHubConnections()
{
	unsigned char s;
	s = ERR_SUCCESS;
	if (UIF_DETECT)                                                        
	{
		UIF_DETECT = 0;    
			if(USB_HUB_ST & bUHS_H0_ATTACH)
			{
				if(rootHubDevice[0].status == ROOT_DEVICE_DISCONNECT || (UHUB0_CTRL & bUH_PORT_EN) == 0x00)
				{
					disableRootHubPort(0);	//todo really need to reset register?
					rootHubDevice[0].status = ROOT_DEVICE_CONNECTED;
					DEBUG_OUT("Device at root hub %i connected\n", 0);
					//sendProtocolMSG(MSG_TYPE_CONNECTED,0, 0x01, 0x01, 0x01, 0);
					s = initializeRootHubConnection(0);
				}
			}
			else
			if(rootHubDevice[0].status >= ROOT_DEVICE_CONNECTED)
			{
    			resetHubDevices(0);
				disableRootHubPort(0);
				DEBUG_OUT("Device at root hub %i disconnected\n", 0);
					//sendProtocolMSG(MSG_TYPE_DISCONNECTED,0, 0x01, 0x01, 0x01, 0);
				s = ERR_USB_DISCON;
			}
			if(USB_HUB_ST & bUHS_H1_ATTACH)
			{
				
				if(rootHubDevice[1].status == ROOT_DEVICE_DISCONNECT || (UHUB1_CTRL & bUH_PORT_EN) == 0x00)
				{
					disableRootHubPort(1);	//todo really need to reset register?
					rootHubDevice[1].status = ROOT_DEVICE_CONNECTED;
					DEBUG_OUT("Device at root hub %i connected\n", 1);
					//sendProtocolMSG(MSG_TYPE_CONNECTED,0, 0x02, 0x02, 0x02, 0);
					s = initializeRootHubConnection(1);
				}
			}
			else
			if(rootHubDevice[1].status >= ROOT_DEVICE_CONNECTED)
			{
    			resetHubDevices(1);
				disableRootHubPort(1);
				DEBUG_OUT("Device at root hub %i disconnected\n", 1);
					//sendProtocolMSG(MSG_TYPE_DISCONNECTED,0, 0x02, 0x02, 0x02, 0);
				s = ERR_USB_DISCON;
			}
	}
	return s;
}