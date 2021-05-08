#include "common.h"
#include "stm32f10x_tim.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "timer3.h"
p_callbackfunction tim_callback_func;
/*******************************************************************************
* Code
*******************************************************************************/

void timer_3_init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    TIM_TimeBaseInitTypeDef timerBaseInit;
    timerBaseInit.TIM_CounterMode = TIM_CounterMode_Down;
    timerBaseInit.TIM_ClockDivision = TIM_CKD_DIV4;
    timerBaseInit.TIM_Prescaler = 72000000/10000 - 1;   /* Ftimer = 10KHz */
    timerBaseInit.TIM_Period = 10000 - 1;               /* 1s */
    TIM_TimeBaseInit(TIM3, &timerBaseInit);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
}

void timer_callback_init(p_callbackfunction callbackfunc)
{
    tim_callback_func = callbackfunc;
}

void timer_3_start()
{
    TIM_Cmd(TIM3,ENABLE);
    NVIC_EnableIRQ(TIM3_IRQn);
}

void timer_3_stop()
{
    TIM_Cmd(TIM3,DISABLE);
    NVIC_DisableIRQ(TIM3_IRQn);
}

void TIM3_IRQHandler()
{
    TIM_ClearFlag(TIM3,TIM_FLAG_Update);
    tim_callback_func();
}