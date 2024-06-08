#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "ch559.h"
#include "usbhost.h"
#include "uart.h"
#include "ps2protocol.h"
#include "ps2.h"
#include "parsedescriptor.h"
#include "menu.h"
#include "mouse.h"
#include "xt.h"
#include "pwm.h"
#include "keyboardled.h"
#include "dataflash.h"
#include "settings.h"
#include "system.h"



#if defined(BOARD_AXP)
	#define OPT_SERIAL_MOUSE
#endif



#if defined(OPT_SERIAL_MOUSE)
	#define SERIAL_MOUSE_MODE_OFF    0
	#define SERIAL_MOUSE_MODE_RESET  1
	#define SERIAL_MOUSE_MODE_INIT   2
	#define SERIAL_MOUSE_MODE_ACTIVE 3

	uint8_t serialMouseMode = SERIAL_MOUSE_MODE_OFF;
	__xdata char serialMouseType = '3'; // Logitech 3 button: '3', Microsoft: 'M'
#endif

/*void WDogInterrupt(void) __interrupt (INT_NO_WDOG) __using (1) {

}*/





uint8_t UsbUpdateCounter = 0;

void EveryMillisecond(void) {

	// every 4 milliseconds (125hz), check one or the other USB port
	if (UsbUpdateCounter == 4)
		s_CheckUsbPort0 = TRUE;
	else if (UsbUpdateCounter == 8){
		s_CheckUsbPort1 = TRUE;
	}

	UsbUpdateCounter++;
	

	// handle serial mouse
	#if defined(OPT_SERIAL_MOUSE)
		static uint8_t RTSHighCounter = 0;
		static __xdata uint8_t serialMousePrevMode = SERIAL_MOUSE_MODE_OFF;

		// High toggle (> 50ms) of RTS (P0.4) means host is resetting mouse.  Wait until falling edge and send mouse identification.

		if (P0 & 0b00010000) { // RTS is high (mouse is resetting)
			if (serialMouseMode != SERIAL_MOUSE_MODE_RESET) {
				serialMousePrevMode = serialMouseMode;
				serialMouseMode = SERIAL_MOUSE_MODE_RESET;
			}
			if (RTSHighCounter < 255) RTSHighCounter++;
			
		} else { // RTS is low
		if (serialMouseMode == SERIAL_MOUSE_MODE_RESET) {
				if (RTSHighCounter > 50) { // Check if RTS was high long enough to indicate reset...
					serialMouseMode = SERIAL_MOUSE_MODE_INIT;
				} else {
					serialMouseMode = serialMousePrevMode;
				}
				RTSHighCounter = 0;
			}
		}
	#endif

	// check the button
	inputProcess();

	// Turn current LED on if we haven't seen any activity in a while
	if (LEDDelayMs) {
		LEDDelayMs--;
	} else {
#if defined(BOARD_MICRO)
		P2 |= 0b00100000;
#elif defined (BOARD_PS2)
		P0 |= 0b01110000;
		if (LEDStatus & 0x01)
			P0 &= ~0b00010000;
		if (LEDStatus & 0x02)
			P0 &= ~0b00100000;
		if (LEDStatus & 0x04)
			P0 &= ~0b01000000;
#else
			SetPWM1Dat(0x00);
			SetPWM2Dat(0x00);
			T3_FIFO_L = 0;
			T3_FIFO_H = 0;

			if (LEDStatus & 0x01)
				SetPWM1Dat(0x30);
			if (LEDStatus & 0x02)
				SetPWM2Dat(0x30);
			if (LEDStatus & 0x04)
			{
				T3_FIFO_L = 0xFF; // blue needs to be brighter
				T3_FIFO_H = 0;
			}
#endif
	}
}

// timer should run at 48MHz divided by (0xFFFF - (TH0TL0))
// i.e. 60khz
void mTimer0Interrupt(void) __interrupt(INT_NO_TMR0)
{
	// Reload to 60KHz

	switch (FlashSettings->KeyboardMode) {
		case (MODE_PS2):
			PS2ProcessPort(PORT_KEY);
			break;

		case (MODE_XT):
			XTProcessPort();
			break;
	}

	// May as well do this even in XT mode, can't hurt
	PS2ProcessPort(PORT_MOUSE);

	// Handle keyboard typematic repeat timers
	// (divide timer down to 15KHz to make maths easier)
	static uint8_t repeatDiv = 0;
	if (++repeatDiv == 4) {
		repeatDiv = 0;
		RepeatTimer();
	}

	static uint8_t msDiv = 0;
	if (++msDiv == 60) {
		msDiv = 0;
		EveryMillisecond();
	}
}


uint8_t DetectCountdown = 0;
uint8_t PrevRTSState = 0;
uint8_t PrevButtons = 0;

#define testintsize(TP, SZ) \
	if (sizeof(TP) != SZ) printf("type " #TP " is not " #SZ " bytes! (%d instead)\n", sizeof(TP));


/*
	Func to test the sizes of various types
	Mainly just to remind me how big all the various weird pointers are
*/
/*void testintsizes(){

	// trad C - note ints and shorts are both 2 bytes
	testintsize(char, 1); testintsize(unsigned char, 1);
	testintsize(short, 2); testintsize(unsigned short, 2);
	testintsize(int, 2); testintsize(unsigned int, 2);
	testintsize(long, 4); testintsize(unsigned long, 4);
	testintsize(long long, 8); testintsize(unsigned long long, 8);

	// C99, this should be obvious but can't hurt to test
	testintsize(int8_t, 1); testintsize(uint8_t, 1);
	testintsize(int16_t, 2); testintsize(uint16_t, 2);
	testintsize(int32_t, 4); testintsize(uint32_t, 4);
	testintsize(int64_t, 8); testintsize(uint64_t, 8);

	// size_t is 2 bytes
	testintsize(size_t, 2);

	// stuff in xdata should work the same
	testintsize(__xdata uint8_t, 1); testintsize(__xdata uint32_t, 4);

	// generic pointers should be 24bit
	testintsize(uint8_t *, 3); testintsize(uint32_t *, 3);

	// code pointers should be 16bit
	testintsize(__code uint8_t *, 2); testintsize(__code uint32_t *, 2);

	// data pointers should be 8bit
	testintsize(__data uint8_t *, 1); testintsize(__data uint32_t *, 1);

	// idata pointers should be 8bit
	testintsize(__idata uint8_t *, 1); testintsize(__idata uint32_t *, 1);

	// xdata pointers should be 16bit
	testintsize(__xdata uint8_t *, 2); testintsize(__xdata uint32_t *, 2);

	// pdata pointers should be 8bit (they are paged 256-byte segments of xdata), page is selected by P2 (or an SFR? depends on 8051 variant)
	testintsize(__pdata uint8_t *, 1); testintsize(__pdata uint32_t *, 1);
}*/

void GPIOInit(){
	
#if defined(BOARD_MICRO) // Pinouts for HIDman-micro
	//port1 setup
	P1_DIR = 0b11110000; // 0.4, 0.5, 0.6, 0.7 are keyboard/mouse outputs
	PORT_CFG |= bP1_OC;	  // open collector
	P1_PU = 0x00;		  // no pullups
	P1 = 0b11110000;	  // default pin states

	//port2 setup
	P2_DIR = 0b00100000; // 2.5 is LED output
	PORT_CFG |= bP2_OC;	  // open collector
	P2_PU = 0x00;		  // no pullups
	P2 = 0b00100000;	  // LED off by default (i.e. high)

#elif defined(BOARD_PS2) // Pinouts for old PS2 version

	//port0 setup
	P0_DIR = 0b01110000; // 456 are red-green-blue LEDs
	PORT_CFG = bP0_OC;	 // Push-pull
	P0_PU = 0x00;		 // no pullups
	P0 = 0b01110000;	 // default pin states

	//port2 setup
	P2_DIR = 0b00000011; // 2.0/2.1 are PS2 outputs
	PORT_CFG |= bP2_OC;	 // open collector
	P2_PU = 0x00;		 // no pullups
	P3 = 0b00000011;	 // default pin states

	//port3 setup
	P3_DIR = 0b11111100; // 234567 are PS2 outputs, 1 is UART0 TXD
	PORT_CFG |= bP3_OC;	 // open collector
	P3_PU = 0b00000001; // pullup on 1 for TXD
	P3 = 0b11111100;	 // default pin states

	//port4 setup
	P4_DIR = 0b00000000; // 4.6 is SWITCH
	P4_PU = 0b01000000;	 // pullup on switch
	P4_OUT = 0b00000000;


#else					  // Default pinouts (HIDman-AXP, HIDman-mini)
	//port0 setup
	P0_DIR = 0b11101010; // 0.3, 0.5, 0.6, 0.7 are all keyboard outputs, 0.4 is CTS (i.e. RTS on host), 0.1 is RTS (i.e. CTS on host)
	PORT_CFG |= bP0_OC;	 // open collector
	P0_PU = 0x00;		 // no pullups
	P0 = 0b11111010;	 // default pin states

	//port2 setup
	P2_DIR = 0b00110000; // 2.4, 2.5 are RED/GREEN LED outputs
	PORT_CFG |= bP2_OC;	 // open collector
	P2_PU = 0x00;		 // no pullups
	P2 = 0b00110000;	 // LEDs off by default (i.e. high)

	//port3 setup
	P3_DIR = 0b11100010; // 4 is switch, 5,6,7 are PS2 outputs, 1 is UART0 TXD
	PORT_CFG |= bP3_OC;	 // open collector
	P3_PU = 0b00010001; // pullup on 1 for TXD, 4 for switch
	P3 = 0b11110010;	 // default pin states

	//port4 setup
	P4_DIR = 0b00010100; //4.0 is RXD, 4.2 is Blue LED, 4.3 is MOUSE DATA (actually input, since we're faking open drain), 4.4 is TXD, 4.6 is SWITCH
	P4_PU = 0b01000000;	 // pullup on switch
	P4_OUT = 0b00000100; //LEDs off (i.e. HIGH), MOUSE DATA low (since it's switched by toggling input on and off, i.e. faking open drain)
#endif

}

void ClockInit(void){
	#if defined(OSC_EXTERNAL)
	if (!(P3 & (1 << 4))) runBootloader();
#endif

	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;												 

#if defined(OSC_EXTERNAL)
	CLOCK_CFG |= bOSC_EN_XT;								  
	CLOCK_CFG &= ~bOSC_EN_INT;												
#endif

	CLOCK_CFG &= ~MASK_SYS_CK_DIV;

	CLOCK_CFG |= 6; 															  
	PLL_CFG = (24 << 0) | (6 << 5);
}

void main(void)
{
	bool WatchdogReset = 0;

	// Watchdog happened, go to "safe mode"
	if (!(PCON & bRST_FLAG0) && (PCON & bRST_FLAG1)){
		WatchdogReset = 1;
	}

	GPIOInit();
	mDelaymS(10);

	ClockInit();
    mDelaymS(500);   
	
#if !defined(BOARD_MICRO)
	InitUART0();
#endif

	InitUsbData();
	InitUsbHost();

	InitPWM();


	// timer0 setup
	TMOD = (TMOD & 0xf0) | 0x02; // mode 1 (8bit auto reload)
	TH0 = 0xBD;					 // 60khz

	TR0 = 1; // start timer0
	ET0 = 1; //enable timer0 interrupt;

	// enable watchdog
	WDOG_COUNT = 0x00;
	//GLOBAL_CFG |= bWDOG_EN;
	
	EA = 1;	 // enable all interrupts


#if defined(OPT_SERIAL_MOUSE)
	uint32_t serialMouseBps = 1200; // can do 19200 with custom mouse driver
	CH559UART1Init(20, 1, 1, serialMouseBps, 8);
#endif

	memset(SendBuffer, 0, 255);
	memset(MouseBuffer, 0, MOUSE_BUFFER_SIZE);


	uint8_t Buttons;
	uint8_t PrevButtons = 0;
	MOUSE *ps2Mouse = &OutputMice[MOUSE_PORT_PS2];

	if (WatchdogReset) DEBUG_OUT("Watchdog reset detected (%x), entering safemode\n", PCON);

	InitSettings(WatchdogReset);



	while (1)
	{

		// reset watchdog
		WDOG_COUNT = 0x00;

		if (MenuActive)
			Menu_Task();

		ProcessUsbHostPort();

		ProcessKeyboardLed();
		HandleRepeats();

		int16_t X, Y, Z;
		uint8_t byte1, byte2, byte3, byte4;

		// Send PS/2 Mouse Packet if necessary
		// make sure there's space in the buffer before we pop any mouse updates
		if ((ports[PORT_MOUSE].sendBuffEnd + 1) % 8 != ports[PORT_MOUSE].sendBuffStart)
		{
			if (GetMouseUpdate(0, -255, 255, &X, &Y, &Z, &Buttons, (ps2Mouse->Ps2Scaling==MOUSE_PS2_SCALING_2X), (3-ps2Mouse->Ps2Resolution)))
			{

				// ps2 is inverted compared to USB
				Y = -Y;

				// TODO: construct bytes from real state
				byte1 = 0b00001000 |			   //bit3 always set
						((Y >> 10) & 0b00100000) | // Y sign bit
						((X >> 11) & 0b00010000) | // X sign bit
						(Buttons & 0x07);

				byte2 = (X & 0xFF);
				byte3 = (Y & 0xFF);

				if (ps2Mouse->Ps2Type == MOUSE_PS2_TYPE_INTELLIMOUSE_3_BUTTON)
				{
					byte4 = (-Z & 0xFF);
					SendMouse4(byte1, byte2, byte3, byte4);
				}
				else if (ps2Mouse->Ps2Type == MOUSE_PS2_TYPE_INTELLIMOUSE_5_BUTTON)
				{
					byte4 = (-Z & 0b00001111) |    // wheel 
					((Buttons << 1) & 0b00110000); // buttons 4 and 5					
					SendMouse4(byte1, byte2, byte3, byte4);
				}
				else
				{
					SendMouse3(byte1, byte2, byte3);
				}
			}
		}

#if defined(OPT_SERIAL_MOUSE)
		if (serialMouseMode == SERIAL_MOUSE_MODE_INIT) {
			// Delay a bit and send 'M' to identify as a mouse
			mDelaymS(5);
			CH559UART1SendByte('M');
			if (serialMouseType != 'M') {
				// Delay a bit longer and send '2' or '3' to further identify # of buttons
				mDelaymS(10);
				CH559UART1SendByte(serialMouseType);
			}
			serialMouseMode = SERIAL_MOUSE_MODE_ACTIVE;
		}
		// Send Serial Mouse Packet if necessary
		// make sure there's space in the fifo before we pop any mouse updates
		else if (serialMouseMode == SERIAL_MOUSE_MODE_ACTIVE && (/*CH559UART1_FIFO_CNT >= 3 || */ SER1_LSR & bLSR_T_FIFO_EMP))
		{
			if (GetMouseUpdate(1, -127, 127, &X, &Y, &Z, &Buttons, false, 0))
			{
				byte1 = 0b11000000 |			  // bit6 always set
						((Buttons & 0x01) << 5) | // left button
						((Buttons & 0x02) << 3) | // right button
						((Y >> 4) & 0b00001100) | // top two bits of Y
						((X >> 6) & 0b00000011);  // top two bits of X

				byte2 = 0b10000000 | (X & 0x3F); // rest of X
				byte3 = 0b10000000 | (Y & 0x3F); // rest of Y

				CH559UART1SendByte(byte1);
				CH559UART1SendByte(byte2);
				CH559UART1SendByte(byte3);

				if (serialMouseType == '3')
				{
					if (Buttons & 0x04)
						CH559UART1SendByte(0b10100000);
					else if (PrevButtons & 0x04)
						CH559UART1SendByte(0b10000000);

					PrevButtons = Buttons;
				}
			}
		}
#endif
	}
}
