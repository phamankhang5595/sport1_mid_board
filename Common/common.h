#ifndef __COMMON_H__
#define __COMMON_H__
#include "stdint.h"
/*******************************************************************************
 * Definition
 ******************************************************************************/
/* program states */
typedef enum
{
    START,
    RUN,
    EXERCISE_SET,
    USER_SET,
    STOP,
    IDLE
} program_state_t;

/* program data for display */
typedef struct
{
    uint8_t runEx;
    uint8_t runSpeed;
    int runTime;
    uint8_t dis;
    uint8_t clo;
    uint8_t incl;
} run_mechine_data_t;

#endif /* common.h */