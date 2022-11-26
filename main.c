#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "CH559.h"
#include "util.h"
#include "USBHost.h"
#include "uart.h"
#include "protocol.h"
#include "ps2.h"
#include "parsedescriptor.h"
#include "menu.h"

SBIT(LED, 0x90, 6);
/*SBIT(KEY_CLOCK, 0xB0, 4);
SBIT(KEY_DATA, 0xB0, 5);

SBIT(MOUSE_CLOCK, 0xA0, 0);
SBIT(MOUSE_DATA, 0xA0, 1);*/

SBIT(KEY_CLOCK, 0x80, 5);
SBIT(KEY_DATA, 0x80, 3);

SBIT(MOUSE_CLOCK, 0xB0, 7);
SBIT(MOUSE_DATA, 0xC1, 3);

typedef struct color
{
	uint8_t r, g, b;
} color;

color fadeseq[] =
	{
		{0x00, 0x00, 0xFF},
		{0x00, 0xFF, 0xFF},
		{0x00, 0xFF, 0x00},
		{0xFF, 0xFF, 0x00},
		{0xFF, 0x00, 0x00},
		{0xFF, 0x00, 0xFF},
		{0xFF, 0xFF, 0xFF},

};

uint8_t *fadepnt;

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
	//PORT_CFG &= ~bP2_OC;
	/*P2_DIR |= bPWM1 | bPWM2; //It is recommended to set the pin to push-pull output when turning on PWM

	SetPWMClk(12); //Set the clock division factor of PWM1&2 to 12
	InitPWM1(1);   //PWM1 initialization, active low
	InitPWM2(1);   //PWM2 initialization, active high
	InitPWM3(1);
	SetPWMCycle(0xff);
	SetPWM1Dat(0x00);
	SetPWM2Dat(0x00);

	P0 = 0xFF;
	P0_PU = 0x00;
	PORT_CFG |= bP0_DRV;

	P2 = 0xFF;
	P2_PU = 0x00;
	PORT_CFG |= bP2_DRV;

	P4_DIR = 0xFF;
	P4_OUT = 0xFF;
	P4_PU = 0x00;
	P4_CFG |= bP4_DRV;




	T3_CK_SE_L = 0x20;
	T3_CK_SE_H = 0;
	T3_END_H = 0;
	T3_END_L = 255;
	T3_FIFO_L = 0;
	T3_FIFO_H = 0;
	uint8_t bright = 0;
	uint8_t seq = 0, r = 0, g = 0, b = 0, nothere = 0;*/

	/*
			while (bright < 254)
		{
			bright++;
			//SetPWM1Dat(bright);
			T3_FIFO_L = bright;
			T3_FIFO_H = 0;
			//T3_FIFO_L = (uint8_t)(((uint16_t)255-bright * (uint16_t)255-bright) >> 8);
			//SetPWM2Dat((uint8_t)(((uint16_t)255-bright * (uint16_t)255-bright) >> 8));
			delayUs(2000);
		};
		while (bright > 1)
		{
			bright--;
			//SetPWM1Dat(bright);
			T3_FIFO_L = bright;
			T3_FIFO_H = 0;
			//T3_FIFO_L = (uint8_t)(((uint16_t)255-bright * (uint16_t)255-bright) >> 8);
			//SetPWM1Dat((uint8_t)(((uint16_t)bright * (uint16_t)bright) >> 8));

			//SetPWM2Dat((uint8_t)(((uint16_t)255-bright * (uint16_t)255-bright) >> 8));
			delayUs(2000);
		};
	*/
	/*	while (1)
	{
		nothere = 0;
		if (fadeseq[seq].r > r)
		{
			r++;
			nothere = 1;
		}
		else if (fadeseq[seq].r < r)
		{
			r--;
			nothere = 1;
		}

		if (fadeseq[seq].g > g)
		{
			g++;
			nothere = 1;
		}
		else if (fadeseq[seq].g < g)
		{
			g--;
			nothere = 1;
		}

		if (fadeseq[seq].b > b)
		{
			b++;
			nothere = 1;
		}
		else if (fadeseq[seq].b < b)
		{
			b--;
			nothere = 1;
		}

		SetPWM1Dat((uint8_t)(((uint16_t)r * (uint16_t)r) >> 10));
		SetPWM2Dat((uint8_t)(((uint16_t)g * (uint16_t)g) >> 10));
		T3_FIFO_L = (uint8_t)(((uint16_t)b * (uint16_t)b) >> 8);
		T3_FIFO_H = 0;

		if (!nothere)
			seq++;

		if (seq == 7)
			seq = 0;

		delayUs(1000);
	}
	*/

	InitSystem();

	//port0 setup
	P0_DIR |= 0b11101000; // 0.3, 0.5, 0.6, 0.7 are all keyboard outputs
	PORT_CFG |= bP0_OC;	  // open collector
	P0_PU = 0x00;		  // no pullups
	P0 = 0b11101000;	  // default high

	//port2 setup
	P2_DIR |= 0b00110000; // 2.4, 2.5 are RED/GREEN LED outputs
	PORT_CFG |= bP2_OC;	  // open collector
	P2_PU = 0x00;		  // no pullups
	P2 = 0b00110000;	  // LEDs off by default (i.e. high)

	//port4 setup
	P4_DIR = 0b00010100; //4.0 is RXD, 4.2 is Blue LED, 4.3 is MOUSE DATA (actually input, since we're faking open drain), 4.4 is TXD, 4.6 is SWITCH
	P4_PU = 0x00;		 // no pullups
	P4_OUT = 0b00000100; //LEDs off (i.e. HIGH), MOUSE DATA low (since it's switched by toggling input on and off, i.e. faking open drain)

	// Switch on LEDs
	/*P2 &= ~0b00110000;
	P4_OUT &= ~0b00000100;*/

	// timer0 setup
	TMOD = (TMOD & 0xf0) | 0x02; // mode 1 (8bit auto reload)
	TH0 = 0xBD;					 // 60khz

	TR0 = 1; // start timer0
	ET0 = 1; //enable timer0 interrupt;
	EA = 1;	 // enable all interrupts

	/*while (1)
	{

		if (GetPort(PORT_MOUSE, DATA)){
			//if (P4_IN &= 0b00001000){
			P2 &= ~0b00110000;
			P4_OUT &= ~0b00000100;
		}
		else {
			P2 |= 0b00110000;
			P4_OUT |= 0b00000100;
		}
	}*/

	printf("Ready\n");

	/*OutPort(PORT_KEY, DATA, 1);
	OutPort(PORT_KEY, CLOCK, 1);

	OutPort(PORT_MOUSE, DATA, 1);
	OutPort(PORT_MOUSE, CLOCK, 1);*/

	memset(SendBuffer, 0, 255);

	SendKeyboardString("We are go\n");

	while (1)
	{
		/*if (!(P4_IN & (1 << 6)))
			MenuActive = 1;

		if (MenuActive)
			Menu_Task();*/

		ProcessUsbHostPort();
		ProcessKeyboardLed();
		HandleRepeats();
	}
}

/*int main()
{
	InitSystem();

	while (TRUE)
	{

		//printf("heh\n");
		ProcessUsbHostPort();

		ProcessKeyboardLed();
	}
	
	return 0;
}

*/