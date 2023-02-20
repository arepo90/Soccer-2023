#include <EEPROM.h>

//Puente H1 - Atras
#define EN_1 33
#define PWM_A1 2
#define PWM_B1 4
//Puente H2 - Izquierda
#define EN_2 32
#define PWM_A2 6
#define PWM_B2 8
//Puente H3 - Adelante
#define EN_3 25
#define PWM_A3 7
#define PWM_B3 9
//Puente H4 - Derecha
#define EN_4 23
#define PWM_A4 3
#define PWM_B4 5

//Luz 1 - Atras
#define LUZ_A1 A3
#define LUZ_B1
//Luz 2 - Izquierda
#define LUZ_A2 
#define LUZ_B2 
//Luz 3 - Adelante
#define LUZ_A3 
#define LUZ_B3 
//Luz 4 - Derecha
#define LUZ_A4 
#define LUZ_B4  

//Ultrasonico 1 - Izquierda
#define US_E1 34
#define US_T1 36
//Ultrasonico 2 - Derecha
#define US_E2 29
#define US_T2 27

#define POWER 80



/*Ultrasonic US_1(US_T1, US_E1);
Ultrasonic US_2(US_T2, US_E2);*/

/*Light L1(LUZ_A1, LUZ_B1);
Light L2(LUZ_A2, LUZ_B2);
Light L3(LUZ_A3, LUZ_B3);
Light L4(LUZ_A4, LUZ_B4);*/

//Las declaraciones de clases ocupan ir antes del setup

void setup(){
    Serial.begin(9600);

    Motor M1(EN_1, PWM_A1, PWM_B1);
    Motor M2(EN_2, PWM_A2, PWM_B2);
    Motor M3(EN_3, PWM_A3, PWM_B3);
    Motor M4(EN_4, PWM_A4, PWM_B4);
}

void loop(){
    digitalWrite(EN_3, HIGH);
    analogWrite(PWM_A3, 50);
    analogWrite(PWM_B3, 0);
    delay(1000);
    digitalWrite(EN_3, HIGH);
    analogWrite(PWM_A3, 0);
    analogWrite(PWM_B3, 50);
    delay(1000);
}