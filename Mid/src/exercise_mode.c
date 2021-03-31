#include "exercise_mode.h"
#include "keypad.h"
#include "screen.h"
/*******************************************************************************
 * Variable
 ******************************************************************************/
/*******************************************************************************
 * Code
 ******************************************************************************/

program_state_t exercise_mode(run_mechine_data_t *mechineData)
{
    static program_state_t stateReturn;
    char key;
    if(IsDataChanged == YES)
    {
        /* update screen */
        updateEx(mechineData->runEx);               /* exercise */
        updateCalo(mechineData->calo);              /* Calo */
        updateDistance(mechineData->distance);      /* Distance */
        updateIncline(mechineData->incline);        /* Incline */
        updateTime(mechineData->runTime);           /* Run time */
        IsDataChanged = NO;
    }    
    /* Scan key */
    key = KEYPAD_ScanKey();
    /* Executive */
    switch (key)
    {
        case EXE_KEY:
            mechineData->runEx += 1;
            if(mechineData->runEx > MAX_RUN_EX)
                mechineData->runEx = DEFAULT_RUN_EX;
            IsDataChanged = YES;
            stateReturn = EXERCISE_SET;
            break;
        case PLUS_KEY:
            mechineData->runTime += 1;
            if(mechineData->runTime > 120)
                mechineData->runTime = 120;
            IsDataChanged = YES;
            stateReturn = EXERCISE_SET;
            break;
        case MINUS_KEY:
            mechineData->runTime -= 1;
            if(mechineData->runTime < 1)
                mechineData->runTime = 1;
            IsDataChanged = YES;
            stateReturn = EXERCISE_SET;
            break;
        case UP_KEY:
            mechineData->runTime += 1;
            if(mechineData->runTime > MAX_RUN_TIME)
                mechineData->runTime = 120;
            IsDataChanged = YES;
            stateReturn = EXERCISE_SET;
            break;
        case DOWN_KEY:
            mechineData->runTime -= 1;
            if(mechineData->runTime < 1)
                mechineData->runTime = 0;
            IsDataChanged = YES;
            stateReturn = EXERCISE_SET;
            break;
        case STOP_KEY:
            resetRunMechineData(mechineData);
            IsDataChanged = YES;
            stateReturn = START;
            break;
        case RUN_KEY:
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        default:
            stateReturn = EXERCISE_SET;
            break;
    }
    return (stateReturn);
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
