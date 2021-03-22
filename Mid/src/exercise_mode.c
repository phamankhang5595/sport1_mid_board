#include "exercise_mode.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

program_state_t exercise_mode(run_mechine_data_t *mechineData)
{
    program_state_t stateReturn;
    char key;
    /* Show screen */

    /* Scan key */
    key = KEYPAD_ScanKey();
    switch (key)
    {
        /* ex key */
        case '6':
            mechineData->runEx += 1;
            if(mechineData->runEx > 15)
                mechineData->runEx = 1;
            stateReturn = EXERCISE_SET;
            break;
        /* + key */
        case '4':
            mechineData->runTime += 1;
            if(mechineData->runTime > 120)
                mechineData->runTime = 120;
            stateReturn = EXERCISE_SET;
            break;
        /* - key */
        case '5':
            mechineData->runTime -= 1;
            if(mechineData->runTime < 0)
                mechineData->runTime = 0;
            stateReturn = EXERCISE_SET;
            break;
        /* up key */
        case 'A':
            mechineData->runTime += 1;
            if(mechineData->runTime > 120)
                mechineData->runTime = 120;
            stateReturn = EXERCISE_SET;
            break;
        /* down key */
        case 'B':
            mechineData->runTime -= 1;
            if(mechineData->runTime < 0)
                mechineData->runTime = 0;
            stateReturn = EXERCISE_SET;
            break;
        /* stop key */
        case '8':
            mechineData->runTime += 1;
            if(mechineData->runTime > 120)
                mechineData->runTime = 120;
            stateReturn = START;
            break;
        /* run key */
        case '7':
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
