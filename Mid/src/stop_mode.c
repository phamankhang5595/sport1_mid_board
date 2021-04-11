#include "stop_mode.h"
#include "screen.h"
#include "systick.h"
/*******************************************************************************
 * Code
 ******************************************************************************/
program_state_t stopMode(run_mechine_data_t *mechineData, program_state_t *laststate)
{
    /* send stop command */
    while(mechineData->speed)
    {
        /* get speed value */
        mechineData->speed -= 1;
        updateSpeed(mechineData->speed);
    }
    resetRunMechineData(mechineData);
    *laststate = STOP;
    return (START);
}