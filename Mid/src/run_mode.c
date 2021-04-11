#include "keypad.h"
#include "run_mode.h"
#include "systick.h"
#include "screen.h"
#include "timer2.h"

/*******************************************************************************
* Definition
*******************************************************************************/
#define DEFAULT_RUN_SPEED   2
#define FLAG_OFF            0
#define FLAG_ON             1
/*******************************************************************************
 * Variable
 ******************************************************************************/
uint32_t IsThisTheFirstRun = YES;
volatile uint32_t Sec;
uint32_t CountDownFlag = FLAG_OFF;
uint32_t ExeRunFlag = FLAG_OFF;
uint32_t 
uint16_t Execrise[12][15]= {
                            {1,2,3,4,5,6,7,8,9,2,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            };
/*******************************************************************************
*Private func
*******************************************************************************/
/*!
 * @brief 
 *
 * @param 
 * @param 
*/
static void countSec()
{
    /* check Flag */
    if(CountDownFlag == FLAG_OFF)
    {
        Sec++;
        if(Sec == 0xFFFFFFFF)
        Sec = 0;
    }
    else
    {
        if(Sec != 0);
        Sec--;
    }
    /* time data change */
    IsDataChanged = YES;
}

/*!
 * @brief 
 *
 * @param 
 * @param 
*/
static void checkLastStateAndTurnFlag(run_mechine_data_t *mechineData, program_state_t *laststate)
{
    switch (*laststate)
    {
        case START:
            mechineData->speed = DEFAULT_RUN_SPEED;
            CountDownFlag = FLAG_OFF;
            ExeRunFlag    = FLAG_OFF;
            break;
        case USER_SET:
            if(mechineData->runTime != 0)
            {
                Sec = mechineData->runTime;
                CountDownFlag = FLAG_ON;
            }
            break;
        case EXERCISE_SET:
            Sec = mechineData->runTime;
            CountDownFlag = FLAG_ON;
            ExeRunFlag    = FLAG_ON;
            break;
        default:
            break;
    }
    *laststate = RUN;
}

/*!
 * @brief 
 *
 * @param 
 * @param 
*/
static void change_data_ex(run_mechine_data_t *mechineData)
{
    switch(mechineData->runEx)
    {
        case 1:
            break;
    }
}
/*******************************************************************************
 * Code
 ******************************************************************************/
program_state_t runMode(run_mechine_data_t *mechineData, program_state_t *laststate)
{
    program_state_t stateReturn;
    char key = NO_KEY;
    static uint32_t changeMoment;
    if(IsThisTheFirstRun == YES)
    {
        /* init timer 2 */
        timer_2_init();
        timer_callback_init(countSec);
        /* screen countdown 3s */
        waittingScreen(mechineData);
        /* turn flag */
        checkLastStateAndTurnFlag(mechineData,laststate);
        changeMoment = mechineData->runTime *14/15;
        /* Start timer for count time */
        timer_2_start();
        IsThisTheFirstRun = NO; 
    }

    /* update mechindata_runtime */
    mechineData->runTime = Sec;

    /* update screen */
    if(IsDataChanged == YES)
    {
        /* send data */
        /* show screen */
        updateIncline(mechineData->incline);
        updateSpeed(mechineData->speed);
        updateTime(mechineData->runTime);
        IsDataChanged = NO;
    }
    
    /* scand keypad */
    key = KEYPAD_ScanKey();
    switch (key)
    {
        case STOP_KEY:
            /* stop mode */
            IsDataChanged = YES;
            IsThisTheFirstRun = YES;
            /* stop timer */
            timer_2_stop();
            Sec = 0;
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
        case PLUS_KEY:
            mechineData->speed += 0.1;
            if(mechineData->speed >= 15)
                mechineData->speed = 15;
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case MINUS_KEY:
            mechineData->speed -= 0.1;
            if(mechineData->speed <= 1)
                mechineData->speed = 1;
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case UP_KEY:
            mechineData->incline += 1;
            if(mechineData->incline >= 12)
                mechineData->incline = 12;
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case DOWN_KEY:
            if(mechineData->incline != 0)
                mechineData->incline -= 1;
            IsDataChanged = YES;
            stateReturn = RUN;
        default:
            stateReturn = RUN;
            break;
    }

    if(ExeRunFlag == FLAG_ON)
    {
        if(Sec == changeMoment)
        {
            changeMoment = Sec*14/15;
            timeChangeExe ++;
            mechineData->speed  += Execrise[]
        }
    }
    /* stop if out of time */
    if((Sec == 0) && (CountDownFlag))
    {
        /* stop mode */
        IsDataChanged = YES;
        IsThisTheFirstRun = YES;
        CountDownFlag = FLAG_OFF;
        /* stop timer */
        timer_2_stop();
        stateReturn = STOP;
    }
    return (stateReturn);
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
