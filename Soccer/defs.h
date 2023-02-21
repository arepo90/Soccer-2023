#ifndef defs_h
#define defs_h
#include "Arduino.h"

//Motor class
class Motor{
    private:
        int EN;
        int PWM_A;
        int PWM_B;
        int id;
    public:
        Motor(int id, int EN, int PWM_A, int PWM_B);
        void move(int pow);
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
        Light(int id, int PIN_A, int PIN_B);
        void setLim(int LIM_A, int LIM_B);
        void debug();
        int read();
};

//Ultrasonic sensors class
class Ultrasonic{
    private:
        int TRIG;
        int ECHO;
        int id;
        unsigned long timeOut;
        unsigned long prevMicros;
    public:
        Ultrasonic(int id, int TRIG, int ECHO, unsigned long timeOut = 20000UL);
        void debug();
        int read();
};

/*Compass sensor class - Not implemented yet
class Compass{
    private:

}
Missing: IR Seeker class & functions
*/

#endif