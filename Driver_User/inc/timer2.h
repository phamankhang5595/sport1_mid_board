#ifndef __TIMER2_H__
#define __TIMER2_H__
typedef void (*p_callbackfunction)(void);
void timer_callback_init(p_callbackfunction callbackfunc);
void timer_2_init();
void timer_2_stop();
void timer_2_start();
#endif  /* __TIMER1_H__ */