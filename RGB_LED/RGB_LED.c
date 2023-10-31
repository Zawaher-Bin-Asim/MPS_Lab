/*********************************************
* 		Project name: Digital Input/Output
* 		File name: Digi_I_O
* 		Author: Fazail Ali Butt
* 		Date: October 1, 2023
* 		Description: LED Blinking (This experiment 
*			is basically to learn how to find the 
*			addresses of the registers.)
*
*********************************************/

#include "TM4C123.h "
#define SYSCTL_RCGCGPIO_R			(*((volatile unsigned long *)0x400FE608))

#define GPIO_PORTF_DATA_R  		(*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R  		(*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_DEN_R  		(*((volatile unsigned long *)0x4002551C))

#define GPIO_PORTF_CLK_EN			0x20

//#define GPIO_PORTF_PIN_EN		0x02  // Red  = Pin1
//#define GPIO_PORTF_PIN_EN		0x04  // Blue = Pin2
#define GPIO_PORTF_PIN_EN		0x08  // Green = Pin3

//#define LED_ON								0x02	// Red  = Pin1
//#define LED_ON								0x04	// Blue = Pin2
#define LED_ON								0x08	// Green = Pin3

//#define LED_OFF								~(0x02) // Red  = Pin1
//#define LED_OFF								~(0x04) // Blue = Pin2
#define LED_OFF								~(0x08) // Green = Pin3

#define DELAY 								200000

void SystemInit (void)
{
	  /* --------------------------FPU settings ----------------------------------*/
	#if (__FPU_USED == 1)
		SCB->CPACR |= ((3UL << 10*2) |                 /* set CP10 Full Access */
                  (3UL << 11*2)  );               /* set CP11 Full Access */
	#endif
}

int main(void)
{
	volatile unsigned long ulLoop;
	
	// Enable the GPIO port that is used for the on-board LED.
	SYSCTL_RCGCGPIO_R |= GPIO_PORTF_CLK_EN;
	
	// Do a dummy read to insert a few cycles after enabling the peripheral.
	ulLoop = SYSCTL_RCGCGPIO_R;
	
	/* Enable the GPIO pin for the LED (PF3). Set the direction as ouput and
	   enable the GPIO pin for the digital function. */
	GPIO_PORTF_DIR_R = GPIO_PORTF_PIN_EN;
	GPIO_PORTF_DEN_R = GPIO_PORTF_PIN_EN;
	
	// Loop forever
	while (1)
	 {
		
			// Turn on the LED.
			GPIO_PORTF_DATA_R |= LED_ON;
		
			// Delay for a bit.
			for ( ulLoop = 0; ulLoop < DELAY; ulLoop ++)
			{
			}
			
			// Turn off the LED.
			GPIO_PORTF_DATA_R &= LED_OFF;
			
			// Delay for a bit.
			for ( ulLoop = 0; ulLoop < DELAY; ulLoop ++)
			{
			}
	 }
}
