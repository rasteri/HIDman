/*
    Test cases for nested hub support
    
    Tests:
    1. Hub hierarchy data structures
    2. Address allocation for nested devices
    3. Parent/child relationships
    4. Child hub port allocation
*/

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
#include "andyalloc.h"
#include "usbll.h"
#include "linkedlist.h"

__xdata bool MenuActive = 0;
__xdata USB_HUB_PORT TestRootHub;
__xdata volatile uint16_t SoftWatchdog = 0;  // Mock for delay.c

// Mock address allocation
static UINT8 g_MockAddress = 1;

void MockResetAddressAllocation()
{
    g_MockAddress = 1;
}

UINT8 MockAssignUniqueAddress()
{
    return g_MockAddress++;
}

// Simple UART functions for testing
int putchar(int c)
{
    while (!TI)
        ;
    TI = 0;
    SBUF = c & 0xFF;
    return c;
}

void UART_Init(void)
{
    SCON = 0x50; /* configure serial */
    TMOD = 0x20; /* configure timer */
    TH1 = 0xE6;  /* baud rate 1200 */
    TL1 = 0xE6;  /* baud rate 1200 */
    TR1 = 1;     /* enable timer */
    TI = 1;      /* enable transmitting */
    RI = 0;      /* waiting to receive */
}

// Test 1: Verify hub hierarchy initialization
void TestHubHierarchy()
{
    printf("Test: Hub Hierarchy Initialization...\n");
    
    // Initialize root hub
    InitHubPortData(&TestRootHub);
    TestRootHub.HubLevel = 0;
    TestRootHub.ParentHub = NULL;
    TestRootHub.ParentHubPortIndex = EXHUB_PORT_NONE;
    
    assert(TestRootHub.HubLevel == 0);
    assert(TestRootHub.ParentHub == NULL);
    assert(TestRootHub.ChildHubPorts == NULL);
    
    // Allocate child ports for root hub
    TestRootHub.HubPortNum = 4;
    TestRootHub.ChildHubPorts = AllocateChildHubPorts(4);
    assert(TestRootHub.ChildHubPorts != NULL);
    
    // Set up child hub
    TestRootHub.ChildHubPorts[0].ParentHub = &TestRootHub;
    TestRootHub.ChildHubPorts[0].ParentHubPortIndex = 0;
    TestRootHub.ChildHubPorts[0].HubLevel = 1;
    
    assert(TestRootHub.ChildHubPorts[0].HubLevel == 1);
    assert(TestRootHub.ChildHubPorts[0].ParentHub == &TestRootHub);
    assert(TestRootHub.ChildHubPorts[0].ParentHubPortIndex == 0);
    
    // Allocate child ports for second-level hub
    TestRootHub.ChildHubPorts[0].HubPortNum = 4;
    TestRootHub.ChildHubPorts[0].ChildHubPorts = AllocateChildHubPorts(4);
    assert(TestRootHub.ChildHubPorts[0].ChildHubPorts != NULL);
    
    // Set up nested hub's child
    TestRootHub.ChildHubPorts[0].ChildHubPorts[0].ParentHub = &TestRootHub.ChildHubPorts[0];
    TestRootHub.ChildHubPorts[0].ChildHubPorts[0].ParentHubPortIndex = 0;
    TestRootHub.ChildHubPorts[0].ChildHubPorts[0].HubLevel = 2;
    
    assert(TestRootHub.ChildHubPorts[0].ChildHubPorts[0].HubLevel == 2);
    assert(TestRootHub.ChildHubPorts[0].ChildHubPorts[0].ParentHub == &TestRootHub.ChildHubPorts[0]);
    
    printf("  PASS - Hub hierarchy correctly initialized\n");
}

// Test 2: Verify mock address allocation
void TestAddressAllocation()
{
    printf("Test: Mock Address Allocation...\n");
    
    UINT8 addr1, addr2, addr3;
    
    // Reset address counter
    MockResetAddressAllocation();
    
    // Allocate addresses
    addr1 = MockAssignUniqueAddress();
    addr2 = MockAssignUniqueAddress();
    addr3 = MockAssignUniqueAddress();
    
    // Addresses should be sequential
    assert(addr1 == 1);
    assert(addr2 == 2);
    assert(addr3 == 3);
    
    printf("  PASS - Addresses allocated sequentially: %d, %d, %d\n", addr1, addr2, addr3);
}

// Test 3: Verify MAX_EXHUB_LEVEL is correct
void TestMaxHubLevel()
{
    printf("Test: MAX_EXHUB_LEVEL...\n");
    
    // Verify the constant is set to 2
    assert(MAX_EXHUB_LEVEL == 2);
    
    printf("  PASS - MAX_EXHUB_LEVEL is %d\n", MAX_EXHUB_LEVEL);
}

// Test 4: Verify child hub port allocation
void TestChildHubAllocation()
{
    printf("Test: Child Hub Port Allocation...\n");
    
    __xdata USB_HUB_PORT *childPorts;
    UINT8 i;
    
    // Test allocation
    childPorts = AllocateChildHubPorts(4);
    assert(childPorts != NULL);
    
    // Verify all ports are initialized
    for (i = 0; i < 4; i++)
    {
        assert(childPorts[i].HubPortStatus == PORT_DEVICE_NONE);
        assert(childPorts[i].DeviceClass == USB_DEV_CLASS_RESERVED);
        assert(childPorts[i].HubLevel == 0);
        assert(childPorts[i].ParentHub == NULL);
        assert(childPorts[i].ChildHubPorts == NULL);
    }
    
    // Test boundary conditions
    assert(AllocateChildHubPorts(0) == NULL);  // 0 ports
    assert(AllocateChildHubPorts(MAX_EXHUB_PORT_NUM + 1) == NULL);  // Too many ports
    
    printf("  PASS - Child hub ports allocated and initialized correctly\n");
}

// Test 5: Verify FreeChildHubPorts works correctly
void TestFreeChildHubPorts()
{
    printf("Test: Free Child Hub Ports...\n");
    
    __xdata USB_HUB_PORT *childPorts;
    __xdata USB_HUB_PORT *nestedPorts;
    
    // Allocate parent ports
    childPorts = AllocateChildHubPorts(2);
    assert(childPorts != NULL);
    
    // Make one a hub with its own children
    childPorts[0].DeviceClass = USB_DEV_CLASS_HUB;
    childPorts[0].HubPortNum = 2;
    nestedPorts = AllocateChildHubPorts(2);
    childPorts[0].ChildHubPorts = nestedPorts;
    
    // Free the hierarchy (note: andyalloc doesn't actually free, just clears pointers)
    FreeChildHubPorts(childPorts, 2);
    
    // Verify nested pointers were cleared
    assert(childPorts[0].ChildHubPorts == NULL);
    
    printf("  PASS - Child hub ports freed correctly\n");
}

void main()
{
    // timer0 setup
    TMOD = (TMOD & 0xf0) | 0x02; // mode 1 (8bit auto reload)
    TH0 = 0x00;

    TR0 = 1; // start timer0
    ET0 = 1; // enable timer0 interrupt

    EA = 1;  // enable all interrupts

    UART_Init();

    printf("\n=== Nested Hub Support Tests ===\n\n");

    TestHubHierarchy();
    TestAddressAllocation();
    TestMaxHubLevel();
    TestChildHubAllocation();
    TestFreeChildHubPorts();

    printf("\n=== ALL NESTED HUB TESTS PASSED ===\n");

    for (;;)
    {
        // Test complete
    }
}
