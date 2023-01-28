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