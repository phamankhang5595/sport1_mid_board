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
#include "keypad.h"
#include "stdint.h"
#include "common.h"
#include "systick.h"
#include "start_mode.h"
#include "setup_mode.h"
#include "exercise_mode.h"
#include "run_mode.h"
#include "stop_mode.h"
#include "screen.h"
#include "lcd.h"
/*******************************************************************************
 * Definition
 ******************************************************************************/
/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    program_state_t laststate = START;
    program_state_t state = START;
    run_mechine_data_t mechineData;
    mechineData.runEx = 1;
    mechineData.speed = 0;
    mechineData.runTime = 0;
    mechineData.distance = 0;
    mechineData.incline = 0;
    GPIO_LcdInit();
    lcd_init();
    lcd_clr();
    KEYPAD_Init();
    SYSTICK_Init();
	while(1)
    {
        switch (state)
        {
            case START:
                 state = start_mode(&mechineData,&laststate);
                 break;
            case RUN:
                 state = run_mode(&mechineData,&laststate);
                 break;
            case STOP:
                 state = stop_mode(&mechineData,&laststate);
                 break;
            case EXERCISE_SET:
                 state = exercise_mode(&mechineData,&laststate);
                 break;
            case SET_UP:
                 state = setup_mode(&mechineData,&laststate);
                 break;
            default:
                 break;
        }
	}
}