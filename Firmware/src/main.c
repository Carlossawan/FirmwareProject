#include "main.h"
#include "hardware.h"
#include "system.h"
#include "scheduler.h"
#include "adc.h"
#include "tempr.h"
#include "rtc.h"
#include "timer.h"
#include "rtc.c"

s_task_handle_t display_task;

void time_temperature_display_task(s_task_handle_t me, s_task_msg_t **msg, void* arg);
void display_number(uint8_t numbers[4]);
void display_digit(uint8_t digit, uint8_t value);
float read_temperature(void);

void main()
{
    bool ret;
    delay_ms(100);

    ret = scheduler_init(get_ticks_counter);
    if (!ret) while (true);

    ret &= init_hw();
    ret &= init_system();
    ret &= init_adc();
    rtc_initialize();

    update_display(0);

    ret &= s_task_create(true, S_TASK_LOW_PRIORITY, 1000, time_temperature_display_task, NULL, NULL);

    if (ret)
    {
        while (true)
        {
            scheduler();
            uint8_t selected_floor = read_buttons();
            if (selected_floor != 255)
            {
                target_floor = selected_floor;
                is_moving = true;
            }
            if (is_moving)
            {
                move_elevator();
            }
            delay_ms(100);
        }
    }
    else
    {
        while (true);
    }
}

// Update display with floor
void update_display(uint8_t floor)
{
    if (floor > 3) return;
    output_bit(SEG_A, (floor & 0b0001));
    output_bit(SEG_B, (floor & 0b0010) >> 1);
    output_bit(SEG_C, (floor & 0b0100) >> 2);
    output_bit(SEG_D, (floor & 0b1000) >> 3);
}

// Read button states
uint8_t read_buttons(void)
{
    // Inside elevator buttons
    if (!input(Cab_GF)) return 0; // Ground Floor (0)
    if (!input(Cab_F1)) return 1; // Floor 1
    if (!input(Cab_F2)) return 2; // Floor 2
    if (!input(Cab_F3)) return 3; // Floor 3

    // Outside elevator buttons
    if (!input(flr3_Btn_Dn)) return 3; // Floor 3 Down
    if (!input(flr2_Btn_Up)) return 2; // Floor 2 Up
    if (!input(flr2_Btn_Dn)) return 2; // Floor 2 Down
    if (!input(flr1_Btn_Up)) return 1; // Floor 1 Up
    if (!input(flr1_Btn_Dn)) return 1; // Floor 1 Down
    if (!input(flr0_Btn_Up)) return 0; // Floor 0 Up

    return 255; // No button pressed
}

// Handle elevator movement
void move_elevator(void)
{
    static bool initial_delay = true;

    if (initial_delay)
    {
        delay_ms(1000); // Initial delay
        initial_delay = false;
    }

    adjust_speed();

    uint16_t delay_time;
    switch (elevator_speed)
    {
        case 1: // Low speed
            delay_time = 3000; // Adjust this value for low speed
            break;
        case 2: // Medium speed
            delay_time = 2000; // Adjust this value for medium speed
            break;
        case 3: // High speed
            delay_time = 1000; // Adjust this value for high speed
            break;
        default:
            delay_time = 2000; // Default to medium speed
            break;
    }

    if (current_floor < target_floor)
    {
        delay_ms(delay_time);
        current_floor++;
        update_display(current_floor);
    }
    else if (current_floor > target_floor)
    {
        delay_ms(delay_time);
        current_floor--;
        update_display(current_floor);
    }
    else
    {
        is_moving = false;
        initial_delay = true;
    }
}

// Adjust elevator speed based on position
void adjust_speed(void)
{
    if (current_floor == target_floor)
    {
        elevator_speed = 1; // Low speed (Stop)
    }
    else if ((target_floor > current_floor && current_floor + 1 == target_floor) || 
             (target_floor < current_floor && current_floor - 1 == target_floor))
    {
        elevator_speed = 2; // Medium speed (Deceleration)
    }
    else
    {
        elevator_speed = 3; // High speed (Cruising speed)
    }
}

// Display time, date, temperature
void time_temperature_display_task(s_task_handle_t me, s_task_msg_t **msg, void* arg)
{
    static uint8_t display_mode = 0;
    static uint8_t seconds_elapsed = 0;

    custom_rtc_time_t rtc_time;
    rtc_get_time(&rtc_time);

    float temperature = read_temperature();

    seconds_elapsed++;
    if (seconds_elapsed >= 10)
    {
        seconds_elapsed = 0;
        display_mode = (display_mode + 1) % 3;
    }

    uint8_t digits[4] = {0, 0, 0, 0};

    switch (display_mode)
    {
    case 0:
        digits[0] = rtc_time.hours / 10;
        digits[1] = rtc_time.hours % 10;
        digits[2] = rtc_time.minutes / 10;
        digits[3] = rtc_time.minutes % 10;
        break;
    case 1:
        digits[0] = rtc_time.day / 10;
        digits[1] = rtc_time.day % 10;
        digits[2] = rtc_time.month / 10;
        digits[3] = rtc_time.month % 10;
        break;
    case 2:
        digits[0] = (uint8_t)temperature / 10;
        digits[1] = (uint8_t)temperature % 10;
        digits[2] = (uint8_t)(temperature * 10) % 10;
        digits[3] = 12;
        break;
    default:
        digits[0] = digits[1] = digits[2] = digits[3] = 0;
        break;
    }

    display_number(digits);
}

// Display digit values
void display_digit(uint8_t digit, uint8_t value)
{
    switch (digit)
    {
    case 1:
        output_bit(DIGIT_1_BCD0, (value & 0b0001));
        output_bit(DIGIT_1_BCD1, (value & 0b0010) >> 1);
        output_bit(DIGIT_1_BCD2, (value & 0b0100) >> 2);
        output_bit(DIGIT_1_BCD3, (value & 0b1000) >> 3);
        break;
    case 2:
        output_bit(DIGIT_2_BCD0, (value & 0b0001));
        output_bit(DIGIT_2_BCD1, (value & 0b0010) >> 1);
        output_bit(DIGIT_2_BCD2, (value & 0b0100) >> 2);
        output_bit(DIGIT_2_BCD3, (value & 0b1000) >> 3);
        break;
    case 3:
        output_bit(DIGIT_3_BCD0, (value & 0b0001));
        output_bit(DIGIT_3_BCD1, (value & 0b0010) >> 1);
        output_bit(DIGIT_3_BCD2, (value & 0b0100) >> 2);
        output_bit(DIGIT_3_BCD3, (value & 0b1000) >> 3);
        break;
    case 4:
        output_bit(DIGIT_4_BCD0, (value & 0b0001));
        output_bit(DIGIT_4_BCD1, (value & 0b0010) >> 1);
        output_bit(DIGIT_4_BCD2, (value & 0b0100) >> 2);
        output_bit(DIGIT_4_BCD3, (value & 0b1000) >> 3);
        break;
    default:
        break;
    }
}

// Display four digits
void display_number(uint8_t numbers[4])
{
    display_digit(1, numbers[0]);
    display_digit(2, numbers[1]);
    display_digit(3, numbers[2]);
    display_digit(4, numbers[3]);
}