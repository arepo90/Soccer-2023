/*
    Source code for hardware classes and general functions-
*/

#include "Arduino.h"
#include "defs.h"
#include <EEPROM.h>
#include <Wire.h>

//Save 4-byte number to EEPROM at target slots (0-indexed)
void memSave(int n, int target){
    for(int i = 4*target; i < 4*(target+1); i++){
        EEPROM.update(i, (n >= 255 ? 255 : n));
        if(n >= 255) n -= 255;
        else n = 0;
    }
}

//Read 4-byte number from EEPROM target slots (0-indexed)
int memRead(int target){
    int ans = 0;
    for(int i = 4*target; i < 4*(target+1); i++){
        ans += EEPROM.read(i);
    }
    return ans;
}

//Setup and pin declaration
Motor::Motor(int id, int EN, int PWM_A, int PWM_B){
    this->id = id;
    this->EN = EN;
    this->PWM_A = PWM_A;
    this->PWM_B = PWM_B;
    pinMode(EN, OUTPUT);
    pinMode(PWM_A, OUTPUT);
    pinMode(PWM_B, OUTPUT);
}

//Set motor power (-255 -> 255)
void Motor::move(int POW){
    this->POW = POW;
    digitalWrite(EN, HIGH);
    analogWrite(PWM_A, (POW < 0 ? 0 : POW));
    analogWrite(PWM_B, (POW < 0 ? POW : 0));
}

void Motor::debug(){
    Serial.print(" M");
    Serial.print(id);
    Serial.print(": ");
    Serial.print(POW);
}

//Setup and pin declaration
Light::Light(int id, int PIN_A, int PIN_B, bool check){
    this->id = id;
    this->PIN_A = PIN_A;
    this->PIN_B = PIN_B;
    if(check){
        LIM_A = memRead(id*2);
        LIM_B = memRead((id*2)+1);
    }
    pinMode(PIN_A, INPUT);
    pinMode(PIN_B, INPUT);
}

//Read sensor block state (0: None, 1: Outer, 2: Inner, 3: Both)
int Light::read(){
    bool temp1 = false, temp2 = false;
    if(analogRead(PIN_A) <= LIM_A) temp1 = true;
    if(analogRead(PIN_B) <= LIM_B) temp2 = true;
    if(temp1 && temp2) return 3;
    else if(temp2) return 2;
    else if(temp1) return 1;
    else return 0;
}

//Set threshold for white/green detection (0 -> 1024)
void Light::setLim(int LIM_A, int LIM_B){
    this->LIM_A = LIM_A;
    this->LIM_B = LIM_B;
    memSave(LIM_A, 2*id);
    memSave(LIM_B, (2*id)+1);
}

//Debug info in serial monitor
void Light::debug(){
    Serial.print(" Light");
    Serial.print(id);
    Serial.print(": ");
    Serial.print(this->read());
    Serial.print(" L_A: ");
    Serial.print(analogRead(PIN_A));
    Serial.print(" L_B: ");
    Serial.print(analogRead(PIN_B));
}

//Setup and pin declaration
Ultrasonic::Ultrasonic(int id, int TRIG, int ECHO, unsigned long timeOut){
    this->id = id;
    this->TRIG = TRIG;
    this->ECHO = ECHO;
    this->timeOut = timeOut;
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
}

//Distance read (cm)
int Ultrasonic::read(){
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    delay(30);
    prevMicros = micros();
    while(!digitalRead(ECHO) && (micros() - prevMicros) <= timeOut);
    prevMicros = micros();
    while(digitalRead(ECHO)  && (micros() - prevMicros) <= timeOut);
    return (micros()-prevMicros) / 56;
}

//Debug info in serial monitor
void Ultrasonic::debug(){
    Serial.print(" US");
    Serial.print(id);
    Serial.print(": ");
    Serial.print(this->read());
}

Compass::Compass(int id, int C1, int C2){
    this->id = id;
    ADDRESS = C1;
    MSG = C2;
    Wire.begin();
    Wire.beginTransmission(ADDRESS);
    Wire.write(0x00);
    Wire.endTransmission();
    while(Wire.available() > 0){
        Wire.read();
    }

    byte buffer[2];
    Wire.beginTransmission(ADDRESS);
    Wire.write(MSG);
    Wire.endTransmission();
    Wire.requestFrom(ADDRESS, 2); 
    while(Wire.available() < 2);
    for(byte i = 0; i < 2; i++){
        buffer[i] = Wire.read();
    }
    OFFSET = word(buffer[0], buffer[1]);
    if(OFFSET < 0) OFFSET += 360;
}

int Compass::read(){
    byte buffer[2];
    Wire.beginTransmission(ADDRESS);
    Wire.write(MSG);
    Wire.endTransmission();
    Wire.requestFrom(ADDRESS, 2); 
    while(Wire.available() < 2);
    for(byte i = 0; i < 2; i++){
        buffer[i] = Wire.read();
    }
    int angle = word(buffer[0], buffer[1]);
    if(angle < 0) angle += 360;
    angle -= OFFSET;
    if(angle < 0) angle += 360;
    return angle;
}

void Compass::debug(){
    Serial.print(" COMPASS: ");
    Serial.print(this->read());
}

IRSeeker::IRSeeker(int id, int IR1, int IR2){
    this->id = id;
    ADDRESS = IR1;
    MSG = IR2;
    Wire.begin();
    Wire.beginTransmission(ADDRESS);
    Wire.write(0x00);
    Wire.endTransmission();
    while(Wire.available() > 0){
        Wire.read();
    }
}

int IRSeeker::read(){
    byte buffer[6];
    Wire.beginTransmission(ADDRESS);
    Wire.write(MSG);
    Wire.endTransmission();
    Wire.requestFrom(ADDRESS, 6);
    while(Wire.available() < 6);
    for(byte i = 0; i < 6; i++){
        buffer[i] = Wire.read();
    }
    while(Wire.available() > 0){
        Wire.read();
    }
    return buffer[0];
}

void IRSeeker::debug(){
    Serial.print(" IR: ");
    Serial.print(this->read());
}
