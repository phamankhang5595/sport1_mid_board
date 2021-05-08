
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "board.h"
#include "uart.h"
/*******************************************************************************
* Definition
*******************************************************************************/
type_UartCallBackInit   type_UART3CallBackFunc;
type_UartCallBackInit   type_Uart2CallBackFunc;
/*******************************************************************************
* Code
*******************************************************************************/
void UART_PinInit(USART_TypeDef* USARTx)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /*Config USART3 Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = UART3_RxPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(UART3_GPIO, &GPIO_InitStructure);
    /*Config USART3 Tx as alternate function pp*/
    GPIO_InitStructure.GPIO_Pin = UART3_TxPin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(UART3_GPIO, &GPIO_InitStructure);
}

void UART_ClockInit(USART_TypeDef* USARTx)
{
        /*Enable GPIO clock*/
        RCC_APB2PeriphClockCmd(UART3_GPIO_CLK, ENABLE);
        /*Enable AFIO clock*/
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        /*Enable UART clock*/
        RCC_APB1PeriphClockCmd(UART3_CLK, ENABLE);
}


void UART_Init(USART_TypeDef* USARTx, u32 baudrate, u16 mode)
{
    USART_InitTypeDef USART_InitStructure;
    UART_ClockInit(USARTx);
    UART_PinInit(USARTx);
    
    /*UARTx Config*/
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = mode;
    
    /*USART Config Init*/
    USART_Init(USARTx, &USART_InitStructure);

    /* Enable USARTx Receive interrupts */
    USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);

    /* NVIC Config */
    NVIC_EnableIRQ(USART3_IRQn);
    /*Enable UARTx*/
    USART_Cmd(USARTx, ENABLE);
}


void UART_CallBackInit(USART_TypeDef* USARTx, type_UartCallBackInit pHandle)
{
    type_UART3CallBackFunc = pHandle;
}


void UART_SendData(USART_TypeDef* USARTx, uint8_t* buff, uint8_t len)
{
    for(uint8_t i = 0; i < len; i++)
    {
        USART_SendData(USARTx, buff[i]);
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET); 
    }
}