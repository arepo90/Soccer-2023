/*
    Main code v0.6 - 25/02/2023 - Soccer 2023
    Esteban Martinez

    TODO: Specific functions (Game Plan)
*/


#include "defs.h"
#include <EEPROM.h>
#include <Wire.h>


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

//---------- AQUI ---------- AQUI ---------- AQUI ----------
//Luz 1 - Atras
#define LUZ_A1 A3 //Los pines estan puestos asi al azar, no son los de verdad
#define LUZ_B1 A4
#define LIM_A1 0 
#define LIM_B1 0
//Luz 2 - Izquierda
#define LUZ_A2 A5
#define LUZ_B2 A6
#define LIM_A2 0 
#define LIM_B2 0
//Luz 3 - Adelante
#define LUZ_A3 A7
#define LUZ_B3 A8
#define LIM_A3 0 
#define LIM_B3 0
//Luz 4 - Derecha
#define LUZ_A4 A9
#define LUZ_B4 A10
#define LIM_A4 0 
#define LIM_B4 0

//Ultrasonico 1 - Izquierda
#define US_E1 34
#define US_T1 36
//Ultrasonico 2 - Derecha
#define US_E2 29
#define US_T2 27

//Compass address and message
#define C1 0x01
#define C2 0x44

//IR Seeker address and message
#define IR1 0x10 / 2
#define IR2 0x49

//Universal motor power
#define POWER 80


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

Compass Comp(0, C1, C2);

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
/*
    Todo ya esta inicializado, solo es necesario cambiar los #defines donde dice AQUI
    mas arriba para los pines de los sensores de luz.
    La funcion de abajo va a sacar por cada sensor un numero raro (ahorita no le hagan caso)
    y luego los valores analogicos de los dos sensores, A: Afuera, B: Adentro.

    O si quieren checar primero uno por uno, pues a puro Serial.print(analogRead()), ahi como
    vean mejor
*/

    lightDebug();
    Serial.println();


//---------- Ignorar por ahora ----------
    //gp();

    //globalDebug();
    //memDebug();
}
