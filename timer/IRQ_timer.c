/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include <stdio.h>
#include "../handler_tamagochi.h"
#include "../RITT.h"

	volatile int sec_update_happiness = 5;
	volatile int sec_update_satiety = 5;
	volatile int sec = 0;
	volatile int min = 0;
	volatile int hours = 0;
	volatile int numBarsHappiness = 5;
	volatile int numBarsSatiety = 5;
	volatile int dead = 0;
	volatile int f_meal = 0;
	volatile int f_snack = 0;
	volatile int f_cuddle = 0;
	volatile int flag1 = 0;
	extern int food;
	volatile int temp1 = 0, temp2 = 0;
	extern int sound;
	extern int amplitude;
	volatile int sound_index = 0;
	volatile const int sound_snack[4]={1592, 0,1125,1263};
	volatile const int sound_meal[4]={1263, 0,1592,2120}; 
	volatile const int sound_cuddle[8]={1890, 1062, 1125, 1592, 1417, 1592, 1684, 1417};
	volatile const int sound_death[8]={2249,2120,2249,2120,2249,4240,2834,3779};
	volatile int animation = 0;
	
	uint16_t SinTable[45] =                                       
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};


/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER0_IRQHandler (void)
{
	if(animation == 0){
		if(sec%2==0){
		LCD_DrawFullCircle(100, 140, 5, 0, 0, White);
		LCD_DrawFullCircle(140, 140, 5, 0, 0, White);
		LCD_DrawFullCircle(100, 140, 3, 0, 0, Green);
		LCD_DrawFullCircle(140, 140, 3, 0, 0, Green);
		}else{

		LCD_DrawFullCircle(100, 140, 5, 0, 0, Green);
		LCD_DrawFullCircle(140, 140, 5, 0, 0, Green);
	}
}
	
	if(sec==59)
	{
		sec=0;
		min++;
	}
	if(min==59)
	{
		min=0;
		sec=0;
		hours++;
	}
	if(hours==24)
	{
		min=0;
		sec=0;
		hours=0;
	}
	else {
		sec++;
	}

	sec_update_happiness--;
	sec_update_satiety--;
	
	draw_age(hours, min, sec);
	draw_battery(numBarsHappiness, 1);
	draw_battery(numBarsSatiety, 2);
	
	if(sec_update_happiness == 0) {
		numBarsHappiness--;
		draw_battery(numBarsHappiness, 1);
		sec_update_happiness = 5;
	}
	if(sec_update_satiety == 0){
		numBarsSatiety--;
		draw_battery(numBarsSatiety, 2);
		sec_update_satiety = 5;
	}
		if(numBarsHappiness == 0 || numBarsSatiety == 0){
			dead = 1;
			disable_timer(1);
			reset_timer(1);
			init_timer1(1, 12500000);
			enable_timer(1);
			disable_timer(0);
		}
		
		if(dead == 0){
			if(temp1 > 0){
				if(temp2 == 0){
						disable_timer(1);
						reset_timer(1);
						init_timer1(1, 12500000);
						enable_timer(1);
						numBarsHappiness++;
						sec_update_happiness++;
						cuddle1();
						temp2++;
					}else if(temp2 == 1) {
						cuddle2();
						temp2++;
					}else if(temp2 == 2){
						cuddle1();
						temp2++;
					}else if(temp2 == 3){
						cuddle2();
						temp2 = 0;
						temp1 = 0;
					}
			}
	}
	
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
	//disable_timer(0);
	
	if (dead == 1)
	{
		disable_timer(3);
		reset_timer(3);
		
		init_timer1(3,sound_death[sound_index]);
		enable_timer(3);
		sound_index++;
		if(sound_index == 0){
			erase_tamagochi(80, 120, 100, 140, 140, 120, 170, 15, 15, 0);
			sound_index++;
		}else if(sound_index == 3){
			sound_index++;
			animation = 1;
			leaving_tamagochi();
			GUI_Text(35, 280, (uint8_t *) " MEAL ", White, White);
			LCD_DrawEmptyRectangle(0, 260, 118, 60, White);
			GUI_Text(150, 280, (uint8_t *) " SNACK ", White, White);
			LCD_DrawEmptyRectangle(122, 260, 120, 60, White);
			LCD_DrawEmptyRectangle(80, 120, 80, 80, Black);
			GUI_Text(110, 150, (uint8_t*) "END", Red, White);
			LCD_DrawEmptyRectangle(0, 260, 240, 60, Red);
			GUI_Text(95, 280, (uint8_t*) "RESTART", Blue, White);
		}
		else if(sound_index==8)
		{
				sound_index = 0;
				disable_timer(3);
				reset_timer(3);
				flag1=0;
				disable_timer(1);
		}else if(dead == 0){
			sound_index++;
			init_timer1(3, sound_death[sound_index]);
			enable_timer(3);
		}
	}else if(f_meal == 1 && dead == 0) {
		disable_timer(3);
		reset_timer(3);
		
		if(sound_index == 0 && dead == 0){
			init_timer1(3, sound_meal[sound_index]);
			sound_index++;
			enable_timer(3);
		}else if(sound_index == 1 && dead == 0){
			animation = 1;
			meal();
			sound_index++;
		}else if(sound_index == 4 && dead == 0){
			erase_tamagochi(160, 120, 180, 220, 140, 200, 170, 18, 0, 0);
			basic_tamagochi_mouth(80, 120, 100, 140, 140, 120, 170, 15, 15, 0);
			animation = 0;
			sound_index = 0;
			disable_timer(3);
			reset_timer(3);
			f_meal = 0;
			flag1 = 0;
			disable_timer(1);
		}else if(dead == 0){
			sound_index++;
			init_timer1(3, sound_meal[sound_index]);
			enable_timer(3);
		}
	
	}
	else if (f_snack == 1 && dead == 0) {
		
		disable_timer(3);
		reset_timer(3);
		
		if(sound_index == 0 && dead == 0){
			init_timer1(3, sound_snack[sound_index]);
			sound_index++;
			enable_timer(3);
		}else if(sound_index == 1 && dead == 0){
			animation = 1;
			meal();
			sound_index++;
		}else if(sound_index == 4 && dead == 0){
			erase_tamagochi(160, 120, 180, 220, 140, 200, 170, 18, 0, 0);
			basic_tamagochi_mouth(80, 120, 100, 140, 140, 120, 170, 15, 15, 0);
			animation = 0;
			sound_index = 0;
			disable_timer(3);
			reset_timer(3);
			f_snack = 0;
			flag1 = 0;
			disable_timer(1);
		}else if(dead == 0){
			sound_index++;
			init_timer1(3, sound_meal[sound_index]);
			enable_timer(3);
		}
		
		}else if (f_cuddle==1 && dead == 0)
	{
		disable_timer(3);
		reset_timer(3);
		
		if(sound_index == 0 && dead == 0){
			init_timer1(3, sound_cuddle[sound_index]);
			sound_index++;
			enable_timer(3);
		}else if((sound_index == 1 || sound_index == 2) && dead == 0){
			animation = 1;
			init_timer1(3, sound_cuddle[sound_index]);
			enable_timer(3);
			sound_index++;
		}else if(sound_index == 8 && dead == 0){
			animation = 0;
			sound_index = 0;
			disable_timer(3);
			reset_timer(3);
			f_cuddle = 0;
			flag1 = 0;
			disable_timer(1);
		}else if(dead == 0){
			sound_index++;
			init_timer1(3, sound_meal[sound_index]);
			enable_timer(3);
		}
	}
	
	
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}


void TIMER2_IRQHandler (void)
{
    if(getDisplayPoint(&display, Read_Ads7846(), &matrix)){
			if(((display.x>=80) && (display.x<=160)) && ((display.y>=120)&&(display.y<=200))){
				temp1++;
				f_cuddle = 1;
				f_meal = 0;
				f_snack = 0;
				
		}
	}
    LPC_TIM2->IR =  1 ;      /* clear interrupt flag */  
	
  return;
}

void TIMER3_IRQHandler (void)
{
	static int ticks=0;
	/* DAC management */	
	LPC_DAC->DACR = (SinTable[ticks]*amplitude)<<6;
	ticks++;
	if(ticks==45) ticks=0;
	
	LPC_TIM3->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
