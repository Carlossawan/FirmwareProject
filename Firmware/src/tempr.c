/**
 * @file tempr.c
 * @author Majdi Richa (majdi.richa@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-11-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "main.h"
#include "tempr.h"
#include "adc.h"

float read_temperature(void)
{
    uint16_t adc_value = get_adc(0); // Read ADC value from channel 0
    float temperature = adc_value/10.24; // Convert voltage to temperature (10 mV/�C)

    return temperature; // Return temperature in �C
}
