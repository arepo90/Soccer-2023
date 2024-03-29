/*
    Source code for hardware classes and general functions
*/

#include "defs.h"

//---------------General functions---------------

//Degree to decimal angle [0, 360] -> [-1, 1]
double degToDec(int x){
    if(x == NaN) return double(NaN);
    if(x == 180) return 1.0;
    return ( ((-fabs(double(x)-180.0)) / (double(x)-180.0)) - 1.0 + double(x)/180.0 );
}

//Decimal angle to degree [-1, 1] -> [0, 360]
int decToDeg(double x){
    if(x == double(NaN)) return NaN;
    if(x == 0.0) return 0;
    return ( 180.0 * (x + 1.0 - fabs(x)/x) );
}

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

//---------------Hardware functions---------------

//Motor setup and pin declaration
Motor::Motor(int id, int PWM, int DIR_A, int DIR_B, int defPow){
    this->id = id;
    this->PWM = PWM;
    this->DIR_A = DIR_A;
    this->DIR_B = DIR_B;
    this->defPow = defPow;
    pinMode(PWM, OUTPUT);
    pinMode(DIR_A, OUTPUT);
    pinMode(DIR_B, OUTPUT);
}

//Motor power getter function
int Motor::getPow(){
    return POW;
}

//Set motor power (-255 -> 255)
void Motor::move(int Pow){
    if(abs(Pow) == NaN) POW = (Pow > 0 ? defPow : -defPow);
    else if(abs(Pow) > 255) POW = (Pow > 0 ? 255 : -255);
    else POW = Pow;
    analogWrite(PWM, abs(POW));
    digitalWrite(DIR_A, (POW > 0 ? LOW : HIGH));
    digitalWrite(DIR_B, (POW > 0 ? HIGH : LOW));
}

//Update motor power (increases or decreases current power)
void Motor::update(int Pow){
    POW += Pow;
    if(POW > 255) POW = 255;
    else if(POW < -255) POW = -255;
    this->move(POW);
}

//Motor stop (0 -> 255)
void Motor::brake(int force){
    analogWrite(PWM, force);
    digitalWrite(DIR_A, HIGH);
    digitalWrite(DIR_B, HIGH);
}

//Test motor back and forth
void Motor::test(){
    this->move(defPow);
    delay(500);
    this->move(-defPow);
    delay(500);
}

//Debug info in serial monitor
void Motor::debug(){
    Serial.print(" M");
    Serial.print(id);
    Serial.print(": ");
    Serial.print(POW);
}

//Light sensor setup and pin declaration
Light::Light(int id, int PIN_A, int PIN_B, bool check){
    this->id = id;
    this->PIN_A = PIN_A;
    this->PIN_B = PIN_B;
    if(check){
        LIM_A = memRead(id*2);
        LIM_B = memRead((id*2)+1);
    }
}

//Read sensor block state (0: None, 1: Outer, 2: Both, 3: Inner)
int Light::read(){
    bool temp1 = false, temp2 = false;
    if(analogRead(PIN_A) >= LIM_A) temp1 = true;
    if(analogRead(PIN_B) >= LIM_B) temp2 = true;
    if(temp1 && temp2) return 2;
    else if(temp2) return 3;
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

//US sensor setup and pin declaration
US::US(int id, int TRIG, int ECHO, int DIS_LIM){
    this->id = id;
    this->TRIG = TRIG;
    this->ECHO = ECHO;
    this->DIS_LIM = DIS_LIM;
    sonar = new NewPing(TRIG, ECHO, DIS_LIM);
}

//Distance read (cm)
int US::read(){
    return sonar->ping_cm();
}

//Debug info in serial monitor
void US::debug(){
    Serial.print(" US");
    Serial.print(id);
    Serial.print(": ");
    Serial.print(this->read());
}

//Compass sensor setup and I2C Communication
Compass::Compass(int id, int C1, int C2, int LIM){
    this->id = id;
    this->LIM = LIM;
    ADDRESS = C1;
    MSG = C2;
}

//Angle read by mode (0: [0, 360], 1: [-1, 1])
double Compass::read(int mode){
    digitalWrite(13, HIGH);
    byte buffer[2];
    Wire.beginTransmission(ADDRESS);
    Wire.write(MSG);
    Wire.endTransmission();
    Wire.requestFrom(ADDRESS, 2); 
    buffer[0] = Wire.read();
    buffer[1] = Wire.read();
    digitalWrite(13, LOW);
    int angle = word(buffer[1], buffer[0]) - OFFSET;
    if(angle < 0) angle += 360;
    if(mode == 0) return double(angle);
    return degToDec(angle);
}

//Check if compass is pointing towards (fake) north
bool Compass::north(){
    int angle = this->read(0);
    if(angle < LIM || angle > 360-LIM) return true;
    else return false;
}

int Compass::getOffset(){
    return OFFSET;
}

void Compass::setOffset(int angle){
    OFFSET = angle;
}

//Initialize I2C communication
void Compass::init(){
    Serial.print("Compass initialization started");
    Wire.begin();
    Wire.beginTransmission(ADDRESS);
    Wire.write(0x00);
    Wire.endTransmission();
    while(Wire.available() > 0){
        Serial.print(".");
        Wire.read();
    }
    OFFSET = this->read(0);
    Serial.println();
    Serial.println("Compass initialization complete");
}

//Debug info in serial monitor
void Compass::debug(){
    Serial.print(" COMP DEG: ");
    Serial.print(this->read(0));
    Serial.print(" Y: ");
    Serial.print(this->read(1));
    Serial.print(" N? ");
    Serial.print(this->north());
    Serial.print(" OFF: ");
    Serial.print(OFFSET);
}

//IR Seeker setup and I2C Communication
IRSeeker::IRSeeker(int id, int IR1, int IR2){
    this->id = id;
    ADDRESS = IR1;
    MSG = IR2;
}

//Initialize I2C communication
void IRSeeker::init(){
    Serial.print("IR Seeker initialization started");
    Wire.begin();
    Wire.beginTransmission(ADDRESS);
    Wire.write(0x00);
    Wire.endTransmission();
    Serial.println();
    Serial.println("IR Seeker initialization complete");
}

//IR ball direction read by mode (0: [0, 9], 1: [-120, 120])
int IRSeeker::read(int mode){
    byte buffer;
    Wire.beginTransmission(ADDRESS);
    Wire.write(MSG);
    Wire.endTransmission();
    Wire.requestFrom(ADDRESS, 1);
    buffer = Wire.read();
    if(mode == 0) return (10 - buffer) % 10;
    if(buffer == 0) return NaN;
    return -(buffer * 30 - 150);
}

//Debug info in serial monitor
void IRSeeker::debug(){
    Serial.print(" IR: ");
    Serial.print(this->read(0));
    Serial.print(" Angle: ");
    Serial.print(this->read(1));
}
