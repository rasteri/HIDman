
#include <stdio.h>
#include <ctype.h>
#include "ch559.h"
#include "parsedescriptor.h"

#include "data.h"

void UART_Init()
{
    SCON = 0x50; /* configure serial */
    TMOD = 0x20; /* configure timer */
    TH1 = 0xE6;  /* baud rate 1200 */
    TL1 = 0xE6;  /* baud rate 1200 */
    TR1 = 1;     /* enable timer */
    TI = 1;      /* enable transmitting */
    RI = 0;      /* waiting to receive */
}

INTERFACE funky;

void main()
{
    UART_Init();

    ParseReportDescriptor(StandardKeyboardDescriptor, 63, &funky);

    INTERFACE *bleh = &funky;

    HID_SEG *tmpseg;
    for (uint8_t x = 0; x < MAX_REPORTS; x++)
    {
        if (bleh->reports[x] != NULL)
        {
            tmpseg = bleh->reports[x]->firstHidSeg;

            printf("Report %x, usage %x, length %u: \n", x, bleh->reports[x]->appUsage, bleh->reports[x]->length);
            while (tmpseg != NULL)
            {
                printf("  startbit %u, it %hx, ip %x, chan %hx, cont %hx, size %hx, count %hx\n", tmpseg->startBit, tmpseg->InputType, tmpseg->InputParam, tmpseg->OutputChannel, tmpseg->OutputControl, tmpseg->reportSize, tmpseg->reportCount);
                tmpseg = tmpseg->next;
            }
        }
    }

    printf("\n");

    for (;;)
    {
        //printf("v");
    }
}