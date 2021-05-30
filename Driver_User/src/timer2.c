#include "common.h"
#include "stm32f10x_tim.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "timer2.h"
p_callbackfunction tim_callback_func;
/*******************************************************************************
* Code
*******************************************************************************/

void timer_2_init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    TIM_TimeBaseInitTypeDef timerBaseInit;
    timerBaseInit.TIM_CounterMode = TIM_CounterMode_Down;
    timerBaseInit.TIM_ClockDivision = TIM_CKD_DIV4;
    timerBaseInit.TIM_Prescaler = 72000000/10000 - 1;   /* Ftimer = 10KHz */
    timerBaseInit.TIM_Period = 10000 - 1;               /* 1s */
    TIM_TimeBaseInit(TIM2, &timerBaseInit);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

void timer_callback_init(p_callbackfunction callbackfunc)
{
    tim_callback_func = callbackfunc;
}

void timer_2_start()
{
    TIM_Cmd(TIM2,ENABLE);
    NVIC_EnableIRQ(TIM2_IRQn);
}

void timer_2_stop()
{
    TIM_Cmd(TIM2,DISABLE);
    NVIC_DisableIRQ(TIM2_IRQn);
}

void TIM2_IRQHandler()
{
    TIM_ClearFlag(TIM2,TIM_FLAG_Update);
    tim_callback_func();
}