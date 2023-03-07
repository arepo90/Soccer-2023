
/*
    Source code for hardware classes and general functions-
*/

#include "defs.h"

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
    analogWrite(PWM_A, (POW > 0 ? 0 : -POW));
    analogWrite(PWM_B, (POW > 0 ? POW : 0));
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
US::US(int id, int TRIG, int ECHO, bool arg, unsigned long timeOut){
    this->id = id;
    this->TRIG = TRIG;
    this->ECHO = ECHO;
    this->timeOut = timeOut;
    this->arg = arg;
    us_fake = new Ultrasonic(TRIG, ECHO, timeOut);
}

//Distance read (cm)
int US::leer(){
    int dist = us_fake->read();
    if(arg && dist == 357*(timeOut/20000UL)) return past;
    else past = dist;
    return dist;
}

//Debug info in serial monitor
void US::debug(){
    Serial.print(" US");
    Serial.print(id);
    Serial.print(": ");
    Serial.print(this->leer());
}

//Setup and I2C Communication
Compass::Compass(int id, int C1, int C2, int LIM){
    this->id = id;
    this->LIM = LIM;
    ADDRESS = C1;
    MSG = C2;
}


//Angle read by state (0: [0, 360], 1: [-1, 1])
double Compass::read(int state){
    byte buffer[2];
    Wire.beginTransmission(ADDRESS);
    Wire.write(MSG);
    Wire.endTransmission();
    Wire.requestFrom(ADDRESS, 2); 
    while(Wire.available() < 2);
    for(byte i = 0; i < 2; i++){
        buffer[i] = Wire.read();
    }
    int angle = word(buffer[1], buffer[0]) - OFFSET;
    if(angle < 0) angle += 360;
    if(state == 0) return double(angle);
    else{
        if(angle < 180) return double(angle) / 180.0;
        else return (double(angle)-360.0) / 180.0;
    }
    return word(buffer[1], buffer[0]);
}

void Compass::init(){
    Wire.begin();
    Wire.beginTransmission(ADDRESS);
    Wire.write(0x00);
    Wire.endTransmission();
    while(Wire.available() > 0){
        Wire.read();
    }
    OFFSET = this->read(0);
}

//Check if compass is pointing (fake) north
bool Compass::north(){
    int angle = this->read(0);
    if(angle < LIM || angle > 360-LIM) return true;
    else return false;
}

//Debug info in serial monitor
void Compass::debug(){
    Serial.print(" COMP DEG: ");
    Serial.print(this->read(0));
    Serial.print(" Y: ");
    Serial.print(this->read(1));
}


//Setup and I2C Communication
IRSeeker::IRSeeker(int id, int IR1, int IR2){
    this->id = id;
    ADDRESS = IR1;
    MSG = IR2;
}

void IRSeeker::init(){
    Wire.begin();
    Wire.beginTransmission(ADDRESS);
    Wire.write(0x00);
    Wire.endTransmission();
    while(Wire.available() > 0){
        Wire.read();
    }
}

//IR ball direction read (0 -> 9)
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

//Debug info in serial monitor
void IRSeeker::debug(){
    Serial.print(" IR: ");
    Serial.print(this->read());
}
