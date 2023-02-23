/*
    Main code v0.4 - 23/02/2023 - Soccer 2023
    Esteban Martinez

    TODO: Compass, IR Seeker, Specific functions (Game Plan)
*/

#include <EEPROM.h>
#include <Wire.h>
#include "defs.h"

//PuenteH 1 - Atras
#define EN_1 33
#define PWM_A1 2
#define PWM_B1 4
//PuenteH 2 - Izquierda
#define EN_2 32
#define PWM_A2 6
#define PWM_B2 8
//PuenteH 3 - Adelante
#define EN_3 25
#define PWM_A3 7
#define PWM_B3 9
//PuenteH 4 - Derecha
#define EN_4 23
#define PWM_A4 3
#define PWM_B4 5

//Luz 1 - Atras
#define LUZ_A1 A3 //Solo este esta bien
#define LUZ_B1 A4
//Luz 2 - Izquierda
#define LUZ_A2 A5
#define LUZ_B2 A6
//Luz 3 - Adelante
#define LUZ_A3 A7
#define LUZ_B3 A8
//Luz 4 - Derecha
#define LUZ_A4 A9
#define LUZ_B4 A10

//Ultrasonico 1 - Izquierda
#define US_E1 34
#define US_T1 36
//Ultrasonico 2 - Derecha
#define US_E2 29
#define US_T2 27

#define POWER 80

//HiTechnic Compass Sensor
const int compAddress = 0x01;

Motor M1(0, EN_1, PWM_A1, PWM_B1);
Motor M2(1, EN_2, PWM_A2, PWM_B2);
Motor M3(2, EN_3, PWM_A3, PWM_B3);
Motor M4(3, EN_4, PWM_A4, PWM_B4);

Light L1(0, LUZ_A1, LUZ_B1);
Light L2(1, LUZ_A2, LUZ_B2);
Light L3(2, LUZ_A3, LUZ_B3);
Light L4(3, LUZ_A4, LUZ_B4);

Ultrasonic U1(0, US_T1, US_E1);
Ultrasonic U2(1, US_T2, US_E2);

void setup(){
    Serial.begin(9600);
}

void loop(){

}
