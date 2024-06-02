# HIDman
## USB HID to XT / AT / PS/2 / Serial converter

HIDman is an open source device to allow the use of modern USB keyboards and mice on legacy PCs.

Peripherals that support legacy PCs are becoming hard to find, especially for the very first PCs. This project aims to provide a simple cheap solution that will cover everything from the original IBM 5150 PC all the way up to modern computers that have PS/2 ports.

![Hidman Front](/images/hidmanfront.jpg)
![Hidman Front](/images/hidmanrear.jpg)


## Features

* (should) Support the majority of standard USB keyboards and mice, including ones that use wireless dongles.
* Also supports (some) USB Game controllers - buttons/axes are mapped to keypresses or mouse actions
* Can emulate the following devices :
* * OG IBM PC / XT Keyboard
  * AT / PS/2 Keyboard
  * Serial Mouse (with optional Logitech-style 3rd button)
  * PS/2 Mouse (with optional Intellipoint-style scroll wheel and up to 5 buttons)
* Driverless configuration menu - change settings just by opening a text editor

## Technical description


The HIDman is based around the CH559 from WCH, a remarkably flexible chip with **two** USB HOST ports. This makes it ideal for our purposes.

The code is forked from atc1441's excellent repository - https://github.com/atc1441/CH559sdccUSBHost

PCB and enclosure was designed in KiCad - source files are in the hardware directory.

Development is very active but it is usable in its current state.
