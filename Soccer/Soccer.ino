/*
    Main code v2.0 alpha 2 - 20/06/2023 - Soccer 2023
    by Esteban Martinez & GPT-4

    RoboCup version
    (nuh uh forget this) Unstable, very likely to break or crash
    not working, wip, fix light and vector handling in gp.ino

    *NEW*
    IR SENSOR WORKS LETS GOOOOO (Should test with different parameters)
    I2C COMMS WORK WITH CLASSES LETS GOOOOO
    DUAL CORE WORKS LETS GOOOOO (I think, I didn't test it a lot, will 
    prob need delays or something to stop watchdog or whatever reset it)
    Robot behaviour is now modified through #define's in defs.h

    *TODO*
    Update US functions and check with TMR version
    Rewrite gp and helper functions
    Test dual core functionality and scheme what goes where and when
*/

#include "defs.h"
TaskHandle_t primary, secondary;

//---------------Hardware definitions---------------

//Joinmax motor pins
//Motor 1 - Back Left
#define PWM_A1 12
#define PWM_B1 14
//Motor 2 - Front Left
#define PWM_A2 27
#define PWM_B2 26
//Motor 3 - Front Right
#define PWM_A3 19
#define PWM_B3 18
//Motor 4 - Back Right
#define PWM_A4 5
#define PWM_B4 17

//Light sensor pins and limits
//Order starts at front and increases clockwise
int LIGHT_PINS[] = {15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15},
    LIGHT_LIMS[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//Ulrtasonic pins and limits
//Ultrasonic 1 - Left
#define US_E1 12
#define US_T1 14
//Ultrasonic 2 - Right
#define US_E2 4
#define US_T2 0
//Distance thresholds (cm)
#define US_LIM 400
#define MIN_DIS 20
#define MAX_DIS 60

//Compass address, message and limit
#define C_ADDRESS 0x01
#define C_MSG 0x44
#define C_LIM 5

//IR pins and detection
//Multiplexer pins
#define IR0 27
#define IR1 26
#define IR2 25
#define IR3 33
#define IR_IN 32
int IR_PINS[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15, 14, 13, 1, 0};
//Detection settings
#define IR_SAMPLES 500
#define IR_DELAY 5

//I2C Slave address for ESP <-> ESP communications
#define COMMS_ADDRESS 9

//Motor max powers
#define POW1 60
#define POW2 60
#define POW3 60
#define POW4 60

//Movement settings
#define THRESHOLD 0.0
#define DEF NaN
#define BRAKE 200
#define FACTOR 3.0
#define KPI 30
#define KPF 100
#define DEL 200

//Wireless comms address
uint8_t WL_ADDRESS[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//Helper variables
double IR_ANGLE, COMP_ANGLE, US1_DIS, US2_DIS, LINE_ANGLE, LINE_MAG;
double VECTOR_ANGLE, VECTOR_MAG;

//---------------Hardware classes declarations---------------

I2C Comms(0, COMMS_ADDRESS, 21);

#if ESP_ID == 0

//Motor declarations
Motor M1(0, PWM_A1, PWM_B1, POW1);
Motor M2(1, PWM_A2, PWM_B2, POW2);
Motor M3(2, PWM_A3, PWM_B3, POW3);
Motor M4(3, PWM_A4, PWM_B4, POW4);

//Light sensor declarations
//Last argument changes the limit's source (0: setLim(), 1: EEPROM)
Light LS(1, LIGHT_PINS, LIGHT_LIMS);

Wireless WL(1, WL_ADDRESS);

#else

//Ultrasonic sensor declarations
//Limit argument changes the max distance (cm)
Ultrasonico U1(1, US_T1, US_E1, US_LIM);
Ultrasonico U2(2, US_T2, US_E2, US_LIM);

//Compass sensor declarations
//Limit argument sets north threshold (symmetrical, in degrees)
Compass Comp(1, C_ADDRESS, C_MSG, C_LIM);

//IR Seeker declarations
Infrared IR(1, IR_PINS, IR0, IR1, IR2, IR3, IR_IN, IR_SAMPLES, IR_DELAY);

#endif

//---------------Main code---------------

int a = 0, b = 0, c = 0, d = 0;

void primary_code(void *pvParameters){
    while(1){
        gp();
    }
}

void secondary_code(void *pvParameters){
    while(1){
        a += 1;
        b += 2;
        c += 3;
        d += 4;
        a = a % 999;
        b = b % 999;
        c = c % 999;
        d = d % 999;
        Serial.println(S(a) + " " + S(b) + " " + S(c) + " " + S(d));
    }
}

void setup(){
    globalInit(1);
    xTaskCreatePinnedToCore(primary_code, "primary", 10000, NULL, 1, &primary, 0);
    xTaskCreatePinnedToCore(secondary_code, "secondary", 10000, NULL, 1, &secondary, 1);
    Serial.println("4 pesos");
}

void loop(){
    vTaskDelete(NULL);
}
