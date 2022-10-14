#include "Type.h"
#include "Protocol.h"
#include "RecvBuffer.h"

//buffer struct
typedef struct _BUFFER_STRUCT
{
	UINT8 *pBufferIn;
	UINT8 *pBufferOut;
	UINT8 *pBufferStart;
	UINT8 *pBufferEnd;

	UINT8 BlockSize;
} BUFFER_STRUCT;

//receive struct
typedef struct _RECV_STRUCT
{
	BUFFER_STRUCT  RecvBuffer;
	UINT8          RPtr;
	UINT8          TimerCounter;
} RECV_STRUCT;

#define BUFFER_BLOCK_SIZE   4
#define BUFFER_BLOCK_COUNT  4

static UINT8X s_buffer[BUFFER_BLOCK_SIZE * BUFFER_BLOCK_COUNT];

static RECV_STRUCT volatile s_recvStruct;

typedef enum
{
	RECV_STATE_IDLE = 0,
	RECV_STATE_PACKET
} RECV_STATE;

//max interval
#define BYTE_INTERVAL    3

static UINT8 volatile s_sum = 0;

static UINT8 volatile s_left = 0;
static RECV_STATE volatile s_recvState = RECV_STATE_IDLE;

void RecvBufferTimerout(void)
{
	if (s_recvStruct.TimerCounter > 0)
	{
		s_recvStruct.TimerCounter--;
		if (s_recvStruct.TimerCounter == 0)
		{
			s_recvState = RECV_STATE_IDLE;
		}
	}
}

void RecvBufferOneByte(UINT8 ch)
{
	if (s_recvState == RECV_STATE_IDLE)
	{
		BOOL validCh = FALSE;
		
		if (ch == ID_LED_STATUS)
		{
			s_left = LED_LEN + 1;

			validCh = TRUE;
		}

		if (validCh)
		{
			s_recvStruct.RecvBuffer.pBufferIn[0] = ch;
			s_recvStruct.RPtr = 1;
			s_recvStruct.TimerCounter = BYTE_INTERVAL;

			s_recvState = RECV_STATE_PACKET;

			s_sum = ch;
		}	
	}
	else if (s_recvState == RECV_STATE_PACKET)
	{
		if (--s_left > 0)
		{
			s_recvStruct.RecvBuffer.pBufferIn[s_recvStruct.RPtr++] = ch;
			s_recvStruct.TimerCounter = BYTE_INTERVAL;

			s_sum ^= ch;
		}
		else
		{
			//check sum
			if (s_sum == ch)
			{
				s_recvStruct.RecvBuffer.pBufferIn +=  s_recvStruct.RecvBuffer.BlockSize;
				if (s_recvStruct.RecvBuffer.pBufferIn == s_recvStruct.RecvBuffer.pBufferEnd)
				{
					s_recvStruct.RecvBuffer.pBufferIn = s_recvStruct.RecvBuffer.pBufferStart;
				}
			}

			s_recvStruct.RPtr = 0;
			s_recvStruct.TimerCounter = 0;

			s_recvState = RECV_STATE_IDLE;
		}
	}
}

void InitRecvBuffer(void)
{
	s_recvStruct.RecvBuffer.pBufferStart = s_buffer;
	s_recvStruct.RecvBuffer.pBufferEnd   = s_buffer + sizeof(s_buffer);
	s_recvStruct.RecvBuffer.pBufferIn    = s_buffer;
	s_recvStruct.RecvBuffer.pBufferOut   = s_buffer;
	s_recvStruct.RecvBuffer.BlockSize    = BUFFER_BLOCK_SIZE;
	
	s_recvStruct.RPtr                    = 0;

	s_recvStruct.TimerCounter            = 0;
}

BOOL IsRecvBufferEmpty(void)
{
	return s_recvStruct.RecvBuffer.pBufferIn == s_recvStruct.RecvBuffer.pBufferOut;
}

UINT8 *GetOutputBuffer(void)
{
	UINT8 *p = s_recvStruct.RecvBuffer.pBufferOut;
	s_recvStruct.RecvBuffer.pBufferOut += s_recvStruct.RecvBuffer.BlockSize;
	if (s_recvStruct.RecvBuffer.pBufferOut == s_recvStruct.RecvBuffer.pBufferEnd)
	{
		s_recvStruct.RecvBuffer.pBufferOut = s_recvStruct.RecvBuffer.pBufferStart;
	}

	return p;
}

