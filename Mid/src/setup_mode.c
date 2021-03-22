#include "setup_mode.h"
/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t modeState;
/*******************************************************************************
 * Private func
 ******************************************************************************/
/*!
 * @brief 
 *
 * @param 
 * @param 
 */
static void increase_val(run_mechine_data_t *mechineData)
{
    if(modeState == 0)
        mechineData->runTime += 1;
    else if(modeState == 1)
        mechineData->dis += 1;
    else if(modeState == 2)
        mechineData->clo += 1;
}


/*!
 * @brief 
 *
 * @param 
 * @param 
 */
static void decrease_val(run_mechine_data_t *mechineData)
{
    if(modeState == 0)
        mechineData->runTime -= 1;
    else if(modeState == 1)
        mechineData->dis -= 1;
    else if(modeState == 2)
        mechineData->clo -= 1;
}
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief 
 *
 * @param 
 * @param 
 */
program_state_t setup_mode(run_mechine_data_t *mechineData)
{
    program_state_t stateReturn;
    char key = NO_KEY;
    
    /* Show screen */

    /* Scan key */
    key = KEYPAD_ScanKey();
    switch(key)
    {
        case '9':
            modeState += 1;
            stateReturn = USER_SET;
            break;
        case '4':
            increase_val(mechineData);
            stateReturn = USER_SET;
            break;
        case '5':
            decrease_val(mechineData);
            stateReturn = USER_SET;
            break;
        case 'A':
            increase_val(mechineData);
            stateReturn = USER_SET;
            break;
        case 'B':
            decrease_val(mechineData);
            stateReturn = USER_SET;
            break;
        case '7':
            stateReturn = RUN;
            break;
        case '8':
            stateReturn = START;
            break;
        default:
            stateReturn = USER_SET;
    }
    if(modeState > 2)
    {
        modeState = 0;
        stateReturn = START;
    }
    return (stateReturn);
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
