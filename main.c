#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "CH559.h"
#include "util.h"
#include "USBHost.h"
#include "uart.h"
#include "ps2protocol.h"
#include "ps2.h"
#include "parsedescriptor.h"
#include "menu.h"
#include "mouse.h"

#if defined(BOARD_MICRO)      // Pinouts for HIDman-micro
	SBIT(KEY_CLOCK, 0x90, 7);
	#if defined(SWAP_KBD_MSC)   // Makes it easier to direct solder combo PS/2 port
		SBIT(KEY_DATA, 0x90, 6);
		SBIT(MOUSE_CLOCK, 0x90, 4);
	#else
		SBIT(KEY_DATA, 0x90, 4);
		SBIT(MOUSE_CLOCK, 0x90, 6);
	#endif
	SBIT(MOUSE_DATA, 0x90, 5);
#else                         // Default pinouts (HIDman-AXD, HIDman-mini)
	SBIT(KEY_CLOCK, 0x80, 5);
	SBIT(KEY_DATA, 0x80, 3);

	SBIT(MOUSE_CLOCK, 0xB0, 7);
	SBIT(MOUSE_DATA, 0xC1, 3);
#endif

__xdata uint8_t repeatDiv = 0;
uint16_t ResetCounter;

void mTimer2Interrupt(void) __interrupt(5);

// timer should run at 48MHz divided by (0xFFFF - (TH0TL0))
// i.e. 60khz
void mTimer0Interrupt(void) __interrupt(1)
{
	// Reload to 60KHz

	PS2ProcessPort(PORT_KEY);
	PS2ProcessPort(PORT_MOUSE);

	// now handle keyboard typematic repeat timers
	// divide down to 15KHz to make maths easier
	if (++repeatDiv == 4)
	{
		RepeatTimer();
		repeatDiv = 0;

		if (!(P4_IN & (1 << 6)))
		{
			ResetCounter++;
			if (ResetCounter > 10000)
			{
				runBootloader();
			}
		}
		else
			ResetCounter = 0;
			
		// turn red LED off (and green on) if we haven't seen any activity in a while
		if (LEDDelay)
			LEDDelay--;
		else
		{
#if defined(BOARD_MICRO)
			P2 |= 0b00100000;
#else
			P2 |= 0b00010000;
			P2 &= ~0b00100000;
#endif
		}
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

uint8_t DetectCountdown = 0;
uint8_t PrevRTSState = 0;

//PWM1, PWM2, PWM3_
void main()
{

	InitSystem();

#if defined(BOARD_MICRO)      // Pinouts for HIDman-micro
	//port1 setup
	P1_DIR |= 0b11110000; // 0.4, 0.5, 0.6, 0.7 are keyboard/mouse outputs
	PORT_CFG |= bP1_OC;	  // open collector
	P1_PU = 0x00;		  // no pullups
	P1 = 0b11110000;	  // default high

	//port2 setup
	P2_DIR |= 0b00100000; // 2.5 is LED output
	PORT_CFG |= bP2_OC;	  // open collector
	P2_PU = 0x00;		  // no pullups
	P2 = 0b00100000;	  // LED off by default (i.e. high)
#else                         // Default pinouts (HIDman-AXD, HIDman-mini)
	//port0 setup
	P0_DIR |= 0b11101000; // 0.3, 0.5, 0.6, 0.7 are all keyboard outputs, 0.4 is CTS (i.e. RTS on host)
	PORT_CFG |= bP0_OC;	  // open collector
	P0_PU = 0x00;		  // no pullups
	P0 = 0b11101000;	  // default high

	//port2 setup
	P2_DIR |= 0b00110000; // 2.4, 2.5 are RED/GREEN LED outputs
	PORT_CFG |= bP2_OC;	  // open collector
	P2_PU = 0x00;		  // no pullups
	P2 = 0b00110000;	  // LEDs off by default (i.e. high)

	//port3 setup
	P3_DIR |= 0b11100010; // 5,6,7 are PS2 outputs, 1 is UART0 TXD
	PORT_CFG |= bP3_OC;	  // open collector
	P3_PU = 0x00;		  // no pullups
	P3 = 0b11100010;	  // default high

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

	CH559UART1Init(20, 1, 1);
#endif

	memset(SendBuffer, 0, 255);
	//SendKeyboardString("We are go\n");
	uint8_t Buttons;
	int16_t X, Y;
	while (1)
	{
		if (!(P4_IN & (1 << 6)))
			MenuActive = 1;

		if (MenuActive)
			Menu_Task();

		ProcessUsbHostPort();
		ProcessKeyboardLed();
		HandleRepeats();

		uint8_t byte1, byte2, byte3;

		// Send PS/2 Mouse Packet if neccessary
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

#if !defined(BOARD_MICRO)
		// falling edge of RTS (P0.4) means host is resetting mouse
		if (!(P0 & 0b00010000) && PrevRTSState)
		{
			DetectCountdown = 20;
		}

		PrevRTSState = P0 & 0b00010000;

		// send a bunch of "M"s to identify MS mouse
		if (DetectCountdown)
		{
			if (SER1_LSR & bLSR_T_FIFO_EMP)
			{
				CH559UART1SendByte('M');
				DetectCountdown--;
			}
		}
		// Send Serial Mouse Packet if neccessary
		// make sure there's space in the fifo before we pop any mouse updates
		else if (/*CH559UART1_FIFO_CNT >= 3 || */ SER1_LSR & bLSR_T_FIFO_EMP)
		{
			if (GetMouseUpdate(1, -127, 127, &X, &Y, &Buttons))
			{
				byte1 = 0b11000000 |			  // bit6 always set
						((Buttons & 0x01) << 5) | // left button
						((Buttons & 0x02) << 3) | // right button
						((Y >> 4) & 0b00001100) | // top two bits of Y
						((X >> 11) & 0b00000011); // top two bits of X

				byte2 = 0b10000000 | (X & 0x3F); // rest of X
				byte3 = 0b10000000 | (Y & 0x3F); // rest of Y

				CH559UART1SendByte(byte1);
				CH559UART1SendByte(byte2);
				CH559UART1SendByte(byte3);
				P3 ^= 0b01000000;
			}
		}
#endif
	}
}
