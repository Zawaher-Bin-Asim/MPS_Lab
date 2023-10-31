/******************************************************************************
*	Project name: Seven_seg_display
*	File name: Task2
*	Author: Fazail Ali Butt
*	Date: 11-10-2023
*	Description: This is a template file for GPIO parallel interfacing for
*				Seven Segment Display. This program is written for common anode
*				type seven segment display
* 				Seven segment digits pins:		PA2-PA5*
*				Seven segment data pins:		PB0-PB7*
*				Port B pins:					76543210*
*				Seven Segment LEDs:				pgfedcba*
 ******************************************************************************/

#include "TM4C123.h"

void SystemInit (void)
{
	  /* --------------------------FPU settings ----------------------------------*/
	#if (__FPU_USED == 1)
		SCB->CPACR |= ((3UL << 10*2) |                 /* set CP10 Full Access */
                  (3UL << 11*2)  );               /* set CP11 Full Access */
	#endif
}
	

/******************************************************************************
*Macros for Register Addresses and Values
*******************************************************************************/
/* Register for clock */
#define	SYSCTL_RCGCGPIO_R		(*((volatile unsigned long*)0x400FE608))

/* GPIO Registers for port B */	
#define	GPIO_PORTB_DATA_R		(*((volatile unsigned long*)0x400053FC))
#define	GPIO_PORTB_DIR_R		(*((volatile unsigned long*)0x40005400)) //PORTB = 0x40005000
#define	GPIO_PORTB_DEN_R		(*((volatile unsigned long*)0x4000551C))

/* GPIO Registers for port A */	
#define	GPIO_PORTA_DATA_R		(*((volatile unsigned long*)0x400043FC))
#define	GPIO_PORTA_DIR_R		(*((volatile unsigned long*)0x40004400)) //PORTA = 0x40004000
#define	GPIO_PORTA_DEN_R		(*((volatile unsigned long*)0x4000451C))

/* Values for enabling seven segments */
#define	SEG_1		0xFB   	//PA2 = 1111 1011 (DF)
#define	SEG_2		0xF7   	//PA3 = 1111 0111 (EF)
#define	SEG_3		0xEF	//PA4 = 1110 1111 (F7)
#define	SEG_4		0xDF	//PA5 = 1101 1111 (FB)
#define	SEG_OFF		0xFF 	//      1111 1111 (FF)

/* Function Declarations */	
void init_gpio(void);
void display_1(void);
void display_2(void);
void delay(unsigned long value);

/*Lookup tables for common anode display */

/*lut for display1*/
const char lut_display1[4]={0xC0,	//U -->(0xC1) 1100 0001 (0-->0xC0) 
							0xF9,	//O -->(0xC0) 1100 0000 (1-->0xF9)
							0x99,	//F -->(0x8E) 1000 1110 (4-->0x99) 
							0xA4	//S -->(0x92) 1001 0010 (2-->0xA4)
							};
							
/* lut for display2 */
const char lut_display2[4]={0xC0,	//O --> 1100 0000
							0xC7,	//L --> 1100 0111
							0x86,	//E --> 1000 0110
							0x89	//H --> 1000 1001
							};

/* lut for segment selection */
const char seg_select[4]={0xFB,	//SEG_1= 1111 1011 (PA2)
						 0xF7,	//SEG_2= 1111 0111 (PA3)
						 0xEF,	//SEG_3= 1110 1111 (PA4)
						 0xDF	//SEG_4= 1101 1111 (PA5)
						};
						
/*initialization function for ports */
void init_gpio(void){
	volatile unsigned long delay_clk;
	
	/*enable clock for PortA and PortB */
	SYSCTL_RCGCGPIO_R |= 0x03;
	
	// dummy read for delay for clock,must have 3sys clock delay
	delay_clk=SYSCTL_RCGCGPIO_R;
	
	/* Enable the GPIO pin for PortB pins 0-7 for digital function
	and set the direction as output. 1111 1111*/	
	GPIO_PORTB_DIR_R |= 0xFF;
	GPIO_PORTB_DEN_R |= 0xFF;
	
	/* Enable the GPIO pin for PortA pins 2-5 for digital function.
	and set the direction as output. 0011 1100*/	
	GPIO_PORTA_DIR_R |= 0x3C;
	GPIO_PORTA_DEN_R |= 0x3C;

}

/* display_1 on seven segments using Macros */
void display_1(void){
	GPIO_PORTA_DATA_R =SEG_OFF;
	GPIO_PORTB_DATA_R =lut_display1[3];
	GPIO_PORTA_DATA_R =SEG_1;
	delay(10000);
	GPIO_PORTA_DATA_R=SEG_OFF;
	GPIO_PORTB_DATA_R=lut_display1[2];
	GPIO_PORTA_DATA_R=SEG_2;
	delay(10000);
	GPIO_PORTA_DATA_R=SEG_OFF;
	GPIO_PORTB_DATA_R=lut_display1[1];
	GPIO_PORTA_DATA_R=SEG_3;
	delay(10000);
	GPIO_PORTA_DATA_R=SEG_OFF;
	GPIO_PORTB_DATA_R=lut_display1[0];
	GPIO_PORTA_DATA_R=SEG_4;
	delay(10000);
}

/* display_2 on seven segments using for loop */
void display_2(void){
	int i;
	for(i=0;i<4;i++)
	{
		GPIO_PORTA_DATA_R=SEG_OFF;
		GPIO_PORTB_DATA_R=lut_display2[i];
		GPIO_PORTA_DATA_R=seg_select[i];
		delay(10000);
	}
}

/* Delay function */
void delay(unsigned long value){
	unsigned long i ;
	for(i=0;i<value;i++);
}

/* Main function */
int main(void){
	int i;
	init_gpio();
	while(1)
	{
		for ( i=0 ; i <100 ; i++){
				display_1 ();}
		for ( i=0 ; i <100 ; i++){
				display_2 ();}
	}
}
