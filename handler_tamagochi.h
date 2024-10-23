#ifndef __HANDLER_TAMAGOCHI_H
#define __HANDLER_TAMAGOCHI_H
#include "LPC17xx.h"

void init_draw(void);
void draw_age(int hour, int min, int sec);
void draw_battery (uint16_t num, uint16_t numBattery);
void basic_tamagochi (uint16_t xFaceStart, uint16_t yFaceStart, uint16_t xLeftEyeStart, uint16_t xRightEyeStart, uint16_t yEyeStart);
void basic_tamagochi_mouth(uint16_t xFaceStart, uint16_t yFaceStart, uint16_t xLeftEyeStart, uint16_t xRightEyeStart, uint16_t yEyeStart, uint16_t xMouth, uint16_t yMouth, uint16_t mouthRadius, uint16_t yin, uint16_t yfin);
void erase_tamagochi(uint16_t xFaceStart, uint16_t yFaceStart, uint16_t xLeftEyeStart, uint16_t xRightEyeStart, uint16_t yEyeStart, uint16_t xMouth, uint16_t yMouth, uint16_t mouthRadius, uint16_t yin, uint16_t yfin);
void snack(void);
void meal(void);
void leaving_tamagochi(void);
void cuddle1(void);
void cuddle2(void);
void volume(void);
void loud(int amplitude);

#endif
