#include "exercise_mode.h"
/*******************************************************************************
 * Variable
 ******************************************************************************/
extern uint8_t isDataChanged;
/*******************************************************************************
 * Code
 ******************************************************************************/

program_state_t exercise_mode(run_mechine_data_t *mechineData)
{
    program_state_t stateReturn;
    char key;
    /* Show screen */
    if(isDataChanged == 0)
    {
        updateCalo(mechineData->clo);
        updateDistance(mechineData->dis);
        updateSpeed(mechineData->dataSpeed);
        updateIncline(mechineData->incline);
        updateTime(mechineData->runTime.minute);
    }
    isDataChanged = 1;    
    /* Scan key */
    key = KEYPAD_ScanKey();
    switch (key)
    {
        /* ex key */
        case '6':
            mechineData->runEx += 1;
            if(mechineData->runEx > 15)
                mechineData->runEx = 1;
            isDataChanged = 0;
            stateReturn = EXERCISE_SET;
            break;
        /* + key */
        case '4':
            mechineData->runTime.minute += 1;
            if(mechineData->runTime.minute > 120)
                mechineData->runTime.minute = 120;
            isDataChanged = 0;
            stateReturn = EXERCISE_SET;
            break;
        /* - key */
        case '5':
            mechineData->runTime.minute -= 1;
            if(mechineData->runTime.minute < 1)
                mechineData->runTime.minute = 0;
            isDataChanged = 0;
            stateReturn = EXERCISE_SET;
            break;
        /* up key */
        case 'A':
            mechineData->runTime.minute += 1;
            if(mechineData->runTime.minute > 120)
                mechineData->runTime.minute = 120;
            isDataChanged = 0;
            stateReturn = EXERCISE_SET;
            break;
        /* down key */
        case 'B':
            mechineData->runTime.minute -= 1;
            if(mechineData->runTime.minute < 1)
                mechineData->runTime.minute = 0;
            isDataChanged = 0;
            stateReturn = EXERCISE_SET;
            break;
        /* stop key */
        case '8':
            mechineData->runTime.minute += 1;
            if(mechineData->runTime.minute > 120)
                mechineData->runTime.minute = 120;
            isDataChanged = 0;
            stateReturn = START;
            break;
        /* run key */
        case '7':
            isDataChanged = 0;
            stateReturn = RUN;
            break;
        default:
            isDataChanged = 0;
            stateReturn = EXERCISE_SET;
            break;
    }
    return (stateReturn);
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
