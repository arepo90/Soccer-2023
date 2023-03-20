/*
    Definitions for hardware classes
*/

#ifndef defs_h
#define defs_h
#include "Arduino.h"
#include <EEPROM.h>
#include <Wire.h>
#include <Ultrasonic.h>
#include <digitalWriteFast.h>

#define NaN 420

void memSave(int n, int target);
int memRead(int target);

//Motor class
class Motor{
    private:
        int EN;
        int PWM_A;
        int PWM_B;
        int POW;
        int defPow;
        int id;
    public:
        Motor(int id, int EN, int PWM_A, int PWM_B, int defPow);
        void move(int POW);
        void brake(int force);
        void test();
        void debug();
};

//Light sensors class
class Light{
    private:
        int PIN_A;
        int PIN_B;
        int LIM_A;
        int LIM_B;
        int id;
    public:
        Light(int id, int PIN_A, int PIN_B, bool check);
        int read();
        void setLim(int LIM_A, int LIM_B);
        void debug();
};

//Ultrasonic sensors class
class US{
    private:
        int TRIG;
        int ECHO;
        unsigned long timeOut;
        bool arg;
        int past = 0;
        int id;
        Ultrasonic* us_fake;
    public:
        US(int id, int TRIG, int ECHO, bool arg = false, unsigned long timeOut = 20000UL);
        int read();
        void debug();
};

//Compas sensor class
class Compass{
    private:
        int ADDRESS;
        int MSG;
        int OFFSET = 0;
        int LIM;
        int id;
    public:
        Compass(int id, int C1, int C2, int LIM);
        double read(int state);
        bool north();
        void init();
        void debug();
};

//IR Seeker sensor class
class IRSeeker{
    private:
        int ADDRESS;
        int MSG;
        int id;
    public:
        IRSeeker(int id, int IR1, int IR2);
        int read();
        void init();
        void debug();
};

#endif
