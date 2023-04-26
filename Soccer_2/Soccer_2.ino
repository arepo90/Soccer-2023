/*
    Main code v1.2 - Secondary robot - 25/04/2023 - Soccer 2023
    by Esteban Martinez & GPT-4

    STABLE (???) VERSION
    I dont even know anymore
    I2C problem never happened here, but still

    US are unreliable af even with new library
*/

#include "defs.h"

//---------------Hardware definitions---------------

//Motor 1 - Back Left
#define PWM_1 2
#define DIR_A1 42
#define DIR_B1 44
//Motor 2 - Front Left
#define PWM_2 4
#define DIR_A2 47
#define DIR_B2 46
//Motor 3 - Front Right
#define PWM_3 7
#define DIR_A3 24
#define DIR_B3 26
//Motor 4 - Back Right
#define PWM_4 9
#define DIR_A4 30
#define DIR_B4 28

//A: Outer, B: Inner
//Light 1 - Back
#define LUZ_A1 A1 
#define LUZ_B1 A3
#define LIM_A1 500
#define LIM_B1 600
//Light 2 - Left
#define LUZ_A2 A5
#define LUZ_B2 A4
#define LIM_A2 600
#define LIM_B2 600
//Light 3 - Front
#define LUZ_A3 A2
#define LUZ_B3 A0
#define LIM_A3 500
#define LIM_B3 600
//Light 4 - Right
#define LUZ_A4 A6
#define LUZ_B4 A7
#define LIM_A4 500
#define LIM_B4 600

//Ultrasonic 1 - Left
#define US_E1 38
#define US_T1 40
//Ultrasonic 2 - Right
#define US_E2 5
#define US_T2 3
//Distance Limit
#define US_LIM 300
#define MIN_DIS 20
#define MAX_DIS 60

//Compass address, message and limit
#define C1 0x01
#define C2 0x44
//Angle limit
#define C_LIM 5

//IR Seeker address and message
#define IR1 0x10 / 2
#define IR2 0x49
//Angle limit
#define IR_LIM 30
#define IR_CORR 45

//Motor powers
#define POW1 150
#define POW2 150
#define POW3 150
#define POW4 150

//Movement settings
#define DEF NaN
#define BRAKE 250
#define FACTOR 3.0
#define KPI 50
#define KPF 200
#define DEL 120

//Helper variables
ul trackNow = 0, trackBefore = 0;
int lin1 = 0, lin2 = 0, lin3 = 0, lin4 = 0;
const int robotId = 1; 
bool led = true;

//---------------Hardware classes declarations---------------

//Motor declarations
Motor M1(1, PWM_1, DIR_A1, DIR_B1, POW1);
Motor M2(2, PWM_2, DIR_A2, DIR_B2, POW2);
Motor M3(3, PWM_3, DIR_A3, DIR_B3, POW3);
Motor M4(4, PWM_4, DIR_A4, DIR_B4, POW4);

//Light sensor declarations
//Last argument changes the limit's source (0: setLim(), 1: EEPROM)
bool arg1 = false;
Light L1(1, LUZ_A1, LUZ_B1, arg1);
Light L2(2, LUZ_A2, LUZ_B2, arg1);
Light L3(3, LUZ_A3, LUZ_B3, arg1);
Light L4(4, LUZ_A4, LUZ_B4, arg1);

//Ultrasonic sensor declarations
US U1(1, US_T1, US_E1, US_LIM);
US U2(2, US_T2, US_E2, US_LIM);

//Compass sensor declarations
Compass Comp(1, C1, C2, C_LIM);

//IR Seeker declarations
IRSeeker IR(1, IR1, IR2);

//---------------Main code---------------

void setup(){
    globalInit(3);
    Serial.println("3 pesos");
}

void loop(){
    gp();
    Serial.println();
}
