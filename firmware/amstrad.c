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

uint8_t amsoldstate = 0;
void AmstradProcessPort(void)
{
	const uint8_t *chunk;
	static __data uint8_t sb;

	bool reEnter = 0;

	do
	{
		/*if (ports[PORT_KEY].state != amsoldstate)
		{
			printf("%hhd-%hhd\n", ports[PORT_KEY].state, ports[PORT_KEY].sendbit);
			amsoldstate = ports[PORT_KEY].state;
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

			//clock appears to be normally high
			WritePS2Clock(PORT_KEY, 1);
			ports[PORT_KEY].sendbit = 0;
			break;

		case S_IDLE:

			// if ps2 clock is low, host is resetting - send acknowledge
			if (!ReadPS2Clock(PORT_KEY))
			{
				SimonSaysSendKeyboard(KEY_BATCOMPLETE);
			}

			// if host is not inhibiting (i.e. not pulling data low)
			else if (ReadPS2Data(PORT_KEY))
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

			// we want to set the data in the middle of the high cycle
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
			//make clock low
			WritePS2Clock(PORT_KEY, 0);
			ports[PORT_KEY].state = S_SEND_CLOCK_LOW;

			break;

		case S_SEND_CLOCK_LOW:

			// if final bit, move onto next byte
			if (sb == 8)
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

				// give ourselves a little break between bytes
				ports[PORT_KEY].state = S_IDLE;

				// data high so we can detect inhibit
				WritePS2Data(PORT_KEY, 1);
			}
			else
			{
				ports[PORT_KEY].state = S_SEND_CLOCK_HIGH;
			}

			// either way set clock line high
			WritePS2Clock(PORT_KEY, 1);

			break;

		}
	} while (reEnter);
}