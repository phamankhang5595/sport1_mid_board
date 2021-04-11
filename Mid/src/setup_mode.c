#include "setup_mode.h"
#include "keypad.h"
#include "systick.h"
#include "screen.h"
#include "lcd.h"
/*******************************************************************************
 * Variables
 ******************************************************************************/
/* mode setup  */
uint8_t ModeState;
/* count for blink */
uint32_t CountForBlink;
/*  */
static uint8_t IsFistTimeRun;
/*******************************************************************************
 * Definition
 ******************************************************************************/
#define MAX_MODE    (2)
#define BLINK_FREQ  (50000)
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
        mechineData->runTime += 60;
        if(mechineData->runTime > MAX_RUN_TIME)
            mechineData->runTime = 1;            /* maximum runtime value */
    }
    else if(ModeState == 1)
    {
        mechineData->distance += 1;
        if(mechineData->distance > MAX_DISTANCE)
            mechineData->distance = 1;           /* maximum distance value */
    }
    else if(ModeState == 2)
    {
        mechineData->calo += 1;
        if(mechineData->calo > MAX_CALO)
            mechineData->calo = 1;                   /* maximum distance value */
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
        if(mechineData->runTime > 60)
        {
            mechineData->runTime -= 60;
        }
        else
            mechineData->runTime = MAX_RUN_TIME;            /* minimum runtime value */
    }
    else if(ModeState == 1)
    {
        mechineData->distance -= 1;
        if(mechineData->distance < 1)
            mechineData->distance = MAX_DISTANCE;           /* minimum distance value */
    }
    else if(ModeState == 2)
    {
        mechineData->calo -= 1;
        if(mechineData->calo < 1)
            mechineData->calo = MAX_CALO;               /* minimum distance value */
    }
}

/*!
 * @brief change data to default
 *
 * @param 
 * @param 
*/
void changeDatatoDefaultVal(run_mechine_data_t *mechineData, uint8_t ModeState)
{
    switch (ModeState)
    {
        case 1:
            mechineData->runTime = 0;
            mechineData->distance = 1;
            break;
        case 2:
            mechineData->distance = 0;
            mechineData->calo = 50;
            break;
        default:
            break;
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
program_state_t setup_mode(run_mechine_data_t *mechineData, program_state_t *laststate)
{
    program_state_t stateReturn;
    char key = NO_KEY;
    /* fist time run */
    if(IsFistTimeRun == YES)
    {
        while(KEYPAD_ScanKey()==SETUP_KEY);
        IsFistTimeRun = NO;
        mechineData->runTime = 900;/* default value 15 min */
    }
    /* Show screen */
    if(IsDataChanged == YES)
    {
        updateTime(mechineData->runTime);
        updateDistance(mechineData->distance);
        updateCalo(mechineData->calo);
        IsDataChanged = NO;
    }
    /* use a variable Count blink led */
    CountForBlink++;
    if(CountForBlink == BLINK_FREQ)
    {
        if (ModeState==0)
        {
            lcd_clr_section(0,1);
            lcd_clr_section(8,3);
        }
        else if (ModeState == 1)
            lcd_clr_section(14,3);
        else if(ModeState == 2)
            lcd_clr_section(20,3);
    }
    else if(CountForBlink == 2*BLINK_FREQ)
    {
        IsDataChanged = YES;
        CountForBlink=0;
    }

    /* Scan key */
    key = KEYPAD_ScanKey();
    switch(key)
    {
        case SETUP_KEY:
            while(KEYPAD_ScanKey()==SETUP_KEY);
            ModeState += 1;
            /* clear data */
            changeDatatoDefaultVal(mechineData, ModeState);
            IsDataChanged = YES;
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
            IsFistTimeRun = YES;
            *laststate = USER_SET;
            stateReturn = RUN;
            break;
        case STOP_KEY:
            IsDataChanged = YES;
            IsFistTimeRun = YES;
            stateReturn = START;
            break;
        default:
            stateReturn = USER_SET;
    }
    if(ModeState > MAX_MODE)
    {
        ModeState = 0;
        IsDataChanged = YES;
        IsFistTimeRun = YES;
        stateReturn = START;
        SYSTICK_Delay_ms(30);
    }
    return (stateReturn);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
