/*
 * Copyright (c) 2021
 * Vitech, JSC.
 * All Rights Reserved
 *
 *
 * Description:
 *
 * Author: Khangpa
 *
 * Last Changed By:  $Author: Khangpa $
 * Revision:         $Revision: xxx $
 * Last Changed:     $Date: xxx $
 */
#include "safety_checking.h"
#include "keypad.h"
#include "stdint.h"
#include "common.h"
#include "systick.h"
#include "start_mode.h"
#include "setup_mode.h"
#include "exercise_mode.h"
#include "run_mode.h"
#include "board.h"
#include "stop_mode.h"
#include "stm32f10x_gpio.h"
#include "screen.h"
#include "power_communicate.h"
#include "stm32f10x_usart.h"
#include "dfplayer.h"

/*******************************************************************************
* Variable
*******************************************************************************/
run_mechine_data_t treadmillData;
volatile uint8_t  WarrningFlag = 0;
/*******************************************************************************
 * Definition
 ******************************************************************************/
/*******************************************************************************
* Private func
*******************************************************************************/
void CheckSafeKey()
{

}
/*******************************************************************************
 * Code
 ******************************************************************************/
 int main(void)
{
    program_state_t laststate = START;
    program_state_t state = START;
    treadmillData.runEx = 1;
    treadmillData.speed = 0;
    treadmillData.runTime = 0;
    treadmillData.distance = 0;
    treadmillData.incline = 0;
    SYSTICK_Init();
    SCREEN_Init();
    SCREEN_Clear();
    KEYPAD_Init();
    POWER_COM_Init();
    DF_Init(20);
    DF_PlayFromStart();
    //SAFETY_CHECK_Init();
    while(1)
    {
        /* prog interrupt check connetion */
        if(WarrningFlag != 0)
        {
            state = FAULT;
        }
        switch (state)
        {
            case START:
                state = start_mode(&treadmillData,&laststate);
                break;
            case RUN:
                state = run_mode(&treadmillData,&laststate);
                break;
            case STOP:
                state = stop_mode(&treadmillData,&laststate);
                break;
            case EXERCISE_SET:
                state = exercise_mode(&treadmillData,&laststate);
                break;
            case SET_UP:
                state = setup_mode(&treadmillData,&laststate);
                break;
            case FAULT:
                SCREEN_ERROR(1);
            default:
                break;
        }
    }
}
