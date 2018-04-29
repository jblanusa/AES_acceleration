#ifndef __TIMER_H_
#define __TIMER_H_

// global timer
void timer_start_global();
int  timer_elapsed_global();
// user timer
void timer_reset();
void timer_start(int ms);
int  timer_read_to();
void timer_wait(int ms);
void timer_wait_for_to();
int  timer_elapsed();

#endif // __TIMER_H_
