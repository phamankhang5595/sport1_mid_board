#include "setup_mode.h"
#include "keypad.h"
#include "systick.h"
#include "screen.h"
/*******************************************************************************
 * Variables
 ******************************************************************************/
/* mode setup  */
uint8_t ModeState;
/*******************************************************************************
 * Definition
 ******************************************************************************/
#define MAX_MODE    (2)

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
    if(ModeState == 0)
    {
        mechineData->runTime += 1;
        if(mechineData->runTime > MAX_RUN_TIME)
            mechineData->runTime = MAX_RUN_TIME;            /* maximum runtime value */
    }
    else if(ModeState == 1)
    {
        mechineData->distance += 1;
        if(mechineData->distance > MAX_DISTANCE)
            mechineData->distance = MAX_DISTANCE;           /* maximum distance value */
    }
    else if(ModeState == 2)
    {
        mechineData->calo += 1;
        if(mechineData->calo > MAX_CALO)
            mechineData->calo = MAX_CALO;                   /* maximum distance value */
    }
}


/*!
 * @brief 
 *
 * @param 
 * @param 
 */
static void decrease_val(run_mechine_data_t *mechineData)
{
    if(ModeState == 0)
    {
        mechineData->runTime -= 1;
        if(mechineData->runTime < 1)
            mechineData->runTime = 1;            /* minimum runtime value */
    }
    else if(ModeState == 1)
    {
        mechineData->distance -= 1;
        if(mechineData->distance < 1)
            mechineData->distance = 1;           /* minimum distance value */
    }
    else if(ModeState == 2)
    {
        mechineData->calo -= 1;
        if(mechineData->calo < 1)
            mechineData->calo = 1;               /* minimum distance value */
    }
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
    if(IsDataChanged == YES)
    {
        updateCalo(mechineData->calo);
        updateDistance(mechineData->distance);
        updateTime(mechineData->runTime);
        IsDataChanged = NO;
    }   
    /* Scan key */
    key = KEYPAD_ScanKey();
    switch(key)
    {
        case SETUP_KEY:
            SYSTICK_Delay_ms(200);
            ModeState += 1;
            stateReturn = USER_SET;
            break;
        case PLUS_KEY:
            increase_val(mechineData);
            IsDataChanged = YES;
            stateReturn = USER_SET;
            break;
        case MINUS_KEY:
            decrease_val(mechineData);
            IsDataChanged = YES;
            stateReturn = USER_SET;
            break;
        case UP_KEY:
            increase_val(mechineData);
            IsDataChanged = YES;
            stateReturn = USER_SET;
            break;
        case DOWN_KEY:
            decrease_val(mechineData);
            IsDataChanged = YES;
            stateReturn = USER_SET;
            break;
        case RUN_KEY:
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case STOP_KEY:
            IsDataChanged = YES;
            stateReturn = START;
            break;
        default:
            stateReturn = USER_SET;
    }
    if(ModeState > MAX_MODE)
    {
        ModeState = 0;
        IsDataChanged = YES;
        stateReturn = START;
        SYSTICK_Delay_ms(30);
    }
    return (stateReturn);
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
