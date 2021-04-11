#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef enum 
{
    RUN_TIME = 0,
    SET_UP
}State_Update_Data;
/*******************************************************************************
 * API
 ******************************************************************************/
void clear_Dot(unsigned char *data,uint8_t address);

void clear_SET_Time(uint32_t time,uint8_t address);

void display_Set(unsigned char *data,uint8_t address);

void updateTime(uint32_t time);

void updateDistance(uint32_t dis);

void updateCalo(uint32_t cal);

void updateIncline(uint8_t incl);

void updateSpeed(uint32_t speed);

void updateEx(uint8_t runEx);

void mainScreen();

void waittingScreen(run_mechine_data_t *mechineData);

char delay_and_scand(uint32_t ms);
#endif /*_SCREEN_H_*/