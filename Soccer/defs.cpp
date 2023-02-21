#include "Arduino.h"
#include "defs.h"

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
void Motor::move(int pow){
    digitalWrite(EN, HIGH);
    analogWrite(PWM_A, (pow < 0 ? 0 : pow));
    analogWrite(PWM_B, (pow < 0 ? pow : 0));
}

//Setup and pin declaration
Light::Light(int id, int PIN_A, int PIN_B){
    this->id = id;
    this->PIN_A = PIN_A;
    this->PIN_B = PIN_B;
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
}

//Debug info in serial monitor
void Light::debug(){
    Serial.print(" Light");
    Serial.print(id)
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
