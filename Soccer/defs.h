/*
    Headers for libraries, hardware classes and general functions
*/

#ifndef defs_h
#define defs_h

//General behaviour (0: master, 1: slave)
#define ESP_ID 1
//Game functions (0: attacker, 1: defender)
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

struct WLmsg{
    int id;
};

//---------------General functions---------------

double degToDec(int x);
int decToDeg(double x);

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
        int id;
        int PINS[12];
        int LIMS[12];
        double MAGNITUDE = 0.0;
    public:
        Light(int id, int *PINS, int *LIMS);
        double read(int mode);
        void debug();
};


//Ultrasonic sensors class
class Ultrasonico{
    private:
        int TRIG;
        int ECHO;
        int DIS_LIM;
        int past = 0;
        int id;
        NewPing* sonar;
    public:
        Ultrasonico(int id, int TRIG, int ECHO, int DIS_LIM);
        int read();
        void debug();
};

//Compass sensor class
class Compass{
    private:
        int id;
        int C_ADDRESS;
        int C_MSG;
        int C_LIM;
        int OFFSET = 0;
    public:
        Compass(int id, int C_ADDRESS, int C_MSG, int LIM);
        double read(int mode);
        bool north();
        int getOffset();
        void setOffset(int angle);
        void init();
        void debug();
};

//Custom infrared sensor class
class Infrared{
    private:
        int id;
        int PINS[16];
        int Mux0;
        int Mux1;
        int Mux2;
        int Mux3;
        int Mux_IN;
        int SAMPLES;
        int DELAY;
        int CERTAINTY;
    public:
        Infrared(int id, int *PINS, int Mux0, int Mux1, int Mux2, int Mux3, int Mux_IN, int SAMPLES, int DELAY);
        void setMux(int n);
        double read(int mode);
        void debug();
};

#if ESP_ID == 0

//Master I2C communications class
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
        void requestMsg();
        double read(String str, int mode);
        void getInfo(int &IR, int &COMP, int &US1, int &US2);
        void debug();
};

void WLsendCB(const uint8_t *ADDRESS, esp_now_send_status_t status);
void WLreceiveMsg(const uint8_t *ADDRESS, const uint8_t *package, int n);

//ESP-NOW wireless communications class
class Wireless{
    private:
        int id;
        uint8_t ADDRESS[6];
        esp_now_peer_info_t ESP;
    public:
        Wireless(int id, uint8_t *ADDRESS);
        void init();
        void send();
        WLmsg read();
        void debug();
};

#else

//Slave I2C communications class
class I2C{
    private:
        int id;
        int ADDRESS;
        int MSG_LENGTH;
    public:
        I2C(int id, int ADDRESS, int MSG_LENGTH);
        void init();
        void update(int I2CIR, int I2CCOMP, int I2CUS1, int I2CUS2);
        static void sendMsg();
        void debug();
};

#endif

#endif
