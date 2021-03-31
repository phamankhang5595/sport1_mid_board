#include "start_mode.h"
#include "keypad.h"
#include "common.h"
#include "screen.h"
#include "systick.h"
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief 
 *
 * @param 
 * @param 
 */
program_state_t start_mode(run_mechine_data_t *mechineData)
{
    program_state_t stateReturn;
    char key = NO_KEY;
    /* start display */
    if(IsDataChanged == YES)
    {
        /* update screen */
        updateCalo(mechineData->calo);
        updateDistance(mechineData->distance);
        updateIncline(mechineData->incline);
        updateSpeed(mechineData->speed);
        updateTime(mechineData->runTime);
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
        stateReturn = USER_SET;
    }
    else
        stateReturn = START;
    return (stateReturn);
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
