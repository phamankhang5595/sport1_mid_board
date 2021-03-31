#ifndef __KEY_PAD_H__
#define __KEY_PAD_H__
/*******************************************************************************
 * Include file
 ******************************************************************************/
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stdint.h"
/*******************************************************************************
 * Definition
 ******************************************************************************/
/* define key name */
#define INCLINE_3_KEY       '3'
#define INCLINE_6_KEY       '2'
#define INCLINE_9_KEY       '1'
#define PLUS_KEY            '4'
#define MINUS_KEY           '5'
#define EXE_KEY             '6'
#define RUN_KEY             '7'
#define STOP_KEY            '8'
#define SETUP_KEY           '9'
#define UP_KEY              'A'
#define DOWN_KEY            'B'
#define SPEED_3_KEY         'C'
#define SPEED_6_KEY         'D'
#define SPEED_9_KEY         'E'

#define NO_KEY            (0xff)

/* keypad gpio  */
#define KEYPAD_GPIO       GPIOA
#define KEYPAD_COL_1      GPIO_Pin_0
#define KEYPAD_COL_2      GPIO_Pin_1
#define KEYPAD_COL_3      GPIO_Pin_2
#define KEYPAD_COL_4      GPIO_Pin_3
#define KEYPAD_ROW_1      GPIO_Pin_4
#define KEYPAD_ROW_2      GPIO_Pin_5
#define KEYPAD_ROW_3      GPIO_Pin_6
#define KEYPAD_ROW_4      GPIO_Pin_7

/* Read write macro */
#define KEYPAD_READ_ALL_ROW         (GPIO_ReadInputData(KEYPAD_GPIO))
#define KEYPAD_READ_ROW(x)          (GPIO_ReadInputDataBit(KEYPAD_GPIO, x))
#define KEYPAD_SET_COL(x)           (GPIO_SetBits(KEYPAD_GPIO,x))
#define KEYPAD_CLR_COL(x)           (GPIO_ResetBits(KEYPAD_GPIO,x))

/* col and row index enum */
typedef enum
{
    Col_1 = 0,
    Col_2,
    Col_3,
    Col_4
} col_index;

typedef enum
{
    Row_1 = 0,
    Row_2,
    Row_3,
    Row_4
} row_index;

/* key state */
typedef enum
{
    pressState,
    holdState,
    releasState
} keypad_state;
/*******************************************************************************
 * API
 ******************************************************************************/
void KEYPAD_Init();
char KEYPAD_ScanKey();
#endif  /* KEY_PAD */
