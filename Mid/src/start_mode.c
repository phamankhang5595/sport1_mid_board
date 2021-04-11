#include "start_mode.h"
#include "keypad.h"
#include "common.h"
#include "screen.h"
#include "systick.h"
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief The start mode
 *
 * @param mechineData
 * @param laststate 
 * @return State of program
*/
program_state_t start_mode(run_mechine_data_t *mechineData, program_state_t *laststate)
{
    static program_state_t stateReturn;
    char key = NO_KEY;
    /* start display */
    if(IsDataChanged == YES)
    {
        /* update screen */
        reset_run_mechineData(mechineData);
        /* update screen */
        updateSpeed(mechineData->speed);            /* Speed */
        updateCalo(mechineData->calo);              /* Calo */
        updateDistance(mechineData->distance);      /* Distance */
        updateIncline(mechineData->incline);        /* Incline */
        updateTime(mechineData->runTime);           /* Run time */
        IsDataChanged = NO;
    }
    SYSTICK_Delay_ms(20);
    key = KEYPAD_ScanKey();
    if(key == EXE_KEY)
    {
        IsDataChanged = YES;
        stateReturn = EXERCISE_SET;
    }
    else if(key == RUN_KEY)
    {
        IsDataChanged = YES;
        stateReturn = RUN;
    }
    else if(key == SETUP_KEY)
    {
        IsDataChanged = YES;
        stateReturn = SET_UP;
    }
    else
        stateReturn = START;
    
    *laststate = START;
    return (stateReturn);
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
