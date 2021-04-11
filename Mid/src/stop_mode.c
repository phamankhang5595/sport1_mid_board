#include "stop_mode.h"
#include "screen.h"
#include "systick.h"
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief The stop mode
 *
 * @param mechineData
 * @param laststate 
 * @return State of program
*/
program_state_t stop_mode(run_mechine_data_t *mechineData, program_state_t *laststate)
{
    /* send stop command */
    while(mechineData->speed)
    {
        /* get speed value */
        mechineData->speed -= 1;
        updateSpeed(mechineData->speed);
    }
    reset_run_mechineData(mechineData);
    *laststate = STOP;
    return (START);
}