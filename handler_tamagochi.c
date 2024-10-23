#include "handler_tamagochi.h"
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include <stdio.h> 

#define WIDTH_BATTERY 42
#define HEIGHT_BATTERY 27
#define WIDTH_BAR 6 //40/5 also considering the space between the single bars and the edges of the battery
#define SPACE_BARS 2

volatile char time_in_char[20] = "";
extern int sec;
extern int sound_index;
extern int sound_meal[];
extern int sound_snack[];
extern int f_meal;


void draw_age(int hour, int min, int sec) {
  sprintf((char*) time_in_char, "AGE: %02d:%02d:%02d", hour, min, sec);
	GUI_Text(60, 20, (uint8_t *) time_in_char, Black, White);
}

void draw_battery (uint16_t num, uint16_t numBattery) { //num is the number of the bars
	uint16_t x = 0;
	uint16_t y = 0;
	uint16_t height_battery;
	if(numBattery == 1){
		x = 42;
		y = 63;
	}else if(numBattery == 2) {
		x = 157;
		y = 63;
	} else{
		return;
	}
	
	height_battery = HEIGHT_BATTERY - 5;
	LCD_DrawEmptyRectangle(x-2, y-3, WIDTH_BATTERY, HEIGHT_BATTERY, Black); //first battery
	LCD_DrawEmptyRectangle(x-2+WIDTH_BATTERY, y-3+5, 10, 15, Black); //right piece of the first battery
	
	switch(num) {
		case 0:
			LCD_DrawRectangle (x, y, WIDTH_BAR, height_battery, White);
			LCD_DrawRectangle (x + WIDTH_BAR + SPACE_BARS, y, WIDTH_BAR, height_battery, White);
			LCD_DrawRectangle (x + (WIDTH_BAR*2) + (SPACE_BARS*2), y, WIDTH_BAR, height_battery, White);
			LCD_DrawRectangle (x + (WIDTH_BAR*3) + (SPACE_BARS*3), y, WIDTH_BAR, height_battery, White);
			LCD_DrawRectangle (x + (WIDTH_BAR*4) + (SPACE_BARS*4), y, WIDTH_BAR, height_battery, White);
			break;
		case 1:
			LCD_DrawRectangle (x, y, WIDTH_BAR, height_battery, Red);
			LCD_DrawRectangle (x + WIDTH_BAR + SPACE_BARS, y, WIDTH_BAR, height_battery, White);
			LCD_DrawRectangle (x + (WIDTH_BAR*2) + (SPACE_BARS*2), y, WIDTH_BAR, height_battery, White);
			LCD_DrawRectangle (x + (WIDTH_BAR*3) + (SPACE_BARS*3), y, WIDTH_BAR, height_battery, White);
			LCD_DrawRectangle (x + (WIDTH_BAR*4) + (SPACE_BARS*4), y, WIDTH_BAR, height_battery, White);
			break;
		case 2:
			LCD_DrawRectangle (x, y, WIDTH_BAR, height_battery, Red);
			LCD_DrawRectangle (x + WIDTH_BAR + SPACE_BARS, y, WIDTH_BAR, height_battery, Magenta);
			LCD_DrawRectangle (x + (WIDTH_BAR*2) + (SPACE_BARS*2), y, WIDTH_BAR, height_battery, White);
			LCD_DrawRectangle (x + (WIDTH_BAR*3) + (SPACE_BARS*3), y, WIDTH_BAR, height_battery, White);
			LCD_DrawRectangle (x + (WIDTH_BAR*4) + (SPACE_BARS*4), y, WIDTH_BAR, height_battery, White);
			break;
		case 3:
			LCD_DrawRectangle (x, y, WIDTH_BAR, height_battery, Red);
			LCD_DrawRectangle (x + WIDTH_BAR + SPACE_BARS, y, WIDTH_BAR, height_battery, Magenta);
			LCD_DrawRectangle (x + (WIDTH_BAR*2) + (SPACE_BARS*2), y, WIDTH_BAR, height_battery, Magenta);
			LCD_DrawRectangle (x + (WIDTH_BAR*3) + (SPACE_BARS*3), y, WIDTH_BAR, height_battery, White);
			LCD_DrawRectangle (x + (WIDTH_BAR*4) + (SPACE_BARS*4), y, WIDTH_BAR, height_battery, White);
			break;
		case 4:
			LCD_DrawRectangle (x, y, WIDTH_BAR, height_battery, Red);
			LCD_DrawRectangle (x + WIDTH_BAR + SPACE_BARS, y, WIDTH_BAR, height_battery, Magenta);
			LCD_DrawRectangle (x + (WIDTH_BAR*2) + (SPACE_BARS*2), y, WIDTH_BAR, height_battery, Magenta);
			LCD_DrawRectangle (x + (WIDTH_BAR*3) + (SPACE_BARS*3), y, WIDTH_BAR, height_battery, Green);
			LCD_DrawRectangle (x + (WIDTH_BAR*4) + (SPACE_BARS*4), y, WIDTH_BAR, height_battery, White);
			break;
		case 5:
			LCD_DrawRectangle (x, y, WIDTH_BAR, height_battery, Red);
			LCD_DrawRectangle (x + WIDTH_BAR + SPACE_BARS, y, WIDTH_BAR, height_battery, Magenta);
			LCD_DrawRectangle (x + (WIDTH_BAR*2) + (SPACE_BARS*2), y, WIDTH_BAR, height_battery, Magenta);
			LCD_DrawRectangle (x + (WIDTH_BAR*3) + (SPACE_BARS*3), y, WIDTH_BAR, height_battery, Green);
			LCD_DrawRectangle (x + (WIDTH_BAR*4) + (SPACE_BARS*4), y, WIDTH_BAR, height_battery, Green);
			break;
		default:
			break;
	}
}

void basic_tamagochi (uint16_t xFaceStart, uint16_t yFaceStart, uint16_t xLeftEyeStart, uint16_t xRightEyeStart, uint16_t yEyeStart) {
	LCD_DrawLine(xFaceStart, yFaceStart, xFaceStart+80, yFaceStart, Black);
	LCD_DrawLine(xFaceStart, yFaceStart+80, xFaceStart+80, yFaceStart+80, Black);
	LCD_DrawLine(xFaceStart, yFaceStart, xFaceStart, yFaceStart+80, Black);
	LCD_DrawLine(xFaceStart+80, yFaceStart, xFaceStart+80, yFaceStart+80, Black);
	LCD_DrawCircle(xLeftEyeStart, yEyeStart, 10, Black);
	LCD_DrawCircle(xRightEyeStart, yEyeStart, 10, Black);
	LCD_DrawFullCircle(xLeftEyeStart, yEyeStart, 3, 0, 0, Green);
	LCD_DrawFullCircle(xRightEyeStart, yEyeStart, 3, 0, 0, Green);
}

void basic_tamagochi_mouth(uint16_t xFaceStart, uint16_t yFaceStart, uint16_t xLeftEyeStart, uint16_t xRightEyeStart, uint16_t yEyeStart, uint16_t xMouth, uint16_t yMouth, uint16_t mouthRadius, uint16_t yin, uint16_t yfin) {
	LCD_DrawLine(xFaceStart, yFaceStart, xFaceStart+80, yFaceStart, Black);
	LCD_DrawLine(xFaceStart, yFaceStart+80, xFaceStart+80, yFaceStart+80, Black);
	LCD_DrawLine(xFaceStart, yFaceStart, xFaceStart, yFaceStart+80, Black);
	LCD_DrawLine(xFaceStart+80, yFaceStart, xFaceStart+80, yFaceStart+80, Black);
	LCD_DrawCircle(xLeftEyeStart, yEyeStart, 10, Black);
	LCD_DrawCircle(xRightEyeStart, yEyeStart, 10, Black);
	LCD_DrawFullCircle(xLeftEyeStart, yEyeStart, 3, 0, 0, Green);
	LCD_DrawFullCircle(xRightEyeStart, yEyeStart, 3, 0, 0, Green);
	LCD_DrawFullCircle(xMouth, yMouth, mouthRadius, yin, yfin, Black);
}

void erase_tamagochi(uint16_t xFaceStart, uint16_t yFaceStart, uint16_t xLeftEyeStart, uint16_t xRightEyeStart, uint16_t yEyeStart, uint16_t xMouth, uint16_t yMouth, uint16_t mouthRadius, uint16_t yin, uint16_t yfin) {
	LCD_DrawLine(xFaceStart, yFaceStart, xFaceStart+80, yFaceStart, White);
	LCD_DrawLine(xFaceStart, yFaceStart+80, xFaceStart+80, yFaceStart+80, White);
	LCD_DrawLine(xFaceStart, yFaceStart, xFaceStart, yFaceStart+80, White);
	LCD_DrawLine(xFaceStart+80, yFaceStart, xFaceStart+80, yFaceStart+80, White);
	//for some unknown reason the eraseration doesn't work properly so i draw only full white circles bigger than the original ones (just 1 pixel more)
	LCD_DrawFullCircle(xLeftEyeStart, yEyeStart, 11, 0, 0, White);
	LCD_DrawFullCircle(xRightEyeStart, yEyeStart, 11, 0, 0, White);
	LCD_DrawFullCircle(xLeftEyeStart, yEyeStart, 4, 0, 0, White);
	LCD_DrawFullCircle(xRightEyeStart, yEyeStart, 4, 0, 0, White);
	LCD_DrawFullCircle(xMouth, yMouth, mouthRadius+1, yin, yfin, White);
}

void snack(void) {
	int i = 0; //xTamagochiStart
	int xLeftEyeStart = 100;
	int xRightEyeStart = 140;
	int xMouthStart = 120;
	LCD_DrawFullCircle(175, 170, 10, 0, 0, Magenta);
	LCD_DrawFullCircle(175, 170, 3, 0, 0, White);
	erase_tamagochi(80, 120, 100, 140, 140, 120, 170, 15, 15, 0); //erase the original tamagochi's mouth
	
	for(i=80; i<160; i++) {
		basic_tamagochi_mouth(i, 120, xLeftEyeStart, xRightEyeStart, 140, xMouthStart, 170, 18, 0, 0);
		erase_tamagochi(i, 120, xLeftEyeStart, xRightEyeStart, 140, xMouthStart, 170, 18, 0, 0);
		//basic_tamagochi_mouth(i, 120, xLeftEyeStart, xRightEyeStart, 140, xMouthStart, 170, 5, 0, 0);
		//erase_tamagochi(i, 120, xLeftEyeStart, xRightEyeStart, 140, xMouthStart, 170, 5, 0, 0);
		xLeftEyeStart++;
		xRightEyeStart++;
		xMouthStart++;
	}
	
	basic_tamagochi_mouth(80, 120, 100, 140, 140, 120, 170, 15, 15, 0);
}

void leaving_tamagochi(void) {
	int i = 0; //xTamagochiStart
	int xLeftEyeStart = 100;
	int xRightEyeStart = 140;
	int xMouthStart = 120;
	
	int xFirstLeftTear = xLeftEyeStart - 10;
	int xFirstRightTear = xRightEyeStart + 10;
	int tearRadius = 3;
	
	erase_tamagochi(80, 120, 100, 140, 140, 120, 170, 15, 15, 0); //erase the original tamagochi's mouth
	for(i=80; i<240; i++) {
		
		basic_tamagochi_mouth(i, 120, xLeftEyeStart, xRightEyeStart, 140, xMouthStart, 180, 15, 0, 15);
		LCD_DrawFullCircle(xFirstLeftTear, 158, tearRadius, 0, 0, Cyan); //eye left's tears
		LCD_DrawFullCircle(xFirstLeftTear, 165, tearRadius, 0, 0, Cyan);
		LCD_DrawFullCircle(xFirstLeftTear, 172, tearRadius, 0, 0, Cyan);
		LCD_DrawFullCircle(xFirstRightTear, 158, tearRadius, 0, 0, Cyan); //eye right's tears
		LCD_DrawFullCircle(xFirstRightTear, 165, tearRadius, 0, 0, Cyan);
		LCD_DrawFullCircle(xFirstRightTear, 172, tearRadius, 0, 0, Cyan);
		
		erase_tamagochi(i, 120, xLeftEyeStart, xRightEyeStart, 140, xMouthStart, 180, 15, 0, 15);
		LCD_DrawFullCircle(xFirstLeftTear, 158, tearRadius+1, 0, 0, White); //eye left's tears cancellation
		LCD_DrawFullCircle(xFirstLeftTear, 165, tearRadius+1, 0, 0, White);
		LCD_DrawFullCircle(xFirstLeftTear, 172, tearRadius+1, 0, 0, White);
		LCD_DrawFullCircle(xFirstRightTear, 158, tearRadius+1, 0, 0, White); //eye right's tears
		LCD_DrawFullCircle(xFirstRightTear, 165, tearRadius+1, 0, 0, White);
		LCD_DrawFullCircle(xFirstRightTear, 172, tearRadius+1, 0, 0, White);
		
		/*basic_tamagochi_mouth(i, 120, xLeftEyeStart, xRightEyeStart, 140, xMouthStart, 180, 15, 0, 15);
		//LCD_DrawFullCircle(xFirstLeftTear, 160, tearRadius, 0, 0, Cyan);
		//LCD_DrawFullCircle(xFirstRightTear, 160, tearRadius, 0, 0, Cyan);
		
		erase_tamagochi(i, 120, xLeftEyeStart, xRightEyeStart, 140, xMouthStart, 180, 15, 0, 15);
		//LCD_DrawFullCircle(xFirstLeftTear, 160, tearRadius+1, 0, 0, White);
		//LCD_DrawFullCircle(xFirstRightTear, 160, tearRadius+1, 0, 0, White);*/
		
		xLeftEyeStart++;
		xRightEyeStart++;
		xMouthStart++;
		xFirstLeftTear++;
		xFirstRightTear++;
	}
}

void meal(void) {
	
	int i = 0; //xTamagochiStart
	int xLeftEyeStart = 100;
	int xRightEyeStart = 140;
	int xMouthStart = 120;
	LCD_DrawFullCircle(180, 170, 15, 0, 0, Red);
	erase_tamagochi(80, 120, 100, 140, 140, 120, 170, 15, 15, 0); //erase the original tamagochi's mouth
	
	for(i=80; i<160; i++) {
		if(i==85){
			disable_timer(3);
			reset_timer(3);
		}
		
		basic_tamagochi_mouth(i, 120, xLeftEyeStart, xRightEyeStart, 140, xMouthStart, 170, 18, 0, 0);
		erase_tamagochi(i, 120, xLeftEyeStart, xRightEyeStart, 140, xMouthStart, 170, 18, 0, 0);
		//basic_tamagochi_mouth(i, 120, xLeftEyeStart, xRightEyeStart, 140, xMouthStart, 170, 5, 0, 0);
		//erase_tamagochi(i, 120, xLeftEyeStart, xRightEyeStart, 140, xMouthStart, 170, 5, 0, 0);
		xLeftEyeStart++;
		xRightEyeStart++;
		xMouthStart++;
	}
	basic_tamagochi_mouth(i, 120, xLeftEyeStart, xRightEyeStart, 140, xMouthStart, 170, 18, 0, 0);
	
}

void cuddle1(void) {
	int i = 0;
	for(i=0;i<20;i++){
	LCD_DrawFullCircle(100, 140, 6, 0, 0, Green);
	LCD_DrawFullCircle(140, 140, 6, 0, 0, Green);
	}
	LCD_DrawFullCircle(100, 140, 6, 0, 0, White);
	LCD_DrawFullCircle(140, 140, 6, 0, 0, White);
}

void cuddle2(void) {
	int i = 0;
	for(i=0;i<20;i++){
	LCD_DrawFullCircle(100, 140, 6, 0, 0, Red);
	LCD_DrawFullCircle(140, 140, 6, 0, 0, Red);
	}
	LCD_DrawFullCircle(100, 140, 6, 0, 0, White);
	LCD_DrawFullCircle(140, 140, 6, 0, 0, White);
}

void init_draw (void) {
	draw_age(0,0,0);
	GUI_Text(30, 40, (uint8_t *) "HAPPINESS", Black, White);
	GUI_Text(150, 40, (uint8_t *) "SATIETY", Black, White);
	
	draw_battery(5, 1); //first battery, linked to Happiness
	draw_battery(5, 2); //second battery, linked to Satiety
	
	GUI_Text(35, 280, (uint8_t *) " MEAL ", Black, White);
	LCD_DrawEmptyRectangle(0, 260, 118, 60, Black);
	GUI_Text(150, 280, (uint8_t *) " SNACK ", Black, White);
	LCD_DrawEmptyRectangle(122, 260, 120, 60, Black);
	
	basic_tamagochi_mouth(80, 120, 100, 140, 140, 120, 170, 15, 15, 0);
	
	return;
} 

void volume(void)
{
	int i, j;
	for(i = 0; i <=16; i++) {
		if(i == 0) {
			for(j = 1; j <= 8; j++) {
				if(j==8) {
					draw_square(j, i, Black, 2, 1, 2);
				}
			}
		}
		if(i == 1) {
			for(j = 1; j <= 8; j++) {
				if(j >=7 ) {
					draw_square(j, i, Black, 2, 1, 2);
				}
			}
		}
		if(i == 2) {
			for(j = 1; j <= 8; j++) {
				if(j>=6) draw_square(j, i, Black, 2, 1, 2);
			}
		}
		if(i == 3) {
			for(j = 1; j <=  8; j++) {
				if(j >= 5) draw_square(j, i, Black, 2, 1, 2);
			}
		}
		if(i == 4) {
			for(j = 1; j <=  8; j++) {
				
				if(j >= 1) draw_square(j, i, Black, 2, 1, 2);
			}
		}
		if(i == 5) {
			for(j = 1; j <=  8; j++) {
				
				if(j >= 1) draw_square(j, i, Black, 2, 1, 2);
			}
		}
		if(i == 6) {
			for(j = 1; j <=  8; j++) {
				
				if(j >= 1) draw_square(j, i, Black, 2, 1, 2);
			}
	}
		if(i == 7) {
			for(j = 1; j <=  8; j++) {
				
				if(j >= 1) draw_square(j, i, Black, 2, 1, 2);
			}
	}
		if(i == 8) {
			for(j = 1; j <=  8; j++) {
				
				if(j >= 1) draw_square(j, i, Black, 2, 1, 2);
			}
	}
		if(i == 9) {
			for(j = 1; j <=  8; j++) {
				
				if(j >= 1) draw_square(j, i, Black, 2, 1, 2);
			}
	}
		if(i == 10) {
			for(j = 1; j <=  8; j++) {
				
				if(j >= 1) draw_square(j, i, Black, 2, 1, 2);
			}
	}
		if(i == 11) {
			for(j = 1; j <=  8; j++) {
				
				if(j >= 1) draw_square(j, i, Black, 2, 1, 2);
			}
	}
		if(i == 12) {
			for(j = 1; j <=  8; j++) {
				
				if(j >= 5) draw_square(j, i, Black, 2, 1, 2);
			}
	}
		
		if(i == 13) {
			for(j = 1; j <= 8; j++) {
				if(j>=6) {
					draw_square(j, i, Black, 2, 1, 2);
				}
			}
		}
		if(i == 14) {
			for(j = 1; j <= 8; j++) {
				if(j >=7 ) {
					draw_square(j, i, Black, 2, 1, 2);
				}
			}
		}
		if(i == 15) {
			for(j = 1; j <= 8; j++) {
				if(j>=8) draw_square(j, i, Black, 2,1,2);
			}
		}
	}

}

void loud(int amplitude){
	
	int i;
	
	switch(amplitude) {
		
		case 0:
			for(i=0;i<24;i++)
		{
			LCD_SetPoint(23,8+i,White);
			LCD_SetPoint(24,8+i,White);//1
			LCD_SetPoint(27,8+i,White);
			LCD_SetPoint(28,8+i,White);//2
			LCD_SetPoint(31,8+i,White);
			LCD_SetPoint(32,8+i,White);//3

		}
			
		break;
		case 1:
				LCD_SetPoint(27,11,White);
			LCD_SetPoint(28,11,White);//2
			LCD_SetPoint(31,11,White);
			LCD_SetPoint(32,11,White);//3
			LCD_SetPoint(31,10,White);
			LCD_SetPoint(32,10,White);//3
				for(i=0;i<15;i++)
		{
			LCD_SetPoint(23,12+i,Black);
			LCD_SetPoint(24,12+i,Black);//1
			LCD_SetPoint(27,12+i,White);
			LCD_SetPoint(28,12+i,White);//2
			LCD_SetPoint(31,12+i,White);
			LCD_SetPoint(32,12+i,White);//3

		}
		LCD_SetPoint(27,27,White);
			LCD_SetPoint(28,27,White);//
		LCD_SetPoint(31,27,White);
			LCD_SetPoint(32,27,White);//3
			LCD_SetPoint(31,28,White);
			LCD_SetPoint(32,28,White);//3
			break;
		
		case 2:
			LCD_SetPoint(31,11,White);
			LCD_SetPoint(32,11,White);//3
			LCD_SetPoint(31,10,White);
			LCD_SetPoint(32,10,White);//3
			LCD_SetPoint(27,11,Black);
			LCD_SetPoint(28,11,Black);//2
					for(i=0;i<15;i++)
		{
			LCD_SetPoint(23,12+i,Black);
			LCD_SetPoint(24,12+i,Black);//1
			LCD_SetPoint(27,12+i,Black);
			LCD_SetPoint(28,12+i,Black);//2
			LCD_SetPoint(31,12+i,White);
			LCD_SetPoint(32,12+i,White);//3

		}
			LCD_SetPoint(27,27,Black);
			LCD_SetPoint(28,27,Black);//
		LCD_SetPoint(31,27,White);
			LCD_SetPoint(32,27,White);//3
			LCD_SetPoint(31,28,White);
			LCD_SetPoint(32,28,White);//3
			
		break;
		case 3: 
			LCD_SetPoint(27,11,Black);
			LCD_SetPoint(28,11,Black);//2
			LCD_SetPoint(31,11,Black);
			LCD_SetPoint(32,11,Black);//3
			LCD_SetPoint(31,10,Black);
			LCD_SetPoint(32,10,Black);//3
					for(i=0;i<15;i++)
		{
			LCD_SetPoint(23,12+i,Black);
			LCD_SetPoint(24,12+i,Black);//1
			LCD_SetPoint(27,12+i,Black);
			LCD_SetPoint(28,12+i,Black);//2
			LCD_SetPoint(31,12+i,Black);
			LCD_SetPoint(32,12+i,Black);//3

		}
			LCD_SetPoint(27,27,Black);
			LCD_SetPoint(28,27,Black);//
			LCD_SetPoint(31,27,Black);
			LCD_SetPoint(32,27,Black);//3
			LCD_SetPoint(31,28,Black);
			LCD_SetPoint(32,28,Black);//3
		
	}
	
}

