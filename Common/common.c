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
    mechineData->calo       = DEFAULT_CALO;
    mechineData->distance   = DEFAULT_DISTANCE;
    mechineData->incline    = DEFAULT_INCLINE;
    mechineData->runEx      = DEFAULT_RUN_EX;
    mechineData->runTime    = DEFAULT_RUN_TIME;
}