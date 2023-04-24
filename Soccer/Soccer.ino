/*
    Main code v1.1.7 - 22/04/2023 - Soccer 2023
    by Esteban Martinez & GPT-4

    AMAZINGLY UNSTABLE VERSION
    (Coming soon: Unfathomably unstable version)

    #1 PRIORITY: FIX I2C COMMS
    I am utterly and completely cluelees as to what tf is going on
    Comp works well MOST of the time with ONE connector
    IR doesn't wotk MOST of the time with the OTHER connector
    Emphasis on MOST, because there is absolutely no indication as to when it changes
    These are probably coincidences, fact is: I2C doesnt work now (it never has as far as i know)

    US will be ready when they're ready
    Still unreliable af with new library, no surprise there
*/

#include "defs.h"

//---------------Hardware definitions---------------

//Motor 1 - Back Left
#define EN_1 33
#define PWM_A1 4
#define PWM_B1 2
//Motor 2 - Front Left
#define EN_2 32
#define PWM_A2 8
#define PWM_B2 6
//Motor 3 - Front Right
#define EN_3 25
#define PWM_A3 7
#define PWM_B3 9
//Motor 4 - Back Right
#define EN_4 23
#define PWM_A4 3
#define PWM_B4 5

//A: Outer, B: Inner
//Light 1 - Back
#define LUZ_A1 A5
#define LUZ_B1 A1
#define LIM_A1 800
#define LIM_B1 800
//Light 2 - Left
#define LUZ_A2 A0
#define LUZ_B2 A2
#define LIM_A2 600
#define LIM_B2 600
//Light 3 - Front
#define LUZ_A3 A3
#define LUZ_B3 A8
#define LIM_A3 600
#define LIM_B3 600
//Light 4 - Right
#define LUZ_A4 A4
#define LUZ_B4 A6
#define LIM_A4 800
#define LIM_B4 800

//Ultrasonic 1 - Left
#define US_E1 34
#define US_T1 36
//Ultrasonic 2 - Right
#define US_E2 29
#define US_T2 27

//Compass address, message and limit
#define C1 0x01
#define C2 0x44
#define C_LIM 5

//IR Seeker address and message
#define IR1 0x10 / 2
#define IR2 0x49
#define IR_LIM 30
#define IR_CORR 45

//Motor powers
#define POW1 80
#define POW2 80
#define POW3 80
#define POW4 80

//Movement settings
#define DEF NaN
#define BRAKE 200
#define FACTOR 3.0
#define KPI 30
#define KPF 100
#define DEL 30

//Helper variables
const int robotId = 0; 
bool led = true;

//---------------Hardware classes declarations---------------

//Motor declarations
Motor M1(1, EN_1, PWM_A1, PWM_B1, POW1);
Motor M2(2, EN_2, PWM_A2, PWM_B2, POW2);
Motor M3(3, EN_3, PWM_A3, PWM_B3, POW3);
Motor M4(4, EN_4, PWM_A4, PWM_B4, POW4);

//Light sensor declarations
//Last argument changes the limit's source (0: setLim(), 1: EEPROM)
bool arg1 = false;
Light L1(1, LUZ_A1, LUZ_B1, arg1);
Light L2(2, LUZ_A2, LUZ_B2, arg1);
Light L3(3, LUZ_A3, LUZ_B3, arg1);
Light L4(4, LUZ_A4, LUZ_B4, arg1);

//Ultrasonic sensor declarations
//Timeout argument changes the max distance (cm)
ul timeout = 300;
US U1(1, US_T1, US_E1, timeout);
US U2(2, US_T2, US_E2, timeout);

//Compass sensor declarations
//Last argument sets limit for north in degrees (same for left and right)
Compass Comp(1, C1, C2, C_LIM);

//IR Seeker declarations
IRSeeker IR(1, IR1, IR2);

//---------------Main code---------------

void setup(){
    globalInit(2);
    Serial.println("3 pesos");
}

void loop(){
    double angle = IR.read(1);
        if(angle == double(NaN)) followPath(1.0);
        else{
            if(angle > 60.0) angle += 30.0;
            else if(angle < -60.0) angle -= 30.0;
            followPath(degToDec(angle));
        }
    delayMicroseconds(500);
}
