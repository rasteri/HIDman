#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "ch559.h"
#include "usbhost.h"
#include "uart.h"
#include "ps2.h"
#include "data.h"
#include "ps2protocol.h"
#include "defs.h"
#include "xt.h"

uint16_t ratelimit = 0;

uint8_t oldstate = 0;
void AmstradProcessPort(void)
{
	const uint8_t *chunk;
	static __data uint8_t sb;

	bool reEnter = 0;

	do
	{
		/*if (ports[PORT_KEY].state != oldstate)
		{
			printf("%hhd-%hhd\n", ports[PORT_KEY].state, ports[PORT_KEY].sendbit);
			oldstate = ports[PORT_KEY].state;
		}*/
		sb = ports[PORT_KEY].sendbit;
		reEnter = 0;

		// XT bit-bang state machine
		switch (ports[PORT_KEY].state)
		{
		case S_INIT:
			ports[PORT_KEY].state = S_IDLE;
			// data should be held high to detect inhibit
			WritePS2Data(PORT_KEY, 1);
			ports[PORT_KEY].sendbit = 0;
			break;

		case S_IDLE:
			// if host is not inhibiting (i.e. not pulling data low)
			if (ReadPS2Data(PORT_KEY))
			{
				if (ports[PORT_KEY].rateLimit > 0) ports[PORT_KEY].rateLimit--;

				//if ratelimiter is ok and buffer not empty
				if (ports[PORT_KEY].rateLimit == 0 && ports[PORT_KEY].sendBuffEnd != ports[PORT_KEY].sendBuffStart)
				{

					ports[PORT_KEY].rateLimit = XT_RATE_LIMIT;

					chunk = ports[PORT_KEY].sendBuff.chunky[ports[PORT_KEY].sendBuffStart];
					ports[PORT_KEY].data = chunk[ports[PORT_KEY].bytenum + 1];
					//DEBUG_OUT("Consuming %x %x %x %x\n", ports[PORT_KEY].sendBuffStart, ports[PORT_KEY].sendBuffEnd, chunk[0], ports[PORT_KEY].data);

					if (ReadPS2Data(PORT_KEY))
					{
						ports[PORT_KEY].state = S_SEND_CLOCK_HIGH;
						ports[PORT_KEY].recvbit = 0;
						//reEnter = 1;
					}

				}
			}

			break;

		case S_SEND_CLOCK_HIGH:
			// reuse recvbit as counter
			ports[PORT_KEY].recvbit++;

			// we want to set the data in the middle of the low cycle
			if (ports[PORT_KEY].recvbit == 2)
			{
				// amstrad bits work MSB first because why not

				// set current bit data
				WritePS2Data(PORT_KEY, ports[PORT_KEY].data & 0x80);

				// shift in preperation for next bit
				ports[PORT_KEY].data = ports[PORT_KEY].data << 1;

				ports[PORT_KEY].sendbit++;
				ports[PORT_KEY].state = S_SEND_CLOCK_FALL;
				ports[PORT_KEY].recvbit = 0;

			}
			break;

		case S_SEND_CLOCK_FALL:
			ports[PORT_KEY].recvbit++;

			// first time round, set clock low
			if (ports[PORT_KEY].recvbit == 1)
			{
				//make clock low
				WritePS2Clock(PORT_KEY, 0);
			}
			// wait 6 clock cycles (100us) before the next thing
			else if (ports[PORT_KEY].recvbit >= 2)
			{
				ports[PORT_KEY].state = S_SEND_CLOCK_LOW;
			}

			break;

		case S_SEND_CLOCK_LOW:

			// if final bit, move onto next byte
			if (sb == 9)
			{
				ports[PORT_KEY].sendbit = 0;

				// for keyboard get the next byte in the chunk
				chunk = ports[PORT_KEY].sendBuff.chunky[ports[PORT_KEY].sendBuffStart];

				ports[PORT_KEY].bytenum++;

				// if we've run out of bytes in this chunk
				if (ports[PORT_KEY].bytenum == chunk[0])
				{
					// move onto next chunk
					//DEBUG_OUT("Consumed %x %x\n", ports[PORT_KEY].sendBuffStart, ports[PORT_KEY].sendBuffEnd);
					ports[PORT_KEY].sendBuffStart = (ports[PORT_KEY].sendBuffStart + 1) % 64;
					ports[PORT_KEY].bytenum = 0;
				}
				else
				{
					ports[PORT_KEY].data = chunk[ports[PORT_KEY].bytenum + 1];
				}

				// also need to set data line high
				WritePS2Data(PORT_KEY, 1);

				// give ourselves a little break between bytes
				ports[PORT_KEY].state = S_IDLE;
			}
			else
			{
				ports[PORT_KEY].state = S_SEND_CLOCK_RISE;
			}
			break;

		// data has been set, send falling edge of clock
		case S_SEND_CLOCK_RISE:

			WritePS2Clock(PORT_KEY, 1);
			ports[PORT_KEY].state = S_SEND_CLOCK_HIGH;
			ports[PORT_KEY].recvbit = 0;
			break;
		}
	} while (reEnter);
}