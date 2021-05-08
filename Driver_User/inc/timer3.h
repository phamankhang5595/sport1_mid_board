#ifndef __TIMER3_H__
#define __TIMER3_H__
typedef void (*p_callbackfunction)(void);
void timer_callback_init(p_callbackfunction callbackfunc);
void timer_3_init();
void timer_3_stop();
void timer_3_start();
#endif  /* __TIMER3_H__ */