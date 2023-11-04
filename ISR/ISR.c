/******************************************************************************
*	Project name: Interrupt and ISR programming
*	File name: I_ISR
*	Author: Fazail Ali Butt
*	Date: 25-10-2023
*	Description: This is Interrupt configuration for User button connected to 
*                PF4. It turns on different LEDs on falling edge of button 
*                press.
 ******************************************************************************/


#include "TM4C123.h"
/******************************************************************************
*Macros for Register Addresses and Values
*******************************************************************************/
#define SYSCTL_RCGCGPIO_R			(*((volatile unsigned long *)0x400FE608))

#define NVIC_EN0_R					(*((volatile unsigned long *)0xE000E100))
#define NVIC_PRI7_R					(*((volatile unsigned long *)0xE000E41C))

#define GPIO_PORTF_DATA_R  			(*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R  			(*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_DEN_R	  		(*((volatile unsigned long *)0x4002551C))

#define GPIO_PORTF_PU_R				(*((volatile unsigned long *)0x40025510))

#define GPIO_PORTF_IS_R				(*((volatile unsigned long *)0x40025404))
#define GPIO_PORTF_IBE_R			(*((volatile unsigned long *)0x40025408))
#define GPIO_PORTF_IEV_R			(*((volatile unsigned long *)0x4002540C))
#define GPIO_PORTF_IM_R 			(*((volatile unsigned long *)0x40025410))
#define GPIO_PORTF_ICR_R			(*((volatile unsigned long *)0x4002541C))

#define GPIO_PORTF_CLK_EN			0x20
#define INT_PF4 					0x10
#define GPIO_PORTF_PIN123_EN		0x0E
#define EN_PORTF_IN30				0x40000000
#define PRI7_SET5					0x00A00000

/* Function Declarations */	
/*----------------------These are functions added in startup file----------------------*/
void EnableInterrupts(void);							// 	Enable interrupts		
void DisableInterrupts(void);							// 	Disableinterrupts
void EnablePriorityInterrupts(void);					// 	Enable Priority interrupts						
void WaitForInterrupt(void);							// 	Implements WFI

/*--------------------------These are C frunctions of this file------------------------*/
void Init_INT_GPIO(void);								// Initialize GPIO and Interrupts
void Delay(unsigned long value);						// Implements delay

volatile unsigned long i=0;								// Global Variable


void SystemInit (void)
{
	  /* --------------------------FPU settings ----------------------------------*/
	#if (__FPU_USED == 1)
		SCB->CPACR |= ((3UL << 10*2) |                 	// set CP10 Full Access
                  (3UL << 11*2)  );               		// set CP11 Full Access
	#endif
}


void Init_INT_GPIO(void){
	volatile unsigned delay_clk;

	SYSCTL_RCGCGPIO_R |= GPIO_PORTF_CLK_EN;				// Enable clock for PORTF

	delay_clk = SYSCTL_RCGCGPIO_R;						// Dummy read to stable the clock
	
	GPIO_PORTF_DIR_R |= GPIO_PORTF_PIN123_EN;  			// Output direction for pin-123
	GPIO_PORTF_DIR_R &= ~(INT_PF4); 					// input direction for pin-4
	GPIO_PORTF_DEN_R |= ((INT_PF4) | 
						(GPIO_PORTF_PIN123_EN));		// Enable digital I/O on PF4,PF3-PF1

	GPIO_PORTF_PU_R |= INT_PF4;							// Enable weak pullup on PF4


	/*------------------------------ INTERRUPT Configuration------------------------------- */

	DisableInterrupts();								// Globally disable all interrupts

	GPIO_PORTF_IM_R &= 0x00;							// Disable all interrupts on PortF

	NVIC_EN0_R  |= EN_PORTF_IN30;						// Enable PortF interrupt in NVIC
	NVIC_PRI7_R |= PRI7_SET5;							// Set priority of PortF as 5

	GPIO_PORTF_IS_R  &= (~INT_PF4);						// PF4 as edge sensitive
	GPIO_PORTF_IBE_R &= (~INT_PF4);						// PF4 as not both edges
	GPIO_PORTF_IEV_R &= (~INT_PF4);						// PF4 as falling edge

	GPIO_PORTF_ICR_R |= INT_PF4;						// Clear interrupt flag for PF4
	GPIO_PORTF_IM_R  |= INT_PF4;							// Enable interrupt on PF4

	EnablePriorityInterrupts();							// Enable interrupts beyond the defined priority level
	EnableInterrupts();									// Globally enable all interrupts
}


void Delay(unsigned long value){
	unsigned long i=0;
	for(i=0;i<value;i++);
}

/* Interrupt Service Routine for PortF */
void GPIOF_Handler(void){
		int j;
		/* Clear interrupt flag for PF4 */
		GPIO_PORTF_ICR_R |= INT_PF4;
		
		/* Turn on different LEDs on each button press */
		if(i>=3)
		i=1;
		else
		i++;
		for(j=0;j<2;j++)
		{
			GPIO_PORTF_DATA_R^=1<<i;
			Delay(10000000);
		}
}

/* Main Function */
int main(){
	Init_INT_GPIO();
	while(1)
	{
		WaitForInterrupt();
	}
}
