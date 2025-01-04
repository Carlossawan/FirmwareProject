
#ifndef  HARDWARE_H
#define  HARDWARE_H
#byte PORTD = 0xF83  // Address of PORTD for PIC18F67K22

// Define BCD output port
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






#include "stdbool.h"    /*in order to use "bool" type*/

//List all IOs here:


///////////////////////////////////////////////////////////////////////////////
bool init_hw(void);
///////////////////////////////////////////////////////////////////////////////
#endif