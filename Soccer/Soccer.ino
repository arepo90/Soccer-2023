#include <EEPROM.h>
//El 1 es el del arduino y va en sentido horario
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

//Ultrasonico 1 - Izquierda
#define US_E1 34
#define US_T1 36
//Ultrasonico 2 - Derecha
#define US_E2 29
#define US_T2 27


#define POWER 80;

void setup() {
    Motor M1(EN_1, PWM_A1, PWM_B1);
    Motor M2(EN_2, PWM_A2, PWM_B2);
    Motor M3(EN_3, PWM_A3, PWM_B3);
    Motor M4(EN_4, PWM_A4, PWM_B4);
}

void loop() {
    movimientos();
}
