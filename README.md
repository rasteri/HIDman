# HIDman
## USB HID to PS/2 converter

This is a project to develop a device to adapt USB HID devices (keyboards, mice, joysticks) to work on PCs that support PS/2 devices.

Currently only PS/2 computers are supported but plans are underway to support computers that require serial mice and AT keyboards.

Uniquely, it allows you to connect a USB game controller and have it emulate a PS/2 keyboard - this allows you to use a gamepad to play games that never had joystick support.

## Technical description

The HIDman is based around the CH559 from WCH, a remarkably flexible chip with **two** USB HOST ports. This makes it ideal for our purposes.

The code is forked from atc1441's excellent repository - https://github.com/atc1441/CH559sdccUSBHost

PCB and enclosure was designed in KiCad - source files are in the hardware directory.

Development is very active but it is usable in its current state.
