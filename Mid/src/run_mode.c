#include "keypad.h"
#include "systick.h"
#include "screen.h"
#include "timer3.h"
#include "lcd.h"
#include "uart.h"
#include "power_communicate.h"
#include "run_mode.h"
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
 * @brief function count seccond with tone
 *
 * @param none
*/
static void countSecWithTone()
{
    Sec++;
    lcd_tone();
}

/*!
 * @brief waitting sceen 3s before start
 *
 * @param 
 * @param 
*/

static void waittingScreen(run_mechine_data_t *mechineDate)
{
    
    unsigned char ch;
    
    lcd_clr();
    updateCalo(mechineDate->calo);
    updateDistance(mechineDate->distance);
    updateIncline(mechineDate->incline);
    updateTime(mechineDate->runTime);
    timer_3_init();
    timer_callback_init(countSecWithTone);
    timer_3_start();
        /* screen countdown 3s */
    while((Sec<4)&&(KEYPAD_ScanKey()!=STOP_KEY))
    {
        switch(4-Sec)
        {
            case 3:
                ch = 0x7a;
                break;
            case 2:
                ch = 0x3e;
                break;
            case 1:
                ch = 0x60;
                break;
            default:
                break;
        }
        NVIC_DisableIRQ(TIM3_IRQn);
        lcd_show_data1(&ch,4);
        NVIC_EnableIRQ(TIM3_IRQn);
    }
    timer_3_stop();
}


/*!
 * @brief check the last state of the machine to change default and method count data
 *        by changing the ExeRunFlag and HowToCountData variables
 *
 * @param mechineData
 * @param lastState
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
    static power_com_cmd_t cmdSend;
    /* Variable to calculate when distances and calorie values change */
    static uint32_t startTickForChangeDistance;
    static uint32_t startTickForChangeCalo;
    static uint32_t changeMoment;
    static uint32_t numberOfChange;
    if(IsThisTheFirstTimeRun == YES)
    {
        waittingScreen(mechineData);
        Sec = mechineData->runTime;
        startTickForChangeCalo = 0;
        /* return immediately when pressing the stop key */
        if(KEYPAD_ScanKey()==STOP_KEY)
        {
            Sec = 0;
            return STOP;
        }
        check_last_state_and_change_run_info(mechineData,laststate);
        /* Start timer for count time */
        timer_callback_init(countSec);
        timer_3_start();
        numberOfChange = 0;
        changeMoment = mechineData->runTime *14/15;
        mechineData->speed = DEFAULT_RUN_SPEED;
        IsThisTheFirstTimeRun = NO;
        
        startTickForChangeDistance = SYSTICK_Get_Tick();
    }

    /* update mechindata_runtime */
    mechineData->runTime = Sec;
    /* update screen and send data to lower layer*/
    if(IsDataChanged == YES)
    {
        /* show screen */
        updateTime(mechineData->runTime);
        updateIncline(mechineData->incline);
        updateSpeed(mechineData->speed);
        /* send data to lower layer */
        cmdSend = power_com_data_to_cmd(mechineData->speed, mechineData->incline);
        if(cmdSend.command != 0xFF)
            power_com_send_cmd(&cmdSend, cmdSend.length + 5);
        //UART_SendData
        IsDataChanged = NO;
    }

    /* scand keypad */
    key = KEYPAD_ScanKey();
    switch (key)
    {
        case STOP_KEY:
            /* stop mode */
            lcd_tone();
            IsDataChanged = YES;
            IsThisTheFirstTimeRun = YES;
            ExeRunFlag = FLAG_OFF;
            HowToCountData = COUNT_UP_ALL;
            /* stop timer */
            timer_3_stop();
            Sec = 0;
            stateReturn = STOP;
            break;
        case INCLINE_3_KEY:
            /* incline = 3 */
            mechineData->incline =  3;
            lcd_tone();
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case INCLINE_6_KEY:
            /* incline = 6 */
            mechineData->incline =  6;
            lcd_tone();
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case INCLINE_9_KEY:
            /* incline = 9 */
            mechineData->incline = 9;
            lcd_tone();
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case SPEED_3_KEY:
            /* speed = 3 */
            mechineData->speed = 30;
            lcd_tone();
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case SPEED_6_KEY:
            /* speed = 6 */
            mechineData->speed = 60;
            lcd_tone();
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case SPEED_9_KEY:
            /* speed = 9 */
            mechineData->speed = 90;
            lcd_tone();
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case PLUS_KEY:  
            mechineData->speed += 1;
            lcd_tone();
            if(mechineData->speed > 150)
                mechineData->speed = 150;
            else
                IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case MINUS_KEY:
            mechineData->speed -= 1;
            lcd_tone();
            if(mechineData->speed < 10)
                mechineData->speed = 10;
            else
                IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case UP_KEY:
            mechineData->incline += 1;
            lcd_tone();
            if(mechineData->incline >= 12)
                mechineData->incline = 12;
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case DOWN_KEY:
            lcd_tone();
            if(mechineData->incline != 0)
                mechineData->incline -= 1;
            IsDataChanged = YES;
            stateReturn = RUN;
        default:
            stateReturn = RUN;
            break;
    }
    
    /*-------------------FOR EXECISE MODE----------------------*/
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

    /*--------------------FOR SETUP MODE----------------------*/

    /* cacutlate time to change calo and distance and change data*/
    if(SYSTICK_Get_Tick() > startTickForChangeDistance)
        if((SYSTICK_Get_Tick() - startTickForChangeDistance) > (360000/mechineData->speed))
        {
            if(HowToCountData == COUNT_DOWN_DIS)
                mechineData->distance -= 1;
            else
                mechineData->distance += 1;
            startTickForChangeDistance = SYSTICK_Get_Tick();
            updateDistance(mechineData->distance);
        }
    else
        if((startTickForChangeDistance - SYSTICK_Get_Tick()) < (360000/mechineData->speed))
        {
            if(HowToCountData == COUNT_DOWN_DIS)
                mechineData->distance -= 1;
            else
                mechineData->distance += 1;
            startTickForChangeDistance = 0;
            startTickForChangeDistance = SYSTICK_Get_Tick();
            updateDistance(mechineData->distance);
        }
    if(SYSTICK_Get_Tick() > startTickForChangeCalo)
        //must replace by the calorie change time formula
        if((SYSTICK_Get_Tick() - startTickForChangeCalo) > (360000/mechineData->speed))
        {
            if(HowToCountData == COUNT_DOWN_CALO)
                mechineData->calo -= 1;
            else
                mechineData->calo += 1;
            startTickForChangeCalo = 0;
            updateCalo(mechineData->calo);
        }
    else
        //must replace by the calorie change time formula
        if((SYSTICK_Get_Tick() - startTickForChangeCalo) > (360000/mechineData->speed))
        {
            if(HowToCountData == COUNT_DOWN_CALO)
                mechineData->calo -= 1;
            else
                mechineData->calo += 1;
            startTickForChangeCalo = 0;
            updateCalo(mechineData->calo);
        }

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
                    timer_3_stop();
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
                    timer_3_stop();
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
                    timer_3_stop();
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
