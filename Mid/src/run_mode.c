#include "keypad.h"
#include "run_mode.h"
#include "systick.h"
#include "screen.h"
#include "timer2.h"

/*******************************************************************************
* Definition
*******************************************************************************/
#define DEFAULT_RUN_SPEED   20
#define FLAG_OFF            0
#define FLAG_ON             1
typedef enum 
{
    COUNT_UP_ALL,
    COUNT_DOWN_TIME,
    COUNT_DOWN_DIS,
    COUNT_DOWN_CALO
} type_of_count;
/*******************************************************************************
 * Variable
 ******************************************************************************/
volatile uint32_t Sec;
type_of_count HowToCountData = COUNT_UP_ALL;
uint32_t ExeRunFlag = FLAG_OFF;
uint8_t Execrise[12][15]= {
                            {0x11,0x21,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11},
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
    if(HowToCountData != COUNT_DOWN_TIME)
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
static void check_last_state_and_change_run_info(run_mechine_data_t *mechineData, program_state_t *laststate)
{
    switch (*laststate)
    {
        case START:
            mechineData->speed = DEFAULT_RUN_SPEED;
            ExeRunFlag = FLAG_OFF;
            HowToCountData = COUNT_UP_ALL;
            break;
        case SET_UP:
            if(mechineData->runTime != 0)
            {
                Sec = mechineData->runTime;
                HowToCountData = COUNT_DOWN_TIME;
            }
            else if(mechineData->distance != 0)
                HowToCountData = COUNT_DOWN_DIS;
            else 
                HowToCountData = COUNT_DOWN_CALO;
            break;
        case EXERCISE_SET:
            Sec = mechineData->runTime;
            HowToCountData = COUNT_DOWN_TIME;
            ExeRunFlag    = FLAG_ON;
            break;
        default:
            break;
    }
}

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief The run mode
 *
 * @param mechineData
 * @param laststate 
 * @return State of program
*/
program_state_t run_mode(run_mechine_data_t *mechineData, program_state_t *laststate)
{
    program_state_t stateReturn;
    char key = NO_KEY;
    /* Variable to calculate when distances and calorie values change */
    static uint32_t countToChangeDistance;
    static uint32_t countToChangeCalo;
    static uint32_t changeMoment;
    static uint32_t numberOfChange;
    if(IsThisTheFirstTimeRun == YES)
    {
        /* run timer 2 */
        timer_2_init();
        timer_callback_init(countSec);
        timer_2_start();
        /* screen countdown 3s */
        waittingScreen(mechineData,&Sec);
        timer_2_stop();

        Sec = mechineData->runTime;
        countToChangeDistance = 0;
        countToChangeCalo = 0;
        /* return immediately when pressing the stop key */
        if(KEYPAD_ScanKey()==STOP_KEY)
        {
            Sec = 0;
            return STOP;
        }
        check_last_state_and_change_run_info(mechineData,laststate);
        /* Start timer for count time */
        timer_2_start();
        numberOfChange = 0;
        changeMoment = mechineData->runTime *14/15;
        mechineData->speed = DEFAULT_RUN_SPEED;
        IsThisTheFirstTimeRun = NO;
    }

    /* update mechindata_runtime */
    mechineData->runTime = Sec;
    /* update screen and send data to lower layer*/
    if(IsDataChanged == YES)
    {
        /* show screen */
        updateIncline(mechineData->incline);
        updateSpeed(mechineData->speed);
        updateTime(mechineData->runTime);
        /* send data to lower layer */
        IsDataChanged = NO;
    }
    
    /* scand keypad */
    key = KEYPAD_ScanKey();
    switch (key)
    {
        case STOP_KEY:
            /* stop mode */
            IsDataChanged = YES;
            IsThisTheFirstTimeRun = YES;
            ExeRunFlag = FLAG_OFF;
            HowToCountData = COUNT_UP_ALL;
            /* stop timer */
            timer_2_stop();
            Sec = 0;
            stateReturn = STOP;
            break;
        case INCLINE_3_KEY:
            /* incline = 3 */
            mechineData->incline =  3;
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case INCLINE_6_KEY:
            /* incline = 6 */
            mechineData->incline =  6;
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
            mechineData->speed = 30;
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case SPEED_6_KEY:
            /* speed = 6 */
            mechineData->speed = 60;
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case SPEED_9_KEY:
            /* speed = 9 */
            mechineData->speed = 90;
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case PLUS_KEY:  
            mechineData->speed += 1;
            if(mechineData->speed >= 150)
                mechineData->speed = 150;
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case MINUS_KEY:
            mechineData->speed -= 1;
            if(mechineData->speed <= 10)
                mechineData->speed = 10;
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
    
    /* cacutlate time to change calo and distance */
    countToChangeDistance++;
    countToChangeCalo++;   
    if(countToChangeDistance >= 8000000 / mechineData->speed)
    {
        if(HowToCountData == COUNT_DOWN_DIS)
            mechineData->distance -= 1;
        else
            mechineData->distance += 1;
        countToChangeDistance = 0;
        
    updateDistance(mechineData->distance);
    }
    if(countToChangeCalo >= 6500000 / mechineData->speed)
    {
        if(HowToCountData == COUNT_DOWN_CALO)
            mechineData->calo -= 1;
        else
            mechineData->calo += 1;
        countToChangeCalo = 0;
        updateCalo(mechineData->calo);
    }

    /* RUN FOR EXEC MODE */
    /* change speed and data if run in exercise mode */
    if(ExeRunFlag == FLAG_ON)
    {
        if(Sec == changeMoment)
        {
            changeMoment = Sec*14/15;
            mechineData->speed   += (Execrise[mechineData->runEx - 1][numberOfChange] >> 4) & 0x0F;
            mechineData->incline += (Execrise[mechineData->runEx - 1][numberOfChange]) & 0x0F;
            if( ((uint32_t)(mechineData->speed)) >= 150)
                mechineData->speed = 150;
            if(mechineData->incline > 12)
                mechineData->incline = 12;
            numberOfChange ++;
            IsDataChanged = YES;
        }
    }

    /* RUN FOR SETUP MODE */
    /* Stop if time or distance or calories expire */
    switch (HowToCountData)
    {
        case COUNT_DOWN_DIS:
            if(mechineData->distance == 0)
            {
                    /* stop mode */
                    IsDataChanged     = YES;
                    IsThisTheFirstTimeRun = YES;
                    HowToCountData    = COUNT_UP_ALL;
                    ExeRunFlag        = FLAG_OFF;
                    /* stop timer */
                    timer_2_stop();
                    stateReturn = STOP;
            }
            break;
        case COUNT_DOWN_CALO:
            if(mechineData->calo == 0)
            {
                    /* stop all */
                    IsDataChanged     = YES;
                    IsThisTheFirstTimeRun = YES;
                    HowToCountData    = COUNT_UP_ALL;
                    ExeRunFlag        = FLAG_OFF;
                    timer_2_stop();
                    stateReturn = STOP;
            }
            break;
        case COUNT_DOWN_TIME:
            if(mechineData->runTime == 0)
            {
                    /* stop mode */
                    IsDataChanged     = YES;
                    IsThisTheFirstTimeRun = YES;
                    HowToCountData    = COUNT_UP_ALL;
                    ExeRunFlag        = FLAG_OFF;
                    /* stop timer */
                    timer_2_stop();
                    stateReturn = STOP;
            }
            break;
        default:
            break;
    }
    *laststate = RUN;
    return (stateReturn);
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
