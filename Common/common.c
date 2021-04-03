#include "common.h"
/*******************************************************************************
 * Variable
 ******************************************************************************/
uint8_t IsDataChanged = YES;
/*******************************************************************************
 * Code
 ******************************************************************************/
void resetRunMechineData(run_mechine_data_t *mechineData)
{
    mechineData->calo       = 0;
    mechineData->distance   = 0;
    mechineData->incline    = 0;
    mechineData->runEx      = DEFAULT_RUN_EX;
    mechineData->runTime    = 0;
}