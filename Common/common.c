#include "common.h"
/*******************************************************************************
 * Variable
 ******************************************************************************/
volatile uint8_t IsDataChanged = YES;
uint32_t IsThisTheFirstTimeRun = YES;
/*******************************************************************************
 * Code
 ******************************************************************************/
void reset_run_mechineData(run_mechine_data_t *mechineData)
{
    mechineData->calo       = 0;
    mechineData->distance   = 0;
    mechineData->incline    = 0;
    mechineData->runEx      = DEFAULT_RUN_EX;
    mechineData->runTime    = 0;
}