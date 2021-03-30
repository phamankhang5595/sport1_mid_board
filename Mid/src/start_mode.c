#include "start_mode.h"
#include "keypad.h"
#include "common.h"
#include "screen.h"
#include "lcd.h"
#include "systick.h"
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
    /* start display */
    lcd_clr();
    
    SYSTICK_Delay_ms(20);
    key = KEYPAD_ScanKey();
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
