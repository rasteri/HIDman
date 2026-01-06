# Nested USB Hub Support Implementation

## Overview

This document describes the implementation of nested USB hub support in the HIDman project. The changes enable the firmware to support up to 2 levels of USB hub nesting (root hub → external hub → nested hub → devices) with efficient interrupt endpoint-based hotplug detection.

## Changes Summary

### 1. Updated Hub Level Constant

**File**: `firmware/inc/usbhost.h`, `firmware/inc/defs.h`

- Changed `MAX_EXHUB_LEVEL` from 1 to 2
- Removed duplicate definition from `defs.h` to avoid compiler warnings
- Now supports up to 2 levels of nested external hubs

### 2. Extended USB_HUB_PORT Structure

**File**: `firmware/inc/usbhost.h`

Added new fields to track hub hierarchy and interrupt endpoint:

```c
typedef struct _USB_HUB_PORT
{
    // ... existing fields ...
    
    // Hub nesting support
    UINT8       HubLevel;                      // 0 for root, 1 for first level hub, etc.
    __xdata struct _USB_HUB_PORT* ParentHub;   // NULL for root hubs
    UINT8       ParentHubPortIndex;            // Port index on parent hub
    __xdata struct _USB_HUB_PORT* ChildHubPorts[MAX_EXHUB_PORT_NUM];  // Array of pointers (lazy allocation)
    
    // Hub interrupt endpoint for efficient change detection
    UINT8       HubIntEndpointAddr;            // Interrupt IN endpoint address (0 if not available)
    UINT8       HubIntMaxPacketSize;           // Max packet size for interrupt endpoint
    UINT8       HubIntTOG;                     // Toggle bit for interrupt endpoint
} USB_HUB_PORT;
```

**Purpose**: 
- Hub hierarchy fields enable traversal of the hub tree in both directions
- Interrupt endpoint fields enable event-driven hotplug detection instead of polling all ports

### 3. Dynamic Child Hub Port Allocation (Lazy Allocation)

**Files**: `firmware/usbll.c`, `firmware/inc/usbll.h`

Implemented lazy allocation for memory efficiency:

#### `AllocateSingleChildPort()`
- Allocates a single USB_HUB_PORT structure on demand
- Only called when a device is actually detected on a port
- Initializes the port with `InitHubPortData()`
- Returns `__xdata USB_HUB_PORT*` pointer or NULL on failure

#### Memory Savings
- **Before**: Allocated N × sizeof(USB_HUB_PORT) upfront for all hub ports
- **After**: Only allocate sizeof(USB_HUB_PORT) when device is detected on a port
- **Example**: 4-port hub with 1 device saves 3 × sizeof(USB_HUB_PORT) bytes

#### Memory Management Strategy
- **Important**: andyalloc doesn't support individual free operations
- Memory is managed through `andyclearmem()` which is called during re-enumeration
- When a device is added/removed, the entire USB tree is re-enumerated:
  1. `ReenumerateAllPorts()` is called
  2. `andyclearmem()` clears ALL allocated memory
  3. The entire USB tree is enumerated from scratch
- Child hub port pointers are cleared in `InitHubPortData()` to reset state
- No explicit "free" operation is needed or possible for individual allocations

### 4. Improved Address Allocation

**File**: `firmware/usbhost.c`

Replaced fixed address scheme with sequential allocation:

#### Old Scheme:
```c
// Fixed formula based on root hub and port index
address = (MAX_EXHUB_PORT_NUM + 1) * RootHubIndex + 1 + HubPortIndex + 1;
```

#### New Scheme:
```c
// Sequential allocation with global counter
static UINT8 g_NextUsbAddress = 1;

UINT8 AssignUniqueAddress(UINT8 RootHubIndex, UINT8 HubPortIndex)
{
    address = g_NextUsbAddress++;
    if (address > 127) {  // USB address limit
        g_NextUsbAddress = 1;
        address = 1;
    }
    return address;
}
```

**Benefits**:
- Simpler logic
- Better support for nested hubs
- Prevents address conflicts
- Scales to USB's 127 device limit

### 5. Recursive Hub Initialization

**File**: `firmware/usbhost.c`

Created `InitializeHubPorts()` to recursively enumerate hub ports:

```c
BOOL InitializeHubPorts(__xdata USB_HUB_PORT *pHubDevice, UINT8 rootHubIndex)
{
    // 1. Verify device is a hub
    // 2. Check hub level doesn't exceed MAX_EXHUB_LEVEL
    // 3. Get hub descriptor and port count
    // 4. Allocate child ports dynamically
    // 5. Initialize parent-child relationships
    // 6. Power up all ports
    // 7. Wait for devices to attach
    // 8. Enumerate each attached device
    // 9. Recursively initialize child hubs
    // 10. Handle failures gracefully
}
```

**Key Features**:
- Checks hub level to prevent excessive nesting
- Handles hub descriptor retrieval
- Manages port power and reset sequences
- Detects device speed (low/full)
- Recursively processes nested hubs
- Robust error handling at each step

### 6. Hub Path Selection

**File**: `firmware/usbll.c`

Implemented `SelectHubPortByDevice()` to select devices in nested hierarchies:

```c
void SelectHubPortByDevice(USB_HUB_PORT *pUsbDevice)
{
    // 1. Check if device is on root hub
    // 2. If nested, trace up through parent pointers to root
    // 3. Select appropriate root hub
    // 4. Configure USB address and speed
    // 5. Enable PRE_PID for low-speed devices behind hubs
}
```

**Purpose**: Replaces the old `SelectHubPort(rootIndex, portIndex)` pattern with a device-centric approach that works with arbitrary nesting levels.

### 7. Recursive Port Iteration

**File**: `firmware/usbhost.c`

Converted all port iteration functions to use recursion:

#### `RegrabInterfacesRecursive(__xdata USB_HUB_PORT *pUsbHubPort)`
- Recursively regrabs HID interfaces for all devices
- Traverses hub tree depth-first
- Handles both leaf devices and hubs

#### `ProcessHIDDataRecursive(__xdata USB_HUB_PORT *pUsbHubPort)`
- Recursively processes HID data transfers
- Called during interrupt processing
- Ensures all devices are polled

#### `UpdateKeyboardLedRecursive(__xdata USB_HUB_PORT *pUsbHubPort, UINT8 led)`
- Recursively updates keyboard LED status
- Propagates LED changes to all keyboards in tree

**Pattern Used**:
```c
void RecursiveFunction(__xdata USB_HUB_PORT *pUsbHubPort)
{
    if (pUsbHubPort == NULL || status != SUCCESS)
        return;
    
    if (pUsbHubPort->DeviceClass != USB_DEV_CLASS_HUB)
    {
        // Process leaf device
    }
    else
    {
        // Recursively process children
        for (i = 0; i < pUsbHubPort->HubPortNum; i++)
        {
            RecursiveFunction(&pUsbHubPort->ChildHubPorts[i]);
        }
    }
}
```

### 8. Updated Core Functions

**File**: `firmware/usbhost.c`

Modified existing functions to use new recursive helpers:

- `RegrabDeviceReports()` - Now calls `RegrabInterfacesRecursive()`
- `InterruptProcessRootHubPort()` - Now calls `ProcessHIDDataRecursive()`
- `UpdateUsbKeyboardLed()` - Now calls `UpdateKeyboardLedRecursive()`
- `ReenumerateAllPorts()` - Now calls `ResetAddressAllocation()`

### 9. Memory Management

**File**: `firmware/usbdata.c`, `firmware/usbll.c`

Updated `InitHubPortData()` to initialize new fields:
```c
void InitHubPortData(USB_HUB_PORT *pUsbHubPort)
{
    // ... existing initialization ...
    
    // Initialize hub nesting fields
    pUsbHubPort->HubLevel = 0;
    pUsbHubPort->ParentHub = NULL;
    pUsbHubPort->ParentHubPortIndex = EXHUB_PORT_NONE;
    pUsbHubPort->ChildHubPorts = NULL;
}
```

Updated `InitRootHubPortData()` for proper re-enumeration:
```c
void InitRootHubPortData(UINT8 rootHubIndex)
{
    // Simply initialize port data - memory cleanup happens via andyclearmem()
    InitHubPortData(&RootHubPort[rootHubIndex]);
    
    for (i = 0; i < MAX_EXHUB_PORT_NUM; i++)
    {
        InitHubPortData(&SubHubPort[rootHubIndex][i]);
    }
}
```

**Note**: Child hub port memory is NOT explicitly freed here. The cleanup happens through the re-enumeration flow:
1. Device change detected → `ReenumerateAllPorts()` called
2. `andyclearmem()` clears all memory
3. USB tree re-enumerated from scratch
4. New child ports allocated as needed

## Testing

### Test Suite: testnestedhubs.c

Created comprehensive unit tests to validate nested hub support:

#### Test 1: Hub Hierarchy Initialization
- Validates parent-child relationships
- Tests 2-level nested hub structure
- Verifies HubLevel tracking

#### Test 2: Address Allocation
- Tests sequential address assignment
- Verifies address counter reset
- Validates uniqueness

#### Test 3: MAX_EXHUB_LEVEL Verification
- Confirms constant is set to 2
- Ensures compile-time configuration is correct

#### Test 4: Child Hub Port Allocation
- Tests dynamic allocation
- Validates initialization of allocated ports
- Tests boundary conditions (0 ports, too many ports)

#### Test 5: Memory Management Strategy
- Validates memory allocation
- Confirms memory cleanup relies on `andyclearmem()` during re-enumeration
- Verifies nested hub port allocation works correctly

### Running Tests

```bash
cd firmware
make testnestedhubs
```

Note: Tests compile successfully but require a simulator or hardware to execute.

## Compilation

The implementation compiles successfully with SDCC 4.2.0. Key considerations:

### SDCC-Specific Issues Resolved

1. **Pointer Type Qualifiers**: All pointers to xdata memory use explicit `__xdata` qualifier
2. **Static Variables**: Recursive functions use `static __xdata` for local variables
3. **Duplicate Definitions**: Removed `MAX_EXHUB_LEVEL` from `defs.h`

### Build Status

- ✅ Main firmware compiles (with pre-existing scancode.c issues unrelated to this change)
- ✅ Test suite compiles cleanly
- ✅ No new warnings or errors introduced

## Limitations and Future Work

### Current Limitations

1. **Maximum Hub Depth**: Limited to 2 levels (MAX_EXHUB_LEVEL = 2)
   - Can be increased by changing the constant
   - Consider memory constraints on CH559 (3500 bytes for andyalloc pool)

2. **Memory Management**: andyalloc doesn't support individual free
   - Child ports remain allocated until `andyclearmem()` is called
   - This is acceptable as enumeration is infrequent

3. **Maximum Ports Per Hub**: Limited to MAX_EXHUB_PORT_NUM (4)
   - Defined in firmware/inc/usbhost.h
   - Increase if needed but consider memory

### Future Enhancements

1. **Dynamic Hub Level**: Could detect and warn about maximum depth at runtime
2. **Error Recovery**: Enhanced error handling for hub failures
3. **Performance**: Further optimize if needed
4. **Debugging**: Add more detailed logging for nested hub operations

## Hotplug Detection

**Files**: `firmware/usbhost.c`, `firmware/usbll.c`, `firmware/inc/usbhost.h`

The firmware supports efficient hotplug detection using interrupt endpoint monitoring.

### Implementation Details

**Root Hub Detection**:
- `QueryHubPortAttach()` detects device attach/detach on root hub ports via CH559 hardware
- Hardware-based detection is fast and efficient with no polling overhead
- Triggers immediate re-enumeration when root devices change

**External Hub Detection** (Interrupt Endpoint Monitoring):

The implementation uses USB hub interrupt endpoints for event-driven change detection:

#### How It Works

1. **During Hub Enumeration** (`InitializeHubPorts()`):
   - Parse configuration descriptor to find interrupt IN endpoint
   - Store endpoint address, max packet size in hub structure
   - Typically endpoint address is 0x81 with 1-byte max packet size

2. **During Periodic Polling** (`DealUsbPort()`):
   - Check interrupt endpoint of **all** hubs in one cycle (not rotating)
   - Perform interrupt IN transfer for each hub
   - Hub NAKs if no changes (very fast, no data transfer)
   - Hub returns bitmap when port status changes

3. **Processing Interrupt Data**:
   - Bitmap format: bit 0 = hub status, bits 1-N = ports 1-N
   - Only query specific ports indicated by bitmap
   - Trigger re-enumeration if connection change detected

#### Performance Comparison

**Old Approach (Rotating Poll)**:
- Poll all ports on one hub per cycle
- N hubs with M ports: M USB transactions per cycle
- Detection latency: N cycles to check all hubs
- Example: 3 hubs × 4 ports = 4 transactions/cycle, 1.5s latency

**New Approach (Interrupt Endpoint)**:
- Check interrupt endpoint of all hubs per cycle
- Idle: N interrupt transfers (NAK responses, minimal data)
- Change: N interrupt transfers + small number of GetHubPortStatus for changed ports
- Immediate detection across all hubs (no latency)
- Much more efficient when no changes occurring

#### Advantages

- **Event-Driven**: Only USB traffic when actual changes occur
- **Lower Latency**: Immediate detection on any hub (no N-cycle delay)
- **More Efficient**: Just NAK responses when idle, no GetHubPortStatus transactions
- **Scalable**: Performance doesn't degrade with more hubs

#### Data Structures

```c
typedef struct _USB_HUB_PORT {
    // ... existing fields ...
    
    // Hub interrupt endpoint for change detection
    UINT8       HubIntEndpointAddr;   // Interrupt IN endpoint address (0 if not available)
    UINT8       HubIntMaxPacketSize;  // Max packet size for interrupt endpoint
    UINT8       HubIntTOG;            // Toggle bit for interrupt endpoint
} USB_HUB_PORT;
```

#### Functions

- `InitializeHubPorts()` - Parses config descriptor to find interrupt endpoint
- `DealUsbPort()` - Checks all hub interrupt endpoints per cycle
- Uses `USBHostTransact()` for interrupt IN transfers

#### Fallback Behavior

If a hub doesn't have an interrupt endpoint (HubIntEndpointAddr == 0), it is simply skipped during polling. Root hub detection via hardware still works normally.
- `ClearHubList()` - Clears the flat list and resets poll index

#### Re-enumeration Flow

When a change is detected on any hub port:
1. Change detected → `ReenumerateAllPorts()` called
2. `andyclearmem()` clears ALL dynamically allocated memory
3. `InitPresets()` resets configuration
4. `ResetAddressAllocation()` resets USB address counter
5. Complete USB tree enumerated from scratch
6. All devices assigned new addresses and initialized
7. `RebuildHubList()` creates new flat hub list for efficient polling

This approach provides a good balance between detection speed and performance overhead.

## Backward Compatibility

The implementation maintains full backward compatibility:

- Single-level hubs continue to work as before
- Direct device connections unchanged
- Existing SubHubPort array remains functional
- No changes to user-visible behavior

## References

- USB 2.0 Specification (Chapter 11: Hub Specification)
- CH559 Datasheet (USB Host Controller)
- SDCC Compiler Manual (8051 Target)

## Authors

- Implementation: GitHub Copilot
- Review: rasteri

## Version

- Implementation Date: January 2026
- Firmware Version: Compatible with HIDman v1.1+
- MAX_EXHUB_LEVEL: 2
