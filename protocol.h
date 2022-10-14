
#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

//packet id
#define ID_USB_KEYBOARD   0xA0
#define ID_USB_MOUSE      0xA1
#define ID_PS2_KEYBOARD   0xA2
#define ID_PS2_MOUSE      0xA3
#define ID_LED_STATUS     0xA4
#define ID_QUERY_ONLINE   0xA5
#define ID_SWITCH         0xA6

//data length
#define KEYBOARD_LEN  8
#define MOUSE_LEN     4
#define LED_LEN       1
#define ONLINE_LEN    1
#define SWITCH_LEN    1

//led status pos
#define NUM_LOCK_POS  0
#define CAP_LOCK_POS  1
#define SCROLL_LOCK_POS 2

//online 
#define STATUS_ONLINE 1
#define STATUS_OFFLINE 0

//switch
#define SWITCH_IN     1
#define SWITCH_OUT    0


#endif

