#include <stdint.h>
#include "stm32f10x_usart.h"
#include "uart.h"
#include "xor.h"
#include "power_communicate.h"

/*******************************************************************************
* Definition
*******************************************************************************/

/*******************************************************************************
* Private func
*******************************************************************************/

/*******************************************************************************
* Code
*******************************************************************************/

uint32_t power_com_init()
{
    UART_Init(USART3,1200,USART_Mode_Tx|USART_Mode_Rx);
    return 0;
}

uint32_t power_com_send_cmd(power_com_cmd_t *command, uint32_t commandLength)
{
    UART_SendData(USART3,(uint8_t *)command,command->length+5);
    return 0;
}

power_com_cmd_t power_com_data_to_cmd(uint8_t speed, uint8_t incline)
{
    static uint8_t lastSpeed = 0;
    static uint8_t lastIncline = 0;
    power_com_cmd_t cmdReturn;
    if(lastSpeed != speed)
    {
        cmdReturn.command = SET_SPEED_MOTOR;
        cmdReturn.type    = MASTER_REQUEST_TYPE;
        cmdReturn.sequence= 0x00;
        cmdReturn.length  = 0x01;
        cmdReturn.buff[0] = speed;
        cmdReturn.buff[1] = XOR_Caculator((uint8_t*)&cmdReturn,0,5);
        lastSpeed = speed;
    }
    else if(lastIncline != incline)
    {
        cmdReturn.command = SET_INCLINE;
        cmdReturn.type    = MASTER_REQUEST_TYPE;
        cmdReturn.sequence= 0x00;
        cmdReturn.length  = 0x01;
        cmdReturn.buff[0] = incline;
        cmdReturn.buff[1] = XOR_Caculator((uint8_t*)&cmdReturn,0,5);
        lastIncline = incline;
    }
    else
        cmdReturn.command = 0xff;
    return (cmdReturn);
}
