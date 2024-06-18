# HIDman
## USB HID to XT / AT / PS/2 / Serial converter

HIDman is an open source device to allow the use of modern USB keyboards and mice on legacy PCs.

Peripherals that support legacy PCs are becoming hard to find, especially for the very first PCs. This project aims to provide a simple cheap solution that will cover everything from the original IBM 5150 PC all the way up to modern computers that have PS/2 ports.

<img src=/images/hidmanfront.jpg width=50%/>
<img src=/images/hidmanrear.jpg width=50%/>


# Features

* (should) Support the majority of standard USB keyboards and mice, including ones that use wireless dongles.
* Also supports (some) USB Game controllers - buttons/axes are mapped to keypresses or mouse actions
* Can emulate the following devices :
* * OG IBM PC / XT Keyboard
  * AT / PS/2 Keyboard
  * Serial Mouse (with optional Logitech-style 3rd button)
  * PS/2 Mouse (with optional Intellipoint-style scroll wheel and up to 5 buttons)
* Driverless configuration menu - change settings just by opening a text editor


# Quickstart Guide

## USB Connection

You can connect many different combinations of USB devices to HIDman.

The most obvious being to connect a keyboard to one USB port, and a mouse to the other one :

<img src=/images/usbsetup1.svg width=300/>

Or, you could perhaps connect a wireless keyboard+mouse dongle to one port, and a game controller to the other :

<img src=/images/usbsetup2.svg width=400/>

Hub support can be hit-and-miss. This is (mostly) not HIDman's fault - many modern hubs don't support low-speed USB devices properly.


## Newer PCs (PS/2) ###

* Connect male-to-male Mini-DIN cables from HIDman’s keyboard and mouse ports to your PC’s PS/2 ports.
* Turn on your PC.
* If necessary, press HIDman's ⏻ power button to select PS/2 mode (light blue).

<img src=/images/newerpcs.svg width=400/>


## Older PCs (AT/XT and Serial)

* Connect a male-to-male Mini-DIN Cable from HIDman's keyboard port into a Mini-DIN to DIN adapter.
* Connect that adapter to your PC's keyboard port.
* Connect a straight-through male-to-female DE-9 cable from HIDman's serial port to your PC's serial port.
* Turn on your PC.
* Press HIDman's ⏻ power button to select AT mode (light blue), or XT mode (orange) for early 80's IBM PCs and clones.

<img src=/images/olderpcs.svg width=500/>


# Configuration

Configuration is provided through a built-in menu system. No software install is required - all interaction is through the keyboard.

Simply open a text editor on your target PC (eg notepad, edit, vi), then hold HIDman's power button for a few seconds. The menu will be typed out into your text editor.

"Adv.Keyboard" and "Adv.Mouse" switch the keyboard and mouse respectively into "Native Mode". This may be required to access certain functions such as scroll wheels and extra buttons on mice.

"Intellimouse" enables scroll wheel and 5-button support for PS/2 mice.

"Debug" enables the debug menu. The most useful debugging option is "Dump HID Data". Enabling this dumps information about all USB devices connected after that point. 

If you are having trouble getting a USB device to work, enable HID Dumping then connect the troublesome USB device. Wait for it to finish dumping, then press ESC and save the resulting data to a text file. Then send it to me for analysis.


## Technical description

The HIDman is based around the CH559 from WCH, a remarkably flexible chip with **two** USB HOST ports. This makes it ideal for our purposes.

The code is forked from atc1441's excellent repository - https://github.com/atc1441/CH559sdccUSBHost

PCB and enclosure was designed in KiCad - source files are in the hardware directory.

Development is very active but it is usable in its current state.
