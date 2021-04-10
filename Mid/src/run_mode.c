#include "keypad.h"
#include "run_mode.h"
#include "systick.h"
#include "screen.h"
#include "timer2.h"

/*******************************************************************************
* Definition
*******************************************************************************/
#define DEFAULT_RUN_SPEED   3
#define FLAG_OFF            0
#define FLAG_ON             1
/*******************************************************************************
 * Variable
 ******************************************************************************/
uint32_t IsThisTheFirstRun = YES;
volatile uint32_t Sec;
uint32_t CountDownFlag = FLAG_OFF;
uint32_t ExeRunFlag = FLAG_OFF;
/*******************************************************************************
*Private func
*******************************************************************************/
static void countUpSec()
{
    Sec++;
    IsDataChanged = YES;
    if(Sec == 0xFFFFFFFF)
        Sec = 0;
}

static void countDownSec()
{
  
    if(Sec != 0);
       Sec--;
    IsDataChanged = YES;
}

static void changeDataEx(run_mechine_data_t *mechineData)
{
    switch (mechineData->runEx)
    {
        case 1:
            mechineData->speed = 5;
            break;
        default:
            break;
            
    }
}
/*******************************************************************************
 * Code
 ******************************************************************************/
program_state_t runMode(run_mechine_data_t *mechineData, program_state_t *laststate)
{
    static program_state_t stateReturn;
    static uint32_t runTimeForChange;
    char key = NO_KEY;
    if(IsThisTheFirstRun == YES)
    {
        /* init timer 2 */
        timer_2_init();
        /* screen countdown 3s */
        waittingScreen(mechineData);
        IsThisTheFirstRun = NO;
    }

    /* config run */
    switch (*laststate)
    {
        case START:
            mechineData->speed = DEFAULT_RUN_SPEED;
            timer_callback_init(countUpSec);
            timer_2_start();
            break;
        case USER_SET:
            if(mechineData->runTime != 0)
            {
                Sec = mechineData->runTime;
                timer_callback_init(countDownSec);
                CountDownFlag = FLAG_ON;
                timer_2_start();
            }
            break;
        case EXERCISE_SET:
            Sec = mechineData->runTime;
            runTimeForChange = mechineData->runTime/2;
            timer_callback_init(countDownSec);
            CountDownFlag = FLAG_ON;
            ExeRunFlag    = FLAG_ON;
            timer_2_start();
            break;
        default:
            break;
    }
    *laststate = RUN;
    /* update mechindata_runtime */
    mechineData->runTime = Sec;
    /* check Exe flag and change data */
    if(ExeRunFlag && ( mechineData->runTime < runTimeForChange))
    {
        changeDataEx(mechineData);
        ExeRunFlag = FLAG_OFF;
    }
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
    /* out of time */
    if((Sec == 0) &&(CountDownFlag))
    {
        /* stop mode */
        IsDataChanged = YES;
        IsThisTheFirstRun = YES;
        CountDownFlag = FLAG_OFF;
        /* stop timer */
        timer_2_stop();
        return STOP;
    }
    /* keypad */
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
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case MINUS_KEY:
            mechineData->speed -= 0.1;
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case UP_KEY:
            mechineData->incline += 1;
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case DOWN_KEY:
            mechineData->incline -= 1;
            IsDataChanged = YES;
            stateReturn = RUN;
        default:
            stateReturn = RUN;
            break;
    }
    return (stateReturn);
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
