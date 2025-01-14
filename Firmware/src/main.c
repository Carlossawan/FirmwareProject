#include "main.h"
#include "hardware.h"
#include "system.h"
#include "scheduler.h"
#include "adc.h"
#include "tempr.h"
#include "rtc.h"
#include "timer.h"
#include "rtc.c"

#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7, stream=UART1)

s_task_handle_t display_task;

void time_temperature_display_task(s_task_handle_t me, s_task_msg_t **msg, void* arg);
void display_number(uint8_t numbers[4]);
void display_digit(uint8_t digit, uint8_t value);
float read_temperature(void);
void set_motor_speed(uint8_t speed);
void set_motor_direction(bool up);
void check_floor_signals(void);
void move_elevator(void);
void adjust_speed(void);

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
                set_motor_direction(current_floor < target_floor); // Set direction
                set_motor_speed(2); // Start at high speed
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

// Update display
void update_display(uint8_t floor)
{
    if (floor > 3) return;
    output_bit(SEG_A, (floor & 0b0001));
    output_bit(SEG_B, (floor & 0b0010) >> 1);
    output_bit(SEG_C, (floor & 0b0100) >> 2);
    output_bit(SEG_D, (floor & 0b1000) >> 3);
}

// Read buttons
uint8_t read_buttons(void)
{
    if (!input(Cab_GF)) return 0;
    if (!input(Cab_F1)) return 1;
    if (!input(Cab_F2)) return 2;
    if (!input(Cab_F3)) return 3;

    if (!input(flr3_Btn_Dn)) return 3;
    if (!input(flr2_Btn_Up)) return 2;
    if (!input(flr2_Btn_Dn)) return 2;
    if (!input(flr1_Btn_Up)) return 1;
    if (!input(flr1_Btn_Dn)) return 1;
    if (!input(flr0_Btn_Up)) return 0;

    return 255;
}

// Check signals
void check_floor_signals(void)
{
    if (!input(Flr3_Stop) || !input(Flr2_Stop) || !input(Flr1_Stop) || !input(Flr0_Stop))
    {
        set_motor_speed(0);
        is_moving = false;
        current_floor = target_floor;
        update_display(current_floor);
    }
    else if ((current_floor == 3 && !input(Flr3_Spd_Chg1)) ||
             (current_floor == 2 && !input(Flr2_Spd_Chg1)) ||
             (current_floor == 1 && !input(Flr1_Spd_Chg1)))
    {
        set_motor_speed(1);
    }
    else if ((current_floor == 2 && !input(Flr2_Spd_Chg2)) ||
             (current_floor == 1 && !input(Flr1_Spd_Chg2)))
    {
        set_motor_speed(2);
    }
    else
    {
        set_motor_speed(2);
    }
}

// Set motor speed
void set_motor_speed(uint8_t speed)
{
    switch (speed)
    {
    case 0:
        output_low(PIN_F1);
        output_low(PIN_F2);
        break;
    case 1:
        output_low(PIN_F1);
        output_high(PIN_F2);
        break;
    case 2:
        output_high(PIN_F1);
        output_low(PIN_F2);
        break;
    default:
        output_low(PIN_F1);
        output_low(PIN_F2);
        break;
    }
}

// Adjust speed
void adjust_speed(void)
{
    check_floor_signals();
}

// Move elevator
void move_elevator(void)
{
    check_floor_signals();

    if (current_floor < target_floor)
    {
        set_motor_direction(true);
        delay_ms(1000);
        current_floor++;
        update_display(current_floor);
    }
    else if (current_floor > target_floor)
    {
        set_motor_direction(false);
        delay_ms(1000);
        current_floor--;
        update_display(current_floor);
    }
    else
    {
        set_motor_speed(0);
        is_moving = false;
    }
}

// Set direction
void set_motor_direction(bool up)
{
    if (up)
    {
        output_high(PIN_F3);
        output_low(PIN_F4);
    }
    else
    {
        output_low(PIN_F3);
        output_high(PIN_F4);
    }
}

// Time and temperature
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

// Display digit
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

// Display number
void display_number(uint8_t numbers[4])
{
    display_digit(1, numbers[0]);
    display_digit(2, numbers[1]);
    display_digit(3, numbers[2]);
    display_digit(4, numbers[3]);
}

