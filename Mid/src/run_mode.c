#include "keypad.h"
#include "run_mode.h"
#include "systick.h"
#include "screen.h"
/*******************************************************************************
 * Variable
 ******************************************************************************/
uint32_t IsThisTheFirstRun;
/*******************************************************************************
 * Code
 ******************************************************************************/
program_state_t runMode(run_mechine_data_t *mechineData)
{
    static program_state_t stateReturn;
    char key = NO_KEY;
    if(IsThisTheFirstRun == YES)
    {
        /* screen countdown 3s */
        IsThisTheFirstRun = NO;
    }

    /* Check if setup run time */
    if((mechineData->runTime > DEFAULT_RUN_TIME)/* && timer not start */)
    {
        /* Start timer count down */
        /* interrup 1 time per sec: function minus run time and updatescreen */
    }

    /* update screen */
    if(IsDataChanged == YES)
    {
        /* send data */
        /* show screen */
        updateIncline(mechineData->incline);
        updateSpeed(mechineData->speed);
        IsDataChanged = NO;
    }
    


    key = KEYPAD_ScanKey();
    switch (key)
    {
        case STOP_KEY:
            /* stop mode */
            IsDataChanged = YES;
            IsThisTheFirstRun = YES;
            stateReturn = STOP;
            break;
        case INCLINE_3_KEY:
            /* incline = 3 */
            mechineData->incline = 3;
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case INCLINE_6_KEY:
            /* incline = 6 */
            mechineData->incline = 6;
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case INCLINE_9_KEY:
            /* incline = 9 */
            mechineData->incline = 9;
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case SPEED_3_KEY:
            /* speed = 3 */
            mechineData->speed = 3;
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case SPEED_6_KEY:
            /* speed = 6 */
            mechineData->speed = 6;
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case SPEED_9_KEY:
            /* speed = 9 */
            mechineData->speed = 9;
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        default:
            stateReturn = RUN;
            break;
    }
    return (stateReturn);
    /* Show screnn */
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
