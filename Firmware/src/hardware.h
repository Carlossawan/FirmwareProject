#ifndef HARDWARE_H
#define HARDWARE_H
#byte PORTD = 0xF83

#define BCD_PORT PORTD

#define DIGIT_1_BCD0 PIN_E4
#define DIGIT_1_BCD1 PIN_E5
#define DIGIT_1_BCD2 PIN_E6
#define DIGIT_1_BCD3 PIN_E7

#define DIGIT_2_BCD0 PIN_E0
#define DIGIT_2_BCD1 PIN_E1
#define DIGIT_2_BCD2 PIN_E2
#define DIGIT_2_BCD3 PIN_E3

#define DIGIT_3_BCD0 PIN_D4
#define DIGIT_3_BCD1 PIN_D5
#define DIGIT_3_BCD2 PIN_D6
#define DIGIT_3_BCD3 PIN_D7

#define DIGIT_4_BCD0 PIN_D0
#define DIGIT_4_BCD1 PIN_D1
#define DIGIT_4_BCD2 PIN_D2
#define DIGIT_4_BCD3 PIN_D3

// Pin Definitions
#define SEG_A PIN_G0
#define SEG_B PIN_G1
#define SEG_C PIN_G2
#define SEG_D PIN_G3

#define Cab_GF PIN_C7 // Ground Floor
#define Cab_F1 PIN_C6 // Floor 1
#define Cab_F2 PIN_B7 // Floor 2
#define Cab_F3 PIN_B6 // Floor 3

#define flr3_Btn_Dn PIN_B0

#define flr2_Btn_Up PIN_B1
#define flr2_Btn_Dn PIN_B2

#define flr1_Btn_Up PIN_B3
#define flr1_Btn_Dn PIN_B4

#define flr0_Btn_Up PIN_B5

// Speed and stop sensors
#define Flr3_Stop PIN_A1
#define Flr3_Spd_Chg1 PIN_A2

#define Flr2_Stop PIN_A4
#define Flr2_Spd_Chg2 PIN_A3
#define Flr2_Spd_Chg1 PIN_A5

#define Flr1_Spd_Chg2 PIN_A6
#define Flr1_Stop PIN_A7
#define Flr1_Spd_Chg1 PIN_C0

#define Flr0_Spd_Chg2 PIN_C1
#define Flr0_Stop PIN_C2

#include "stdbool.h"
uint8_t current_floor = 0;
uint8_t target_floor = 0;
bool is_moving = false;
uint8_t elevator_speed = 1; // Default speed

// Function Prototypes
void update_display(uint8_t floor);
uint8_t read_buttons(void);
void move_elevator(void);
void adjust_speed(void);

// List all IOs here:

///////////////////////////////////////////////////////////////////////////////
bool init_hw(void);
///////////////////////////////////////////////////////////////////////////////
#endif
