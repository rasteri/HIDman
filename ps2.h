
#ifndef __PS2_H__
#define __PS2_H__
#include <stdbool.h>
#define PORT_KEY 0
#define PORT_MOUSE 1

#define CLOCK 0
#define DATA 1

typedef union sendbuffer {
	const uint8_t *chunky[64];
	uint8_t arbitrary[64];
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

	uint8_t recvvalid;
	uint8_t recvout;
	uint8_t recverror;

	uint8_t lastByte;

	// ring buffer (pointers to chunks)
	uint8_t sendBuffStart;
	uint8_t sendBuffEnd;
	sendbuffer sendBuff;
	uint8_t recvBuff;
	uint8_t prevhid[8];

	// temporary buffer to hold a custom chunk
	// (i.e. one defined in RAM rather than code)
	uint8_t customChunk[8];
} ps2port;

extern __xdata ps2port ports[];

void SendHIDPS2(unsigned short length, unsigned char type, unsigned char __xdata *msgbuffer);

bool GetPort(unsigned char port, unsigned char channel);

void SendPS2(uint8_t port, const uint8_t *chunk);
void PS2ProcessPort(uint8_t port);



#define OutPort(port, channel, val) if (port == PORT_KEY) if (channel == CLOCK) KEY_CLOCK = val; else KEY_DATA = val;  else if (port == PORT_MOUSE) if (channel == CLOCK) MOUSE_CLOCK = val; else MOUSE_DATA = val;

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

#define R_IDLE 0
#define R_LEDS 1
#define R_REPEAT 2
#define R_SECONDBYTE 3


#endif