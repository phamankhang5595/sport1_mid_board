#include "start_mode.h"
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief 
 *
 * @param 
 * @param 
 */
program_state_t start_mode()
{
    program_state_t stateReturn;
    char key = NO_KEY;
    key = KEYPAD_ScanKey();
    /* start display */
    if(key == '6')
        stateReturn = EXERCISE_SET;
    else if(key == '7')
        stateReturn = RUN;
    else if(key == '9')
        stateReturn = USER_SET;
    else
        stateReturn = START;
    return (stateReturn);
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
