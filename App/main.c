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
/*******************************************************************************
 * Definition
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    program_state_t state = START;
    run_mechine_data_t mechineData;
    mechineData.runEx = 1;
    mechineData.runSpeed = 1;
    mechineData.runTime = 0;
    mechineData.dis = 0;
    mechineData.incl = 0;
    char key;
    KEYPAD_Init();
    SYSTICK_Init();
	while(1)
    {
       switch (state)
       {
            case START:
                state = start_mode();
                break;
            case RUN:
                break;
            case STOP:
                break;
            case EXERCISE_SET:
                state = exercise_mode(&mechineData);
                break;
            case USER_SET:
                break;
            default:
                break;
       }
	}
}