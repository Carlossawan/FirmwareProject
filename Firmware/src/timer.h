#ifndef TIMER_H
#define TIMER_H

#include "scheduler.h"
#include <stdint.h>


extern volatile uint8_t hours;
extern volatile uint8_t minutes;
extern volatile uint8_t seconds;




void TIMER1_isr(void);
#define  TMR1Reload      2000     

cntr_size_t get_ticks_counter(void);

#endif
