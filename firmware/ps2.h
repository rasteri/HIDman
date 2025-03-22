#ifndef __PS2_H__
#define __PS2_H__

#include <stdbool.h>
#include "ch559.h"
#include "settings.h"
#define PORT_KEY 0
#define PORT_MOUSE 1

// increase to reduce maximum rate bytes can be sent
// units are in 60khz
#define PS2_RATE_LIMIT 0
#define XT_RATE_LIMIT 1000

void InitPS2Ports(void);

#if defined(BOARD_MICRO)        // Pinouts for HIDman-micro
	SBIT(KEY_CLOCK, 0x90, 7);
	SBIT(KEYAUX_CLOCK, 0x90, 7);
	#if defined(OPT_SWAP_KBD_MSC) // Makes it easier to direct solder combo PS/2 port
		SBIT(KEY_DATA, 0x90, 6);
		SBIT(KEYAUX_DATA, 0x90, 6);
		SBIT(MOUSE_CLOCK, 0x90, 4);
		SBIT(MOUSEAUX_CLOCK, 0x90, 4);
	#else
		SBIT(KEY_DATA, 0x90, 4);
		SBIT(KEYAUX_DATA, 0x90, 4);
		SBIT(MOUSE_CLOCK, 0x90, 6);
		SBIT(MOUSEAUX_CLOCK, 0x90, 6);
	#endif
	SBIT(MOUSE_DATA, 0x90, 5);
	SBIT(MOUSEAUX_DATA, 0x90, 5);
#elif defined(BOARD_PS2)
	SBIT(KEY_CLOCK, 0xB0, 4);
	SBIT(KEY_DATA, 0xB0, 5);

	SBIT(KEYAUX_CLOCK, 0xB0, 6);
	SBIT(KEYAUX_DATA, 0xB0, 7);

	SBIT(MOUSE_CLOCK, 0xA0, 0);
	SBIT(MOUSE_DATA, 0xA0, 1);

	SBIT(MOUSEAUX_CLOCK, 0xB0, 2);
	SBIT(MOUSEAUX_DATA, 0xB0, 3);

#else // Default pinouts (HIDman-AXD, HIDman-mini)
	SBIT(KEY_CLOCK, 0x80, 5);
	SBIT(KEY_DATA, 0x80, 3);

	SBIT(KEYAUX_CLOCK, 0xB0, 5);
	SBIT(KEYAUX_DATA, 0xB0, 6);

	SBIT(MOUSE_CLOCK, 0xB0, 7);
	SBIT(MOUSE_DATA, 0xC1, 3);

	SBIT(MOUSEAUX_CLOCK, 0x80, 7);
	SBIT(MOUSEAUX_DATA, 0x80, 6);
#endif

typedef union sendbuffer
{
	// pointers to chunks in code space
	const uint8_t *chunky[64];

	// chunks of up to 8 bytes, first byte is length
	uint8_t chonky[8][8];
} sendbuffer;

typedef struct ps2port
{
	uint8_t state;
	uint8_t port;
	uint8_t data;
	uint8_t sendbit;
	uint8_t recvbit;
	uint8_t parity;
	uint8_t sendingCustom;

	uint8_t recvstate;

	// byte number within current chunk
	uint8_t bytenum;

	uint8_t recvout;
	uint8_t sendDisabled;

	uint8_t lastByte;

	uint16_t rateLimit;

	// ring buffer (pointers to chunks)
	uint8_t sendBuffStart;
	uint8_t sendBuffEnd;
	sendbuffer sendBuff;
	uint8_t recvBuff;

	uint16_t resetCounter;

} ps2port;

extern __xdata ps2port ports[];

bool ReadPS2Clock(unsigned char port);
bool ReadPS2Data(unsigned char port);

bool SendKeyboard(const uint8_t *chunk);
//void SendMouse();

void SimonSaysSendKeyboard(const uint8_t *chunk);
void SimonSaysSendMouse(void);
void PressKey(uint8_t currchar);
void ReleaseKey(uint8_t currchar);
void PS2ProcessPort(uint8_t port);

#define SimonSaysSendMouse1(one)                                                    \
	if ((ports[PORT_MOUSE].sendBuffEnd + 1) % 8 != ports[PORT_MOUSE].sendBuffStart) \
	{                                                                               \
		ports[PORT_MOUSE].sendBuff.chonky[ports[PORT_MOUSE].sendBuffEnd][0] = 1;    \
		ports[PORT_MOUSE].sendBuff.chonky[ports[PORT_MOUSE].sendBuffEnd][1] = one;  \
		ports[PORT_MOUSE].sendBuffEnd = (ports[PORT_MOUSE].sendBuffEnd + 1) % 8;    \
	}

#define SimonSaysSendMouse2(one, two)                                               \
	if ((ports[PORT_MOUSE].sendBuffEnd + 1) % 8 != ports[PORT_MOUSE].sendBuffStart) \
	{                                                                               \
		ports[PORT_MOUSE].sendBuff.chonky[ports[PORT_MOUSE].sendBuffEnd][0] = 2;    \
		ports[PORT_MOUSE].sendBuff.chonky[ports[PORT_MOUSE].sendBuffEnd][1] = one;  \
		ports[PORT_MOUSE].sendBuff.chonky[ports[PORT_MOUSE].sendBuffEnd][2] = two;  \
		ports[PORT_MOUSE].sendBuffEnd = (ports[PORT_MOUSE].sendBuffEnd + 1) % 8;    \
	}

#define SimonSaysSendMouse3(one, two, three)                                         \
	if ((ports[PORT_MOUSE].sendBuffEnd + 1) % 8 != ports[PORT_MOUSE].sendBuffStart)  \
	{                                                                                \
		ports[PORT_MOUSE].sendBuff.chonky[ports[PORT_MOUSE].sendBuffEnd][0] = 3;     \
		ports[PORT_MOUSE].sendBuff.chonky[ports[PORT_MOUSE].sendBuffEnd][1] = one;   \
		ports[PORT_MOUSE].sendBuff.chonky[ports[PORT_MOUSE].sendBuffEnd][2] = two;   \
		ports[PORT_MOUSE].sendBuff.chonky[ports[PORT_MOUSE].sendBuffEnd][3] = three; \
		ports[PORT_MOUSE].sendBuffEnd = (ports[PORT_MOUSE].sendBuffEnd + 1) % 8;     \
	}

#define SimonSaysSendMouse4(one, two, three, four)                                   \
	if ((ports[PORT_MOUSE].sendBuffEnd + 1) % 8 != ports[PORT_MOUSE].sendBuffStart)  \
	{                                                                                \
		ports[PORT_MOUSE].sendBuff.chonky[ports[PORT_MOUSE].sendBuffEnd][0] = 4;     \
		ports[PORT_MOUSE].sendBuff.chonky[ports[PORT_MOUSE].sendBuffEnd][1] = one;   \
		ports[PORT_MOUSE].sendBuff.chonky[ports[PORT_MOUSE].sendBuffEnd][2] = two;   \
		ports[PORT_MOUSE].sendBuff.chonky[ports[PORT_MOUSE].sendBuffEnd][3] = three; \
		ports[PORT_MOUSE].sendBuff.chonky[ports[PORT_MOUSE].sendBuffEnd][4] = four;  \
		ports[PORT_MOUSE].sendBuffEnd = (ports[PORT_MOUSE].sendBuffEnd + 1) % 8;     \
	}

#define SendMouse1(...)               \
	TR0 = 0;                          \
	SimonSaysSendMouse1(__VA_ARGS__); \
	TR0 = 1;
#define SendMouse2(...)               \
	TR0 = 0;                          \
	SimonSaysSendMouse2(__VA_ARGS__); \
	TR0 = 1;
#define SendMouse3(...)               \
	TR0 = 0;                          \
	SimonSaysSendMouse3(__VA_ARGS__); \
	TR0 = 1;
#define SendMouse4(...)               \
	TR0 = 0;                          \
	SimonSaysSendMouse4(__VA_ARGS__); \
	TR0 = 1;


#define WritePS2Clock(port, val)    \
	if (port == PORT_KEY) {       	\
			KEY_CLOCK = val;         \
			if (FlashSettings->EnableAUXPS2) KEYAUX_CLOCK = val;		\
	}	\
	else if (port == PORT_MOUSE){  	\
			MOUSE_CLOCK = val;	\
			if (FlashSettings->EnableAUXPS2) MOUSEAUX_CLOCK = val; \
	}\




#if defined(BOARD_AXP) || defined(BOARD_MINI)
//P4 dir should be 1 (output) when low, 0 (input) when high
#define WritePS2Data(port, val)     \
	if (port == PORT_KEY){        \
		KEY_DATA = val;            \
		if (FlashSettings->EnableAUXPS2) KEYAUX_DATA = val;\
	} \
	else if (port == PORT_MOUSE) { \
		if (val) {                 \
			P4_OUT |= 0b00001000;    \
			P4_DIR &= 0b11110111;    \
		} else {                   \
			P4_OUT &= 0b11110111;    \
			P4_DIR |= 0b00001000;    \
		}							\
		if (FlashSettings->EnableAUXPS2) MOUSEAUX_DATA = val;\
	}
#else
#define WritePS2Data(port, val)     \
	if (port == PORT_KEY){        \
			KEY_DATA = val;          \
			if (FlashSettings->EnableAUXPS2) KEYAUX_DATA = val;}		\
	else if (port == PORT_MOUSE){ \
			MOUSE_DATA = val;\
			if (FlashSettings->EnableAUXPS2) MOUSEAUX_DATA = val; }

#endif



#define S_INIT 0
#define S_IDLE 1
#define S_SEND_CLOCK_LOW 2
#define S_SEND_CLOCK_HIGH 3
#define S_SEND_CLOCK_FALL 12
#define S_SEND_CLOCK_RISE 13
#define S_RECEIVE_CLOCK_LOW 4
#define S_RECEIVE_CLOCK_HIGH 5
#define S_RECEIVE_ACK_HIGH 6
#define S_RECEIVE_ACK_FALL 14
#define S_RECEIVE_ACK_LOW 15
#define S_RECEIVE_ACK_RISE 17
#define S_RECEIVE_ACK_HIGH_DONE 16
#define S_RECEIVE_CLOCK_FALL 7
#define S_RECEIVE_CLOCK_RISE 8
#define S_PAUSE 9
#define S_INHIBIT 10
#define S_WAIT 11
#define S_MIDSEND_PAUSE 18

#define S_RTS 19
#define S_WAITING_CTS 20

#define R_IDLE 0
#define R_LEDS 1
#define R_REPEAT 2
#define R_SCANCODESET 3
#define R_KEYLIST 4

#define PS2_KEY_LED_SCROLLLOCK  0b00000001
#define PS2_KEY_LED_NUMLOCK     0b00000010
#define PS2_KEY_LED_CAPSLOCK    0b00000100
#define PS2_KEY_LED_KANA        0b00010000 // this might not actually be correct

#endif