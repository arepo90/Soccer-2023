/*
    Main code v0.15 - 20/03/2023 - Soccer 2023
    Esteban Martinez

    TODO: Conitue testing motor functions (Global 50 power works)
*/


#include "defs.h"


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
#define LIM_A1 845 
#define LIM_B1 709
//Light 2 - Left
#define LUZ_A2 A0
#define LUZ_B2 A2
#define LIM_A2 636 
#define LIM_B2 575
//Light 3 - Front
#define LUZ_A3 A3
#define LUZ_B3 A8
#define LIM_A3 675 
#define LIM_B3 680
//Light 4 - Right
#define LUZ_A4 A4
#define LUZ_B4 A6
#define LIM_A4 862
#define LIM_B4 855

//Ultrasonic 1 - Left
#define US_E1 34
#define US_T1 36
//Ultrasonic 2 - Right
#define US_E2 29
#define US_T2 27

//Compass address, message and limit
#define C1 0x01
#define C2 0x44
#define C_LIM 12

//IR Seeker address and message
#define IR1 0x10 / 2
#define IR2 0x49

//Motor powers
#define POW1 50
#define POW2 50
#define POW3 50
#define POW4 50

//Movement settings
#define DEF NaN
#define BRAKE 150
#define KPI 30
#define KPF 120

const int robotId = 0; 

//Hardware declarations
Motor M1(0, EN_1, PWM_A1, PWM_B1, POW1);
Motor M2(1, EN_2, PWM_A2, PWM_B2, POW2);
Motor M3(2, EN_3, PWM_A3, PWM_B3, POW3);
Motor M4(3, EN_4, PWM_A4, PWM_B4, POW4);

//Last argument changes the limit's source (0: setLim(), 1: EEPROM)
bool arg1 = 0;
Light L1(0, LUZ_A1, LUZ_B1, arg1);
Light L2(1, LUZ_A2, LUZ_B2, arg1);
Light L3(2, LUZ_A3, LUZ_B3, arg1);
Light L4(3, LUZ_A4, LUZ_B4, arg1);

//Optional last argument sets the timeout (range and delay change proportionally)
bool arg2 = 20000UL;
US U1(0, US_T1, US_E1, arg2);
US U2(1, US_T2, US_E2, arg2);

Compass Comp(0, C1, C2, C_LIM);

IRSeeker IR(0, IR1, IR2);


//Main code
void setup(){
    globalInit(0);
    if(!arg1){
        L1.setLim(LIM_A1, LIM_B1);
        L2.setLim(LIM_A2, LIM_B2);
        L3.setLim(LIM_A3, LIM_B3);
        L4.setLim(LIM_A4, LIM_B4);
    }
    Serial.println("3 pesos");
}

void loop(){
    movements();
}
