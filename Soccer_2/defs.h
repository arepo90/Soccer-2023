/*
    Libraries, hardware classes and general functions
*/

#ifndef defs_h
#define defs_h

//-----------Libraries and definitions-----------

#include "Arduino.h"
#include <EEPROM.h>
#include <Wire.h>
#include <NewPing.h>


typedef long long lli;
typedef long double ld;
typedef unsigned long ul;

#define NaN 420

//---------------General functions---------------

bool checkDelay(int time);
double degToDec(int x);
int decToDeg(double x);
void memSave(int n, int target);
int memRead(int target);

//---------------Hardware classes---------------

//Motor class
class Motor{
    private:
        int PWM;
        int DIR_A;
        int DIR_B;
        int POW = 0;
        int defPow;
        int id;
    public:
        Motor(int id, int PWM, int DIR_A, int DIR_B, int defPow);
        int getPow();
        void move(int Pow);
        void update(int Pow);
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
        int DIS_LIM;
        int past = 0;
        int id;
        NewPing* sonar;
    public:
        US(int id, int TRIG, int ECHO, int DIS_LIM);
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
        double read(int mode);
        bool north();
        int getOffset();
        void setOffset(int angle);
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
        int read(int mode);
        void init();
        void debug();
};

#endif
