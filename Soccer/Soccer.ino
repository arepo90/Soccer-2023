/*
    Main code v2.0 early - 17/06/2023 - Soccer 2023
    by Esteban Martinez & GPT-4

    RoboCup version
    IR SENSOR WORKS LETS GOOOOO
    Should test with different parameters

    *NEW*
    Robot behaviour is now chosen through parameters
    ROBOT_ID in defs.h will determine slave/master

    *TODO*
    Check that comms functions work with classes
    Rewrite gp and helper functions
*/

#include "defs.h"

//---------------Hardware definitions---------------

//Motor 1 - Back Left
#define PWM_A1 1
#define PWM_B1 2
//Motor 2 - Front Left
#define PWM_A2 3
#define PWM_B2 4
//Motor 3 - Front Right
#define PWM_A3 5
#define PWM_B3 6
//Motor 4 - Back Right
#define PWM_A4 7
#define PWM_B4 8

//A: Outer, B: Inner
//Light 1 - Back
#define LUZ_A1 9
#define LUZ_B1 10
#define LIM_A1 800
#define LIM_B1 800
//Light 2 - Left
#define LUZ_A2 11
#define LUZ_B2 12
#define LIM_A2 600
#define LIM_B2 600
//Light 3 - Front
#define LUZ_A3 13
#define LUZ_B3 14
#define LIM_A3 600
#define LIM_B3 600
//Light 4 - Right
#define LUZ_A4 15
#define LUZ_B4 15
#define LIM_A4 800
#define LIM_B4 800

//Ultrasonic 1 - Left
#define US_E1 12
#define US_T1 14
//Ultrasonic 2 - Right
#define US_E2 4
#define US_T2 0
//Distance limit (cm)
#define US_LIM 400
#define MIN_DIS 20
#define MAX_DIS 60

//Compass address, message and limit
#define C1 0x01
#define C2 0x44
#define C_LIM 5

//IR and Multiplexer pins and aliases
#define IR0 27
#define IR1 26
#define IR2 25
#define IR3 33
#define IR_IN 32
#define IR_SAMPLES 500
#define IR_DELAY 5
int IR_PINS[16] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15, 14, 13, 1, 0};

//I2C Slave address for ESP <-> ESP communications
#define COMMS_ADDRESS 69

//Motor powers
#define POW1 60
#define POW2 60
#define POW3 60
#define POW4 60

//Movement settings
#define DEF NaN
#define BRAKE 200
#define FACTOR 3.0
#define KPI 30
#define KPF 100
#define DEL 200

//Wireless comms address
uint8_t WL_Address[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//Helper variables
int lin1 = 0, lin2 = 0, lin3 = 0, lin4 = 0;
bool led = true;

//---------------Hardware classes declarations---------------

//I2C Comms(0, COMMS_ADDRESS, 21);

#if ROBOT_ID == 0

//Motor declarations
Motor M1(1, PWM_A1, PWM_B1, POW1);
Motor M2(2, PWM_A2, PWM_B2, POW2);
Motor M3(3, PWM_A3, PWM_B3, POW3);
Motor M4(4, PWM_A4, PWM_B4, POW4);

//Light sensor declarations
//Last argument changes the limit's source (0: setLim(), 1: EEPROM)
Light L1(1, LUZ_A1, LUZ_B1, LIM_A1, LIM_B1);
Light L2(2, LUZ_A2, LUZ_B2, LIM_A2, LIM_B2);
Light L3(3, LUZ_A3, LUZ_B3, LIM_A3, LIM_B3);
Light L4(4, LUZ_A4, LUZ_B4, LIM_A4, LIM_B4);

Wireless WL(0, WL_Address);

#else

//Ultrasonic sensor declarations
//Limit argument changes the max distance (cm)
US U1(1, US_T1, US_E1, US_LIM);
US U2(2, US_T2, US_E2, US_LIM);

//Compass sensor declarations
//Limit argument sets north threshold (symmetrical, in degrees)
Compass Comp(1, C1, C2, C_LIM);

//IR Seeker declarations
Infrared IR(1, IR_PINS, IR0, IR1, IR2, IR3, IR_IN, IR_SAMPLES, IR_DELAY);

#endif

//---------------Main code---------------

void setup(){
    globalInit(0);
    Serial.println("4 pesos");
}

void loop(){
    //IR.read(0);
}
