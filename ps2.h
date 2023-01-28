#ifndef __PS2_H__
#define __PS2_H__

#include <stdbool.h>
#include "defs.h"




bool ReadPS2Clock(unsigned char port);
bool ReadPS2Data(unsigned char port);

bool SendKeyboard(const uint8_t *chunk);
//void SendMouse();

void SimonSaysSendKeyboard(const uint8_t *chunk);
void SimonSaysSendMouse();

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
	if (port == PORT_KEY)        \
			KEY_CLOCK = val;         \
	else if (port == PORT_MOUSE) \
			MOUSE_CLOCK = val;

#if defined(BOARD_MICRO)

#define WritePS2Data(port, val)     \
	if (port == PORT_KEY)        \
			KEY_DATA = val;          \
	else if (port == PORT_MOUSE) \
			MOUSE_DATA = val;
#else

//P4 dir should be 1 (output) when low, 0 (input) when high
#define WritePS2Data(port, val)     \
	if (port == PORT_KEY)        \
		KEY_DATA = val;            \
	else if (port == PORT_MOUSE) \
		if (val) {                 \
			P4_OUT |= 0b00001000;    \
			P4_DIR &= 0b11110111;    \
		} else {                   \
			P4_OUT &= 0b11110111;    \
			P4_DIR |= 0b00001000;    \
		}
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

#endif