#include "main.h"
#include "timer.h"


//Globals:
cntr_size_t local_sys_tick_counter  = 0; //tick counter
volatile uint8_t hours = 11;    // Start at 12:00:00 (arbitrary initialization)
volatile uint8_t minutes = 0;
volatile uint8_t seconds = 0;


/**
 * ISR for TIMER1 in order to generate a 1ms local_sys_tick
 * 
 * @author m_richa 
 * @date 
 */
#int_TIMER1

#int_TIMER1
void TIMER1_isr(void)
{
    set_timer1(get_timer1() - TMR1Reload);  /* Re-load timer */
    local_sys_tick_counter++;

    
    static uint16_t ms_counter = 0;
    ms_counter++;
    if (ms_counter >= 1000)  
    {
        ms_counter = 0;  
        seconds++;
        if (seconds >= 60)
        {
            seconds = 0;
            minutes++;
            if (minutes >= 60)
            {
                minutes = 0;
                hours++;
                if (hours >= 24)
                {
                    hours = 0;  
                }
            }
        }
    }
}


/**
 * Function to return system ticks counter in ms, passed to scheduler
 * 
 * @author m_richa 
 * @date 
 * 
 * @return cntr_size_t system ticks count in ms
 */
cntr_size_t get_ticks_counter(void)
{
    return local_sys_tick_counter;
}
