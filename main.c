#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "ch559.h"
#include "util.h"
#include "usbhost.h"
#include "uart.h"
#include "ps2protocol.h"
#include "ps2.h"
#include "parsedescriptor.h"
#include "menu.h"
#include "mouse.h"

#if !defined(BOARD_MICRO)
	#define OPT_SERIAL_MOUSE
#endif

#if defined(BOARD_MICRO)        // Pinouts for HIDman-micro
	SBIT(KEY_CLOCK, 0x90, 7);
	#if defined(OPT_SWAP_KBD_MSC) // Makes it easier to direct solder combo PS/2 port
		SBIT(KEY_DATA, 0x90, 6);
		SBIT(MOUSE_CLOCK, 0x90, 4);
	#else
		SBIT(KEY_DATA, 0x90, 4);
		SBIT(MOUSE_CLOCK, 0x90, 6);
	#endif
	SBIT(MOUSE_DATA, 0x90, 5);
#else                           // Default pinouts (HIDman-AXD, HIDman-mini)
	SBIT(KEY_CLOCK, 0x80, 5);
	SBIT(KEY_DATA, 0x80, 3);

	SBIT(MOUSE_CLOCK, 0xB0, 7);
	SBIT(MOUSE_DATA, 0xC1, 3);
#endif

#if defined(OPT_SERIAL_MOUSE)
	#define SERIAL_MOUSE_MODE_OFF    0
	#define SERIAL_MOUSE_MODE_RESET  1
	#define SERIAL_MOUSE_MODE_INIT   2
	#define SERIAL_MOUSE_MODE_ACTIVE 3

	uint8_t serialMouseMode = SERIAL_MOUSE_MODE_OFF;
	__xdata char serialMouseType = '3'; // Logitech 3 button: '3', Microsoft: 'M'
#endif

void mTimer2Interrupt(void) __interrupt(5);

// timer should run at 48MHz divided by (0xFFFF - (TH0TL0))
// i.e. 60khz
void mTimer0Interrupt(void) __interrupt(1)
{
	// Reload to 60KHz

	PS2ProcessPort(PORT_KEY);
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

void EveryMillisecond() {

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

	static uint16_t ResetCounter = 0;
	if (!(P4_IN & (1 << 6))) {
		ResetCounter++;
		if (ResetCounter > 1000) {
			runBootloader();
		}
	} else {
		ResetCounter = 0;
	}

	// Turn red LED off (and green on) if we haven't seen any activity in a while
	if (LEDDelayMs) {
		LEDDelayMs--;
	} else {
#if defined(BOARD_MICRO)
		P2 |= 0b00100000;
#else
		P2 |= 0b00010000;
		P2 &= ~0b00100000;
#endif
	}
}

#define SetPWMClk(CK_SE) (PWM_CK_SE = CK_SE)   //Frequency division, default clock Fsys
#define SetPWMCycle(Cycle) (PWM_CYCLE = Cycle) //Set the cycle period
#define SetPWM1Dat(dat) (PWM_DATA = dat)	   //Set PWM output duty cycle
#define SetPWM2Dat(dat) (PWM_DATA2 = dat)
#define PWMPINAlter()              \
	{                              \
		P4_DIR |= bPWM2_ | bPWM1_; \
		PIN_FUNC |= bPWM1_PIN_X;   \
	} //Set PWM pin mapping

/************************************************* ******************************
* Function Name: InitPWM1(UINT8 polar)
* Description: PWM1 initialization function
* Input: polar=0 selects the default low level, high level output is valid;
                   polar=1 selects the default high level, the low level output is valid;
* Output: None
* Return: None
************************************************** *****************************/
void InitPWM1(UINT8 polar)
{
	PWM_CTRL &= ~bPWM_CLR_ALL; //Clear FIFO and count
	//PWM_CTRL &= ~bPWM_MOD_MFM;
	//PWM_CTRL |= bPWM_IE_END; //Enable PWM counting cycle completion interrupt
	PWM_CTRL |= bPWM_OUT_EN; //PWM1 output enable
	PWM_CTRL |= bPWM_IF_END; //Clear all interrupt flags
	if (polar)
	{
		PWM_CTRL |= bPWM_POLAR; //active low
	}
	else
	{
		PWM_CTRL &= ~bPWM_POLAR; //High level active
	}
}

/************************************************* ******************************
* Function Name: InitPWM2(UINT8 polar)
* Description: PWM initialization function
* Input: polar=0 selects the default low level, high level output is valid;
                   polar=1 selects the default high level, the low level output is valid;
* Output: None
* Return: None
************************************************** *****************************/
void InitPWM2(UINT8 polar)
{
	PWM_CTRL &= ~bPWM_CLR_ALL; //Clear FIFO and count
	//PWM_CTRL &= ~bPWM_MOD_MFM;
	//PWM_CTRL |= bPWM_IE_END;  //Enable PWM counting cycle completion interrupt
	PWM_CTRL |= bPWM2_OUT_EN; //PWM2 output enable
	PWM_CTRL |= bPWM_IF_END;  //Clear all interrupt flags
	if (polar)
	{
		PWM_CTRL |= bPWM2_POLAR; //active low
	}
	else
	{
		PWM_CTRL &= ~bPWM2_POLAR; //High level active
	}
}

/************************************************* ******************************
* Function Name: InitPWM2(UINT8 polar)
* Description: PWM initialization function
* Input: polar=0 selects the default low level, high level output is valid;
                   polar=1 selects the default high level, the low level output is valid;
* Output: None
* Return: None
************************************************** *****************************/
void InitPWM3(UINT8 polar)
{
	T3_CTRL &= ~bT3_CLR_ALL;
	T3_SETUP |= bT3_EN_CK_SE;
	T3_CTRL |= bT3_OUT_EN | bT3_CNT_EN | bT3_PWM_POLAR;

	PIN_FUNC |= bTMR3_PIN_X;
}

//PWM1, PWM2, PWM3_
void main()
{

	InitSystem();

#if defined(BOARD_MICRO)        // Pinouts for HIDman-micro
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
#else                           // Default pinouts (HIDman-AXD, HIDman-mini)
	//port0 setup
	P0_DIR = 0b11101010; // 0.3, 0.5, 0.6, 0.7 are all keyboard outputs, 0.4 is CTS (i.e. RTS on host), 0.1 is RTS (i.e. CTS on host)
	PORT_CFG |= bP0_OC;	  // open collector
	P0_PU = 0x00;		  // no pullups
	P0 = 0b11111010;	  // default pin states

	//port2 setup
	P2_DIR = 0b00110000; // 2.4, 2.5 are RED/GREEN LED outputs
	PORT_CFG |= bP2_OC;	  // open collector
	P2_PU = 0x00;		  // no pullups
	P2 = 0b00110000;	  // LEDs off by default (i.e. high)

	//port3 setup
	P3_DIR = 0b11100010; // 5,6,7 are PS2 outputs, 1 is UART0 TXD
	PORT_CFG |= bP3_OC;	  // open collector
	P3_PU = 0x00;		  // no pullups
	P3 = 0b11100010;	  // default pin states

	//port4 setup
	P4_DIR = 0b00010100; //4.0 is RXD, 4.2 is Blue LED, 4.3 is MOUSE DATA (actually input, since we're faking open drain), 4.4 is TXD, 4.6 is SWITCH
	P4_PU = 0b01000000;	 // pullup on switch
	P4_OUT = 0b00000100; //LEDs off (i.e. HIGH), MOUSE DATA low (since it's switched by toggling input on and off, i.e. faking open drain)
#endif

	// timer0 setup
	TMOD = (TMOD & 0xf0) | 0x02; // mode 1 (8bit auto reload)
	TH0 = 0xBD;					 // 60khz

	TR0 = 1; // start timer0
	ET0 = 1; //enable timer0 interrupt;
	EA = 1;	 // enable all interrupts

#if !defined(BOARD_MICRO)
	printf("Ready\n");

	// GREEN LED ON
	P2 &= ~0b00100000;
#endif

#if defined(OPT_SERIAL_MOUSE)
	uint32_t serialMouseBps = 1200; // can do 19200 with custom mouse driver
	CH559UART1Init(20, 1, 1, serialMouseBps, 8);
#endif

	memset(SendBuffer, 0, 255);
	//SendKeyboardString("We are go\n");
	uint8_t Buttons;
	uint8_t PrevButtons = 0;

	while (1)
	{
		if (!(P4_IN & (1 << 6)))
			MenuActive = 1;

		if (MenuActive)
			Menu_Task();

		ProcessUsbHostPort();
		ProcessKeyboardLed();
		HandleRepeats();

		int16_t X, Y;
		uint8_t byte1, byte2, byte3;

		// Send PS/2 Mouse Packet if necessary
		// make sure there's space in the buffer before we pop any mouse updates
		if ((ports[PORT_MOUSE].sendBuffEnd + 1) % 8 != ports[PORT_MOUSE].sendBuffStart)
		{
			if (GetMouseUpdate(0, -255, 255, &X, &Y, &Buttons))
			{

				// ps2 is inverted compared to USB
				Y = -Y;

				byte1 = 0b00001000 |			   //bit3 always set
						((Y >> 10) & 0b00100000) | // Y sign bit
						((X >> 11) & 0b00010000) | // X sign bit
						(Buttons & 0x07);

				byte2 = (X & 0xFF);
				byte3 = (Y & 0xFF);

				SendMouse3(byte1, byte2, byte3);
			}
		}

#if defined(OPT_SERIAL_MOUSE)
		if (serialMouseMode == SERIAL_MOUSE_MODE_INIT) {
			// Delay a bit and send 'M' to identify as a mouse
			delay(5);
			CH559UART1SendByte('M');
			if (serialMouseType != 'M') {
				// Delay a bit longer and send '2' or '3' to further identify # of buttons
				delay(10);
				CH559UART1SendByte(serialMouseType);
			}
			serialMouseMode = SERIAL_MOUSE_MODE_ACTIVE;
		}
		// Send Serial Mouse Packet if necessary
		// make sure there's space in the fifo before we pop any mouse updates
		else if (serialMouseMode == SERIAL_MOUSE_MODE_ACTIVE && (/*CH559UART1_FIFO_CNT >= 3 || */ SER1_LSR & bLSR_T_FIFO_EMP))
		{
			if (GetMouseUpdate(1, -127, 127, &X, &Y, &Buttons))
			{
				byte1 = 0b11000000 |			  // bit6 always set
						((Buttons & 0x01) << 5) | // left button
						((Buttons & 0x02) << 3) | // right button
						((Y >> 4) & 0b00001100) | // top two bits of Y
						((X >> 6) & 0b00000011); // top two bits of X

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

				P3 ^= 0b01000000;
			}
		}
#endif
	}
}
