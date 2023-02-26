/*
    Main code v0.8 - 25/02/2023 - Soccer 2023
    Esteban Martinez

    TODO: Specific functions (Game Plan)
*/


#include "defs.h"
#include <EEPROM.h>
#include <Wire.h>


//Motor 1 - Backward
#define EN_1 33
#define PWM_A1 2
#define PWM_B1 4
//Motor 2 - Left
#define EN_2 32
#define PWM_A2 6
#define PWM_B2 8
//Motor 3 - Forward
#define EN_3 25
#define PWM_A3 7
#define PWM_B3 9
//Motor 4 - Right
#define EN_4 23
#define PWM_A4 3
#define PWM_B4 5

//A: Outer, B: Inner
//Light 1 - Backward
#define LUZ_A1 A0 //asi o al reves
#define LUZ_B1 A7
#define LIM_A1 0 
#define LIM_B1 0
//Light 2 - Left
#define LUZ_A2 A4
#define LUZ_B2 A6
#define LIM_A2 0 
#define LIM_B2 0
//Light 3 - Forward
#define LUZ_A3 A2
#define LUZ_B3 A1
#define LIM_A3 0 
#define LIM_B3 0
//Light 4 - Right
#define LUZ_A4 A5
#define LUZ_B4 A3
#define LIM_A4 0 
#define LIM_B4 0

//Ultrasonic 1 - Left
#define US_E1 34
#define US_T1 36
//Ultrasonic 2 - Right
#define US_E2 29
#define US_T2 27

//Compass address, message and limit
#define C1 0x01
#define C2 0x44
#define C_LIM 10

//IR Seeker address and message
#define IR1 0x10 / 2
#define IR2 0x49

//Motor settings
#define POWER 80
#define KPI 30
#define KPF 100


//Hardware declarations
Motor M1(0, EN_1, PWM_A1, PWM_B1);
Motor M2(1, EN_2, PWM_A2, PWM_B2);
Motor M3(2, EN_3, PWM_A3, PWM_B3);
Motor M4(3, EN_4, PWM_A4, PWM_B4);

//Last argument changes the limit's source (0: setLim(), 1: EEPROM)
bool arg = 0;
Light L1(0, LUZ_A1, LUZ_B1, arg);
Light L2(1, LUZ_A2, LUZ_B2, arg);
Light L3(2, LUZ_A3, LUZ_B3, arg);
Light L4(3, LUZ_A4, LUZ_B4, arg);

//Optional last argument sets the timeout (range and delay change proportionally)
Ultrasonic U1(0, US_T1, US_E1);
Ultrasonic U2(1, US_T2, US_E2);

Compass Comp(0, C1, C2, C_LIM);

IRSeeker IR(0, IR1, IR2);


//Main code
void setup(){
    Serial.begin(9600);
    Serial.print("Tacos de a ");

    if(arg == 0){
        L1.setLim(LIM_A1, LIM_B1);
        L2.setLim(LIM_A2, LIM_B2);
        L3.setLim(LIM_A3, LIM_B3);
        L4.setLim(LIM_A4, LIM_B4);
    }

    Serial.println("3 pesos");
}

void loop(){
    //gp();

    //globalDebug();
    //memDebug();
}
