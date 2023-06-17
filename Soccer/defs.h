/*
    Libraries, hardware classes and general functions
*/

#ifndef defs_h
#define defs_h

//General behaviour (0: master, 1: slave)
#define ROBOT_ID 0

//-----------Libraries and definitions-----------

#include "Arduino.h"
#include <Wire.h>
#include <NewPing.h>
#include <esp_now.h>
#include <WiFi.h>

typedef long long lli;
typedef long double ld;
typedef unsigned long ul;
typedef String S;

#define NaN 420

//PWM settings
#define FREQ 5000
#define RES 8

//IR settings
#define SAMPLES 500

struct WLmsg{
    int id;
};

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
        int PWM_A;
        int PWM_B;
        int POW = 0;
        int defPow;
        int id;
    public:
        Motor(int id, int PWM_A, int PWM_B, int defPow);
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
        Light(int id, int PIN_A, int PIN_B, int LIM_A, int LIM_B);
        int read();
        void debug();
};


//Ultrasonic sensors class
class US{
    private:
        int TRIG;
        int ECHO;
        int DIS_LIM;
        bool arg;
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
        int REQ;
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

class Infrared{
    private:
        int PINS[16];
        int Mux0;
        int Mux1;
        int Mux2;
        int Mux3;
        int Mux_IN;
        int CERTAINTY = -1;
        int id;
    public:
        Infrared(int id, int pins[], int Mux0, int Mux1, int Mux2, int Mux3, int Mux_IN);
        void setMux(int n);
        double read(int method, int mode);
        void debug();
};

#if ROBOT_ID == 0

class I2C{
    private:
        int id;
        int ADDRESS;
        int MSG_LENGTH;
        int IR_STATE;
        int COMP_STATE;
        int US1_STATE;
        int US2_STATE;
    public:
        I2C(int id, int ADDRESS, int MSG_LENGTH);
        void init();
        void parseMsg(String str);
        int requestMsg();
        double read(String str, int mode);
        void debug();
};

class Wireless{
    private:
        int id;
        uint8_t ADDRESS[6];
        esp_now_peer_info_t ESP;
    public:
        Wireless(int id, uint8_t *ADDRESS);
        void init();
        static void sendCB(const uint8_t *ADDRESS, esp_now_send_status_t status);
        static void receiveMsg(const uint8_t *ADDRESS, const uint8_t *package, int n);
        void send();
        WLmsg read();
        void debug();
};

#else

class I2C{
    private:
        int id;
        int ADDRESS;
        int MSG_LENGTH;
    public:
        I2C(int id, int ADDRESS, int MSG_LENGTH);
        void init();
        void sendMsg(int IR, int COMP, int U1, int U2);
        void receiveMsg();
        void debug();
};

#endif

#endif