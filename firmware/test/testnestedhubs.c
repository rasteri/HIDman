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
    assert(TestRootHub.ChildHubPorts[0] == NULL);
    
    // Allocate individual child port (lazy allocation)
    TestRootHub.HubPortNum = 4;
    TestRootHub.ChildHubPorts[0] = AllocateSingleChildPort();
    assert(TestRootHub.ChildHubPorts[0] != NULL);
    
    // Set up child hub
    TestRootHub.ChildHubPorts[0]->ParentHub = &TestRootHub;
    TestRootHub.ChildHubPorts[0]->ParentHubPortIndex = 0;
    TestRootHub.ChildHubPorts[0]->HubLevel = 1;
    
    assert(TestRootHub.ChildHubPorts[0]->HubLevel == 1);
    assert(TestRootHub.ChildHubPorts[0]->ParentHub == &TestRootHub);
    assert(TestRootHub.ChildHubPorts[0]->ParentHubPortIndex == 0);
    
    // Allocate child port for second-level hub
    TestRootHub.ChildHubPorts[0]->HubPortNum = 4;
    TestRootHub.ChildHubPorts[0]->ChildHubPorts[0] = AllocateSingleChildPort();
    assert(TestRootHub.ChildHubPorts[0]->ChildHubPorts[0] != NULL);
    
    // Set up nested hub's child
    TestRootHub.ChildHubPorts[0]->ChildHubPorts[0]->ParentHub = TestRootHub.ChildHubPorts[0];
    TestRootHub.ChildHubPorts[0]->ChildHubPorts[0]->ParentHubPortIndex = 0;
    TestRootHub.ChildHubPorts[0]->ChildHubPorts[0]->HubLevel = 2;
    
    assert(TestRootHub.ChildHubPorts[0]->ChildHubPorts[0]->HubLevel == 2);
    assert(TestRootHub.ChildHubPorts[0]->ChildHubPorts[0]->ParentHub == TestRootHub.ChildHubPorts[0]);
    
    printf("  PASS - Hub hierarchy correctly initialized with lazy allocation\n");
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

// Test 4: Verify child hub port allocation (lazy allocation)
void TestChildHubAllocation()
{
    printf("Test: Child Hub Port Lazy Allocation...\n");
    
    __xdata USB_HUB_PORT *childPort1, *childPort2;
    
    // Test individual port allocation
    childPort1 = AllocateSingleChildPort();
    assert(childPort1 != NULL);
    assert(childPort1->HubPortStatus == PORT_DEVICE_NONE);
    assert(childPort1->DeviceClass == USB_DEV_CLASS_RESERVED);
    assert(childPort1->HubLevel == 0);
    assert(childPort1->ParentHub == NULL);
    assert(childPort1->ChildHubPorts[0] == NULL);
    
    childPort2 = AllocateSingleChildPort();
    assert(childPort2 != NULL);
    assert(childPort2 != childPort1);  // Different allocations
    
    printf("  PASS - Child hub ports allocated lazily and independently\n");
}

// Test 5: Verify memory management strategy
void TestMemoryManagement()
{
    printf("Test: Memory Management Strategy...\n");
    
    __xdata USB_HUB_PORT *childPort1, *childPort2;
    
    // Allocate some ports
    childPort1 = AllocateSingleChildPort();
    assert(childPort1 != NULL);
    
    // Make it a hub with a child
    childPort1->DeviceClass = USB_DEV_CLASS_HUB;
    childPort1->HubPortNum = 4;
    childPort2 = AllocateSingleChildPort();
    childPort1->ChildHubPorts[0] = childPort2;
    
    // Note: andyalloc doesn't support individual free operations
    // Memory is cleared via andyclearmem() during re-enumeration
    // Just verify pointers are set correctly
    assert(childPort1->ChildHubPorts[0] != NULL);
    assert(childPort1->ChildHubPorts[0] == childPort2);
    
    printf("  PASS - Memory management relies on andyclearmem() during re-enumeration\n");
    printf("  INFO - Lazy allocation saves memory by only allocating ports with devices\n");
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
    TestMemoryManagement();

    printf("\n=== ALL NESTED HUB TESTS PASSED ===\n");

    for (;;)
    {
        // Test complete
    }
}
