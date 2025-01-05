
#ifndef  HARDWARE_H
#define  HARDWARE_H
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

// Floor Data





#include "stdbool.h"    
uint8_t current_floor = 0; 
uint8_t target_floor = 0;  
bool is_moving = false;   

// Function Prototypes
void update_display(uint8_t floor);
uint8_t read_buttons(void);
void move_elevator(void);


//List all IOs here:


///////////////////////////////////////////////////////////////////////////////
bool init_hw(void);
///////////////////////////////////////////////////////////////////////////////
#endif
