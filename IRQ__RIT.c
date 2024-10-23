/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RITT.h"
#include "GLCD/GLCD.h"
#include "_joystick.h"
#include "handler_tamagochi.h"
#include "./timer/timer.h"
#include "./TouchPanel/TouchPanel.h"
#include "adc.h"


/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

	extern int dead;
	volatile int food = -1;
	extern int hours, min, sec, sec_update_happiness, sec_update_satiety, numBarsHappiness, numBarsSatiety, temp1, temp2;
	int count = 0;
	extern int f_meal;
	extern int f_snack;
	extern int f_cuddle;
	extern int animation, flag1, sound_index;

void RIT_IRQHandler (void)
{			
	static int select = 0;
	static int J_left = 0;
	static int J_right = 0;
	
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0) {
		select++;
		switch(select){
			case 1:
				//disable_timer(0);
				if(dead == 0){
						disable_timer(1);
						reset_timer(1);
						init_timer1(1, 12500000);
					if(J_left >= 1){
						f_meal = 1;
						f_snack = 0;
						f_cuddle = 0;
						enable_timer(1);
						numBarsSatiety++;
						sec_update_satiety++;
						//reset_timer(0);
						//enable_timer(0);
					}else if(J_right >= 1){
						f_meal = 0;
						f_snack = 1;
						f_cuddle = 0;
						enable_timer(1);
						numBarsHappiness++;
						sec_update_happiness++;
						
						//reset_timer(0);
						//enable_timer(0);
					}
				}else{
						LCD_Clear(White);
						f_cuddle = 0;
						f_snack = 0;
						f_meal = 0;
						temp1 = 0;
						temp2 = 0;
						hours = 0;
						min = 0;
						sec = 0;
						numBarsHappiness = 5;
						numBarsSatiety = 5;
						sec_update_happiness = 5;
						sec_update_satiety = 5;
						dead = 0;
						flag1 = 0;
						sound_index = 0;
						animation = 0;
					  init_draw();
						volume();
						reset_timer(0);
						//reset_timer(1);
						animation = 0;
						enable_timer(0);
					}
				break;
			default:
				break;
		}
	}
	else{
		select = 0;
	}
	
	if(dead == 0) {
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0) {
		J_left++;
		switch(J_left){
			case 1:
				J_right = 0;
				//disable_timer(0);
				GUI_Text(35, 280, (uint8_t *) " MEAL ", Red, White);
				LCD_DrawEmptyRectangle(0, 260, 118, 60, Red);
				GUI_Text(150, 280, (uint8_t *) " SNACK ", Black, White);
				LCD_DrawEmptyRectangle(122, 260, 120, 60, Black);
				//enable_timer(0);
			break;
			default:
				break;
		}
	}else{
		//J_left = 0;
		//GUI_Text(35, 280, (uint8_t *) " MEAL ", Black, White);
		//LCD_DrawEmptyRectangle(0, 260, 118, 60, Black);
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0) {
		J_right++;
		switch(J_right){
			case 1:
				J_left = 0;
				//disable_timer(0);
				GUI_Text(150, 280, (uint8_t *) " SNACK ", Red, White);
				LCD_DrawEmptyRectangle(122, 260, 120, 60, Red);
				GUI_Text(35, 280, (uint8_t *) " MEAL ", Black, White);
				LCD_DrawEmptyRectangle(0, 260, 118, 60, Black);
				//enable_timer(0);
			break;
			default:
				break;
		}
	}else {
		//J_right = 0;
		//GUI_Text(150, 280, (uint8_t *) " SNACK ", Black, White);
		//LCD_DrawEmptyRectangle(122, 260, 120, 60, Black);
		}
	}
	
	ADC_start_conversion();
	
	reset_RIT();
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return; 
	}
 

/******************************************************************************
**                            End Of File
******************************************************************************/
