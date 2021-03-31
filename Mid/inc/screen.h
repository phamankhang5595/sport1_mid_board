#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "common.h"
/*******************************************************************************
 * API
 ******************************************************************************/

void updateTime(uint32_t time);

void updateDistance(float dis);

void updateCalo(float cal);

void updateIncline(uint8_t incl);

void updateSpeed(float speed);

void updateEx(uint8_t runEx);

void mainScreen();

#endif /*_SCREEN_H_*/