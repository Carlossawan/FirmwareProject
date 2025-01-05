#ifndef RTC_H
#define RTC_H

#include <stdint.h>

typedef struct
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day;   
    uint8_t month; 
    uint8_t year;  
} custom_rtc_time_t;

void rtc_initialize(void);                    
void rtc_get_time(custom_rtc_time_t *time);     

#endif

