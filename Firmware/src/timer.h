#ifndef TIMER_H
#define TIMER_H

#include "scheduler.h"
#include <stdint.h>

// External declarations for time variables
extern volatile uint8_t hours;
extern volatile uint8_t minutes;
extern volatile uint8_t seconds;



//Timer related:
void TIMER1_isr(void);
#define  TMR1Reload      2000     //running @ 16MIPS

cntr_size_t get_ticks_counter(void);

#endif
