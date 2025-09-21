
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "type.h"
#include "ch559.h"
#include "system.h"
#include "defs.h"
#include "usbdef.h"
#include "usbhost.h"
#include "menu.h"
#include "data.h"
#include "settings.h"
#include "andyalloc.h"
#include "keyboardled.h"
#include "parsedescriptor.h"
#include "ps2protocol.h"
#include "ps2.h"
#include "testdata.h"
#include "preset.h"
#include "test.h"
#include "usbll.h"
#include "linkedlist.h"
#include "testcommon.h"
#include "scancode.h"
#include "processreport.h"

bool TestExtractValue(uint8_t reportSize, uint16_t startBit){

    static __xdata HID_SEG testSeg;
    
    testSeg.startBit = startBit;
    testSeg.reportSize = reportSize;

    //DumpHID(pInterface);

    static __xdata uint32_t ogtestdata;
    static __xdata uint32_t testdata;
    static __xdata uint32_t afterdata;

    //start with all zeros
    testdata = 0;
    afterdata = SegExtractValue(&testSeg, (__xdata uint8_t *)(&testdata));
    assert(testdata == afterdata);

    //now all 1s
    ogtestdata = (0xFFFFFFFF & bitMasks32[reportSize]);
    testdata = ogtestdata << startBit;
    afterdata = SegExtractValue(&testSeg, (__xdata uint8_t *)(&testdata));
    assert(ogtestdata == afterdata);

    //now a few random values
    for (uint8_t c = 0; c < 20; c++) {
        ogtestdata = rand32() & bitMasks32[reportSize];
        testdata = ogtestdata << startBit;
        afterdata = SegExtractValue(&testSeg, (__xdata uint8_t *)(&testdata));
        if (ogtestdata != afterdata){
            printf("%llx != %llx\n", ogtestdata, afterdata);
        }
        assert(ogtestdata == afterdata);
    }

    // if we're testing a single bit, set JUST that bit and make sure none of the other bits return 1
    if (reportSize == 1) {
        testdata = (uint32_t)0x01 << (uint32_t)startBit;

        for (uint8_t d = 0; d < 32; d++){
            testSeg.startBit = d;

            // the bit we're looking for should be 1
            if (d == startBit) {

                assert (SegExtractValue(&testSeg, (__xdata uint8_t *)(&testdata)) == 0x01);
            }

            // the bits we're not looking for should be zero
            else {
                assert (SegExtractValue(&testSeg, (__xdata uint8_t *)(&testdata)) != 0x01);
            }
        }
    }

    return 1;
}

int main() {

    TestSetup();

    for (int size=1; size <= 32; size++) {
        for (int sb=0; sb <= 32 - size; sb++){
            //printf ("%d-%d ", size, sb);
            assert(TestExtractValue(size, sb));
        }
    }

    printf("PASS\n");
    halt();
}


