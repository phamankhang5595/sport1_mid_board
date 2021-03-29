#include "keypad.h"
#include "run_mode.h"
#include "systick.h"
#include "screen.h"
/*******************************************************************************
 * Variable
 ******************************************************************************/
uint8_t isDataChange;

#define YES     0
#define NO      1
/*******************************************************************************
 * Code
 ******************************************************************************/
program_state_t runMode(run_mechine_data_t *mechineData, program_state_t lastState)
{
    program_state_t stateReturn;
    /* Stand by mode (3s)*/
    if(lastState != RUN)
    {
        /* show standby screen */
        SYSTICK_Delay_ms(1000);
        /* show standby screen */
        SYSTICK_Delay_ms(1000);
        /* show standby screen */
        SYSTICK_Delay_ms(1000);
    }
    /* Start run */
    if(isDataChange == YES)
    {
        /* send data */
        /* show screen */
        updateIncline(mechineData->incline);
        updateSpeed(mechineData->dataSpeed);
    }
    isDataChange = NO;
    char key = NO_KEY;
    key = KEYPAD_ScanKey();
    switch (key)
    {
    case '8':
        /* stop mode */
        stateReturn = STOP;
        break;
    case '1':
        /* incline = 3 */
        mechineData->incline = 3;
        stateReturn = RUN;
        isDataChange = YES;
        break;
    case '2':
        /* incline = 6 */
        mechineData->incline = 6;
        stateReturn = RUN;
        isDataChange = YES;
        break;
    case '3':
        /* incline = 9 */
        mechineData->incline = 9;
        stateReturn = RUN;
        isDataChange = YES;
        break;
    case 'C':
        /* speed = 3 */
        mechineData->dataSpeed = 3;
        stateReturn = RUN;
        isDataChange = YES;
        break;
    case 'D':
        /* speed = 6 */
        mechineData->dataSpeed = 6;
        stateReturn = RUN;
        isDataChange = YES;
        break;
    case 'E':
        /* speed = 9 */
        mechineData->dataSpeed = 9;
        stateReturn = RUN;
        isDataChange = YES;
        break;
    default:
        stateReturn = RUN;
        isDataChange = NO;
        break;
    }
    return (stateReturn);
    /* Show screnn */
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
