#include "stm32f10x_sdio.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "keypad.h"
#include "systick.h"
/*******************************************************************************
 * Definition
 ******************************************************************************/
#define NOT_PRESS   (0)
#define PRESS       (1)

#define NO_ROW      (9)
/*******************************************************************************
 * Variable
 ******************************************************************************/
keypad_state laststate = releasState;
const char Keypad_Button_Values[4][4] =  {  
                                            {'1', '2', '3', '4'},
                                            {'5', '6', '7', '8'},
                                            {'9', 'A', 'B', 'C'},
                                            {'D', 'E', 'F', 'G'}
                                         };
/*******************************************************************************
 * Private func
 ******************************************************************************/
/*!
 * @brief Set high all col
 *
 * @param none
 */
static void setHighAllCol()
{
    KEYPAD_SET_COL(KEYPAD_COL_1);
    KEYPAD_SET_COL(KEYPAD_COL_2);
    KEYPAD_SET_COL(KEYPAD_COL_3);
    KEYPAD_SET_COL(KEYPAD_COL_4);
}

/*!
 * @brief Set low all col
 *
 * @param none
 */
static void setLowAllCol()
{
    KEYPAD_CLR_COL(KEYPAD_COL_1);
    KEYPAD_CLR_COL(KEYPAD_COL_2);
    KEYPAD_CLR_COL(KEYPAD_COL_3);
    KEYPAD_CLR_COL(KEYPAD_COL_4);
}
/*!
 * @brief Set 1 high low and another low
 *
 * @param col_index     index of col
 * @return  none 
 */
static void setColX(col_index index)
{
    setLowAllCol();
    switch (index)
    {
    case (Col_1):
        /* code */
        KEYPAD_SET_COL(KEYPAD_COL_1);
        break;
    case Col_2:
        KEYPAD_SET_COL(KEYPAD_COL_2);
        break;
    case Col_3:
        KEYPAD_SET_COL(KEYPAD_COL_3);
        break;
    case Col_4:
        KEYPAD_SET_COL(KEYPAD_COL_4);
        break;
    default:
        break;
    }
}

/*!
 * @brief 
 *
 * @return return PRESS: key pressed
 *         return NOT_PRESS: key not press
 */
static uint32_t isKeyPressed()
{
    uint8_t keyStatus = NOT_PRESS;
    setHighAllCol();
    if( KEYPAD_READ_ROW(KEYPAD_ROW_1)||
        KEYPAD_READ_ROW(KEYPAD_ROW_2)||
        KEYPAD_READ_ROW(KEYPAD_ROW_3)||
        KEYPAD_READ_ROW(KEYPAD_ROW_4) )
        keyStatus = PRESS;
    return keyStatus;
}

/*!
 * @brief 
 *
 * @param 
 * @param 
 */
static uint8_t rowCheck()
{
    uint32_t retVal;
    if(KEYPAD_READ_ROW(KEYPAD_ROW_1))
        retVal = Row_1;
    else if(KEYPAD_READ_ROW(KEYPAD_ROW_2))
        retVal = Row_2;
    else if(KEYPAD_READ_ROW(KEYPAD_ROW_3))
        retVal = Row_3;
    else if(KEYPAD_READ_ROW(KEYPAD_ROW_4))
        retVal = Row_4;
    else
        retVal = NO_ROW;
    return (retVal);
}

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Init GPIO pin for keyPad
 *
 * @param None
 */
void KEYPAD_Init()
{
    GPIO_InitTypeDef GPIO_InitStruce;
    /* GPIOA Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    /* Col init */
    GPIO_InitStruce.GPIO_Pin = KEYPAD_COL_1;
    GPIO_InitStruce.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruce.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(KEYPAD_GPIO,&GPIO_InitStruce);

    GPIO_InitStruce.GPIO_Pin = KEYPAD_COL_2;
    GPIO_InitStruce.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruce.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(KEYPAD_GPIO,&GPIO_InitStruce);

    GPIO_InitStruce.GPIO_Pin = KEYPAD_COL_3;
    GPIO_InitStruce.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruce.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(KEYPAD_GPIO,&GPIO_InitStruce);

    GPIO_InitStruce.GPIO_Pin = KEYPAD_COL_4;
    GPIO_InitStruce.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruce.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(KEYPAD_GPIO,&GPIO_InitStruce);

    /* ROW Init*/
    GPIO_InitStruce.GPIO_Pin = KEYPAD_ROW_1;
    GPIO_InitStruce.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruce.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(KEYPAD_GPIO,&GPIO_InitStruce);

    GPIO_InitStruce.GPIO_Pin = KEYPAD_ROW_2;
    GPIO_InitStruce.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruce.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(KEYPAD_GPIO,&GPIO_InitStruce);

    GPIO_InitStruce.GPIO_Pin = KEYPAD_ROW_3;
    GPIO_InitStruce.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruce.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(KEYPAD_GPIO,&GPIO_InitStruce);

    GPIO_InitStruce.GPIO_Pin = KEYPAD_ROW_4;
    GPIO_InitStruce.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruce.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(KEYPAD_GPIO,&GPIO_InitStruce);
}

/*!
 * @brief Scan keypad
 *
 * @param None
 * @return 
 */
char KEYPAD_ScanKey()
{
    char key = NO_KEY;
    uint32_t rowVal;
    if(isKeyPressed())
    {
        /* Debounce time */
        SYSTICK_Delay_ms(20);
        if(isKeyPressed())
        {
            setColX(Col_1);
            rowVal = rowCheck();
            if(rowVal != NO_ROW)
                key = Keypad_Button_Values[rowVal][Col_1];
            else
            {
                setColX(Col_2);
                rowVal = rowCheck();
                if(rowVal != NO_ROW)
                    key = Keypad_Button_Values[rowVal][Col_2];
                else
                {
                    setColX(Col_3);
                    rowVal = rowCheck();
                    if(rowVal != NO_ROW)
                        key = Keypad_Button_Values[rowVal][Col_3];
                    else
                    {
                        setHighAllCol();
                        rowVal = rowCheck();
                        if(rowVal != NO_ROW)
                            key = Keypad_Button_Values[rowVal][Col_4];
                    }
                }

            }
        }
    }
    return (key);
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
