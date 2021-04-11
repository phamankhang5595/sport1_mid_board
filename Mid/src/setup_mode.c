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
/*******************************************************************************
 * Definition
 ******************************************************************************/
#define MAX_MODE    (2)
#define BLINK_FREQ  (50000)
/*******************************************************************************
 * Private func
 ******************************************************************************/
/*!
 * @brief Increase the value according to the mode
 *        runtime, distance, calo <=> mode: 0 1 2
 *
 * @param mechineData
 * @return None 
 */
static void increase_val(run_mechine_data_t *mechineData);


/*!
 * @brief Decrease the value according to the mode
 *        runtime, distance, calo <=> mode: 0 1 2
 * @param 
 * @param 
 */
static void decrease_val(run_mechine_data_t *mechineData);

/*!
 * @brief change data to default
 *
 * @param 
 * @param 
*/
void reset_data_to_default(run_mechine_data_t *mechineData, uint8_t ModeState);
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief The setup mode
 *
 * @param mechineData
 * @param laststate 
 * @return State of program
*/
program_state_t setup_mode(run_mechine_data_t *mechineData, program_state_t *laststate)
{
    program_state_t stateReturn;
    char key = NO_KEY;
    /* fist time run */
    if(IsThisTheFirstTimeRun == YES)
    {
        while(KEYPAD_ScanKey()==SETUP_KEY);
        IsThisTheFirstTimeRun = NO;
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
            reset_data_to_default(mechineData, ModeState);
            IsDataChanged = YES;
            stateReturn = SET_UP;
            break;
        case PLUS_KEY:
            increase_val(mechineData);
            IsDataChanged = YES;
            stateReturn = SET_UP;
            break;
        case MINUS_KEY:
            decrease_val(mechineData);
            IsDataChanged = YES;
            stateReturn = SET_UP;
            break;
        case UP_KEY:
            increase_val(mechineData);
            IsDataChanged = YES;
            stateReturn = SET_UP;
            break;
        case DOWN_KEY:
            decrease_val(mechineData);
            IsDataChanged = YES;
            stateReturn = SET_UP;
            break;
        case RUN_KEY:
            IsDataChanged = YES;
            IsThisTheFirstTimeRun = YES;
            
            stateReturn = RUN;
            break;
        case STOP_KEY:
            IsDataChanged = YES;
            IsThisTheFirstTimeRun = YES;
            stateReturn = START;
            break;
        default:
            stateReturn = SET_UP;
    }
    if(ModeState > MAX_MODE)
    {
        ModeState = 0;
        IsDataChanged = YES;
        IsThisTheFirstTimeRun = YES;
        stateReturn = START;
        SYSTICK_Delay_ms(30);
    }
    
    *laststate = SET_UP;
    return (stateReturn);
}


static void increase_val(run_mechine_data_t *mechineData)
{
    if(ModeState == 0)
    {
        mechineData->runTime += 60;
        if(mechineData->runTime > MAX_RUN_TIME)
            mechineData->runTime = 60;
    }
    else if(ModeState == 1)
    {
        mechineData->distance += 10;
        if(mechineData->distance > MAX_DISTANCE)
            mechineData->distance = 10;
    }
    else if(ModeState == 2)
    {
        mechineData->calo += 10;
        if(mechineData->calo > MAX_CALO)
            mechineData->calo = 10;
    }
}

static void decrease_val(run_mechine_data_t *mechineData)
{
    if(ModeState == 0)
    {
        if(mechineData->runTime > 300)
            mechineData->runTime -= 60;
        else
            mechineData->runTime = MAX_RUN_TIME;
    }
    else if(ModeState == 1)
    {
        mechineData->distance -= 10;
        if(mechineData->distance < 1)
            mechineData->distance = MAX_DISTANCE;
    }
    else if(ModeState == 2)
    {
        
        mechineData->calo -= 10;
        if(mechineData->calo < 10)
            mechineData->calo = MAX_CALO;
    }
}

void reset_data_to_default(run_mechine_data_t *mechineData, uint8_t ModeState)
{
    switch (ModeState)
    {
        case 1:
            mechineData->runTime = 0;
            mechineData->distance = 100;
            break;
        case 2:
            mechineData->distance = 0;
            mechineData->calo = 500;
            break;
        default:
            break;
    }
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
