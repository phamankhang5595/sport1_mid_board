#include "exercise_mode.h"
#include "keypad.h"
#include "screen.h"
#include "lcd.h"
#include "systick.h"
/*******************************************************************************
 * Variable
 ******************************************************************************/
static uint8_t  ResetVal;
static uint8_t  IsFistTimeRun;
static uint32_t CountForBlink;

/*******************************************************************************
* Definition
*******************************************************************************/

#define BLINK_FREQ      (50000U)

/*******************************************************************************
* Private func
*******************************************************************************/
/*!
 * @brief 
 *
 * @param 
 * @param 
*/
static void blinkTimeLed()
{
    CountForBlink++;
    if(CountForBlink == BLINK_FREQ)
    {
        lcd_clr_section(0,1);
        lcd_clr_section(8,3);
    }
    else if(CountForBlink == 2*BLINK_FREQ)
    {
        IsDataChanged = YES;
        CountForBlink=0;
    }
}

/*!
 * @brief 
 *
 * @param 
 * @param 
*/
static void resetExVal(run_mechine_data_t *mechineData)
{
    /* time: 30 min */
    mechineData->runTime = 1800;
    switch(mechineData->runEx)
    {
        case 7:
            mechineData->incline = 2;
            break;
        case 10:
            mechineData->incline = 9;
            break;
        case 11:
            mechineData->incline = 9;
            break;
        case 12:
            mechineData->incline = 2;
            break;
        default:
            mechineData->incline = 0;
            break;
    }
    updateIncline(mechineData->incline);
    ResetVal = NO;
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
program_state_t exercise_mode(run_mechine_data_t *mechineData, program_state_t *laststate)
{
    static program_state_t stateReturn;
    char key;
    /* reset time, incline for new exercise*/
    if(ResetVal == YES)
    {
        resetExVal(mechineData);
        ResetVal = NO;
    }

    /* update screen */
    if(IsDataChanged == YES)
    {
        updateEx(mechineData->runEx);               /* exercise */
        SYSTICK_Delay_ms(50);
        updateTime(mechineData->runTime);           /* Run time */
        IsDataChanged = NO;
    }

    /* check first time run */
    if(IsFistTimeRun == YES)
    {
        while(KEYPAD_ScanKey()==EXE_KEY);
        IsFistTimeRun = NO;
    }

    blinkTimeLed();
    
    /* Scan key */
    key = KEYPAD_ScanKey();
    /* Executive */
    switch (key)
    {
        case EXE_KEY:
            while(KEYPAD_ScanKey()==EXE_KEY);
            mechineData->runEx += 1;
            if(mechineData->runEx > MAX_RUN_EX)
                mechineData->runEx = DEFAULT_RUN_EX;
            ResetVal = YES;
            IsDataChanged = YES;
            stateReturn = EXERCISE_SET;
            break;
        case PLUS_KEY:
            mechineData->runTime += 60;
            if(mechineData->runTime > MAX_RUN_TIME)
                mechineData->runTime = MIN_RUN_TIME;
            IsDataChanged = YES;
            stateReturn = EXERCISE_SET;
            break;
        case MINUS_KEY:
            mechineData->runTime -= 60;
            if(mechineData->runTime < MIN_RUN_TIME)
                mechineData->runTime = MAX_RUN_TIME;
            IsDataChanged = YES;
            stateReturn = EXERCISE_SET;
            break;
        case UP_KEY:
            mechineData->runTime += 60;
            if(mechineData->runTime > MAX_RUN_TIME)
                mechineData->runTime = MIN_RUN_TIME;
            IsDataChanged = YES;
            stateReturn = EXERCISE_SET;
            break;
        case DOWN_KEY:
            mechineData->runTime -= 60;
            if(mechineData->runTime < MIN_RUN_TIME)
                mechineData->runTime = MAX_RUN_TIME;
            IsDataChanged = YES;
            stateReturn = EXERCISE_SET;
            break;
        case STOP_KEY:
            resetRunMechineData(mechineData);
            IsDataChanged = YES;
            IsFistTimeRun = YES;
            ResetVal      = YES;
            stateReturn = START;
            break;
        case RUN_KEY:
            IsDataChanged = YES;
            IsFistTimeRun = YES;
            ResetVal      = YES;
            *laststate     = EXERCISE_SET;
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
