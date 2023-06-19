/*
    Source code for hardware classes and general functions
*/

#include "defs.h"

WLmsg WLMSG;
int I2C_IR, I2C_COMP, I2C_US1, I2C_US2;

//---------------General functions---------------

//Degree to decimal angle [-1, 1]
double degToDec(int x){
    if(x == NaN) return double(NaN);
    if(x == 180) return 1.0;
    return ( ((-fabs(double(x)-180.0)) / (double(x)-180.0)) - 1.0 + double(x)/180.0 );
}

//Decimal angle to degree [0, 360]
int decToDeg(double x){
    if(x == 0.0) return 0;
    return ( 180.0 * (x + 1.0 - fabs(x)/x) );
}

//---------------Hardware functions---------------

//Motor setup and pin declaration
Motor::Motor(int id, int PWM_A, int PWM_B, int defPow){
    this->id = id;
    this->PWM_A = PWM_A;
    this->PWM_B = PWM_B;
    this->defPow = defPow;
    ledcSetup((id*2), FREQ, RES);
    ledcSetup((id*2+1), FREQ, RES);
    ledcAttachPin(PWM_A, (id*2));
    ledcAttachPin(PWM_B, (id*2+1));
}

//Set motor power (-255 -> 255)
void Motor::move(int Pow){
    if(abs(Pow) == NaN) POW = (Pow > 0 ? defPow : -defPow);
    else POW = Pow;
    ledcWrite((id*2), (POW > 0 ? 0 : -POW));
    ledcWrite((id*2+1), (POW > 0 ? POW : 0));
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
    if(POW == 0) return;
    this->move((POW > 0 ? -force : force));
    delay(2);
    this->move(0);
    delay(2);
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
Light::Light(int id, int *PINS, int *LIMS){
    this->id = id;
    memcpy(this->PINS, PINS, sizeof(this->PINS));
    memcpy(this->LIMS, LIMS, sizeof(this->LIMS));
    for(int i = 0; i < 12; i++){
        pinMode(PINS[i], INPUT);
    }
}

//Read sensor block state (0: [0, 360], 1: [-1, 1])
double Light::read(int mode){
    double angle = 0.0, cont = 0.0;
    for(int i = 0; i < 12; i++){
        if(analogRead(PINS[i]) > LIMS[i]){
            angle += 30.0 * double(i);
            cont++;
        }
    }
    if(cont == 0.0) return double(NaN);
    angle /= cont;
    if(mode == 0) return angle;
    return degToDec(angle);
}

//Debug info in serial monitor
void Light::debug(){
    Serial.print(" Light: ");
    for(int i = 0; i < 12; i++){
        Serial.print(" L");
        Serial.print(id);
        Serial.print(": ");
        Serial.print(analogRead(PINS[i]));
    }
    Serial.print(" ANGLE: ");
    Serial.println(this->read(0));
}

//US sensor setup and pin declaration
Ultrasonico::Ultrasonico(int id, int TRIG, int ECHO, int DIS_LIM){
    this->id = id;
    this->TRIG = TRIG;
    this->ECHO = ECHO;
    this->DIS_LIM = DIS_LIM;
    sonar = new NewPing(TRIG, ECHO, DIS_LIM);
}

//Distance read (cm)
int Ultrasonico::read(){
    return sonar->ping_cm();
}

//Debug info in serial monitor
void Ultrasonico::debug(){
    Serial.print(" US");
    Serial.print(id);
    Serial.print(": ");
    Serial.print(this->read());
}

//Compass sensor setup
Compass::Compass(int id, int C_ADDRESS, int C_MSG, int C_LIM){
    this->id = id;
    this->C_ADDRESS = C_ADDRESS;
    this->C_MSG = C_MSG;
    this->C_LIM = C_LIM;
}

//Initialise I2C communication
void Compass::init(){
    OFFSET = this->read(0);
}

//Angle read by mode (0: [0, 360], 1: [-1, 1])
double Compass::read(int mode){
    byte buffer[2];
    Wire.beginTransmission(C_ADDRESS);
    Wire.write(C_MSG);
    Wire.endTransmission();
    Wire.requestFrom(C_ADDRESS, 2); 
    buffer[0] = Wire.read();
    buffer[1] = Wire.read();
    int angle = word(buffer[1], buffer[0]) - OFFSET;
    if(angle < 0) angle += 360;
    if(mode == 0) return double(angle);
    return degToDec(angle);
}

//Check if compass is pointing towards (fake) north
bool Compass::north(){
    int angle = this->read(0);
    if(angle < C_LIM || angle >  360 - C_LIM) return true;
    else return false;
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

//IR sensor and Multiplexer setup
Infrared::Infrared(int id, int *PINS, int Mux0, int Mux1, int Mux2, int Mux3, int Mux_IN, int SAMPLES, int DELAY){
    this->id = id;
    this->Mux0 = Mux0;
    this->Mux1 = Mux1;
    this->Mux2 = Mux2;
    this->Mux3 = Mux3;
    this->Mux_IN = Mux_IN;
    this->SAMPLES = SAMPLES;
    this->DELAY = DELAY;
    memcpy(this->PINS, PINS, sizeof(this->PINS));
    pinMode(Mux0, OUTPUT);
    pinMode(Mux1, OUTPUT);
    pinMode(Mux2, OUTPUT);
    pinMode(Mux3, OUTPUT);
    pinMode(Mux_IN, INPUT);
}

//Write 4-bit number to IR multiplexer
void Infrared::setMux(int n){
    digitalWrite(Mux0, (n % 2 == 0 ? 0 : 1));
    n /= 2;
    digitalWrite(Mux1, (n % 2 == 0 ? 0 : 1));
    n /= 2;
    digitalWrite(Mux2, (n % 2 == 0 ? 0 : 1));
    n /= 2;
    digitalWrite(Mux3, (n % 2 == 0 ? 0 : 1));
}

//TODO: Check with different sample size and delay, or full cycle for every check
//Takes samples of IR sensors with MUX to calculate the vector sum (0: [0, 360], 1: [-1, 1])
double Infrared::read(int mode){
    int arr[16];
    memset(arr, 0, sizeof(arr));
    bool flag = false;
    for(int i = 0; i < 16; i++){
        this->setMux(PINS[i]);
        for(int j = 0; j < SAMPLES; j++){
            if(digitalRead(Mux_IN) == 0){
                flag = true;
                arr[i]++;
            }
            delayMicroseconds(DELAY);
        }
    }
    /*for(int i = 0; i < SAMPLES; i++){
        for(int j = 0; j < 16; j++){
            this->setMux(PINS[j]);
            if(digitalRead(MUX_IN) == 0){
                flag = true;
                arr[j]++;
            }
            delayMicroseconds(DELAY);
        }
    }*/
    if(!flag) return double(NaN);
    double angle, c_x = 0.0, c_y = 0.0;
    for(int i = 0; i < 16; i++){
        c_x += double(arr[i]) / double(SAMPLES) * sin(radians(double(i) * 22.5));
        c_y += double(arr[i]) / double(SAMPLES) * cos(radians(double(i) * 22.5));
    }
    angle = (c_x >= 0 ? 90.0 : 270.0) - degrees(atan(c_y / c_x));
    CERTAINTY = sqrt(sq(c_x) + sq(c_y));
    if(mode == 0) return angle;
    return degToDec(angle);
}

//Debug info in serial monitor
void Infrared::debug(){
    Serial.print(" IR: ");
    Serial.print(this->read(0));
    Serial.print(" CERTAINTY: ");
    Serial.println(CERTAINTY);
}

I2C::I2C(int id, int ADDRESS, int MSG_LENGTH){
    this->id = id;
    this->ADDRESS = ADDRESS;
    this->MSG_LENGTH = MSG_LENGTH;
}

#if ROBOT_ID == 0

//Initialize I2C Slave <-> Master communications
void I2C::init(){
    if(this->requestMsg() != 0) Serial.println("Problem in I2C I2C");
    else Serial.println("MCU <-> MCU I2C check complete");
}

//Translates byte array message into variables
void I2C::parseMsg(String str){
    for(int i = 0; i < MSG_LENGTH; i++){
        switch(str[i]){
            case 'S': case 'X':
                continue;
            case 'I':
                IR_STATE = str.substring(i+1, i+4).toInt();
                i += 3;
                break;
            case 'C':
                COMP_STATE = str.substring(i+1, i+4).toInt();
                i += 3;
                break;
            case 'U':
                US1_STATE = str.substring(i+1, i+4).toInt();
                i += 3;
                break;
            case 'u':
                US2_STATE = str.substring(i+1, i+4).toInt();
                i += 3;
                break;
            case 'E':
                if(i != MSG_LENGTH - 1) Serial.println("uh oh");
        }
    }
}

//Requests message from I2C Slave
int I2C::requestMsg(){
    String msg = "";
    Wire.beginTransmission(ADDRESS);
    Wire.write(0x00);
    Wire.endTransmission();
    Wire.requestFrom(ADDRESS, MSG_LENGTH);
    for(int i = 0; i < MSG_LENGTH; i++){
        char temp = Wire.read();
        msg += S(temp);
    }
    if(Wire.available()) return -1;
    parseMsg(msg);
    return 0;
}

//Outputs desired info from buffer ("IR", "COMP", "US")
double I2C::read(String type, int mode){
    this->requestMsg();
    if(type == "IR"){
        if(mode == 0) return double(IR_STATE);
        return degToDec(IR_STATE);
    }
    else if(type == "COMP"){
        if(mode == 0) return double(COMP_STATE);
        return degToDec(COMP_STATE);
    }
    else if(type == "US"){
        if(mode == 0) return double(US1_STATE);
        return double(US2_STATE);
    }
    Serial.println("nuh uh");
    return double(NaN);
}

//Getter for all buffers
void I2C::getInfo(int &IR, int &COMP, int &US1, int &US2){
    this->requestMsg();
    IR = IR_STATE;
    COMP = COMP_STATE;
    US1 = US1_STATE;
    US2 = US2_STATE;
}

//Debug info in serial monitor
void I2C::debug(){
    this->getInfo(IR_STATE, COMP_STATE, US1_STATE, US2_STATE);
    Serial.print(" I2C IR: ");
    Serial.print(IR_STATE);
    Serial.print(" COMP: ");
    Serial.print(COMP_STATE);
    Serial.print(" US1: ");
    Serial.print(US1_STATE);
    Serial.print(" US2: ");
    Serial.println(US2_STATE);
}

//Wireless communication setup
Wireless::Wireless(int id, uint8_t *ADDRESS){
    this->id = id;
    memcpy(this->ADDRESS, ADDRESS, sizeof(this->ADDRESS));
}

//Initialise ESP-NOW wireless communications
void Wireless::init(){
    WiFi.mode(WIFI_STA);
    esp_now_init();
    esp_now_register_send_cb(WLsendCB);
    esp_now_register_recv_cb(WLreceiveMsg);
    memcpy(ESP.peer_addr, ADDRESS, 6);
    ESP.channel = 0;
    ESP.encrypt = false;
    esp_now_add_peer(&ESP);
}

//Callback function for message deliveries
void WLsendCB(const uint8_t *ADDRESS, esp_now_send_status_t status){
    Serial.print(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery failed");
}

//Handler for received messages
void WLreceiveMsg(const uint8_t *ADDRESS, const uint8_t *package, int n){
    memcpy(&WLMSG, package, sizeof(WLMSG));
}

//Handler for sending messages
void Wireless::send(){
    WLmsg msg;
    esp_err_t status = esp_now_send(ADDRESS, (uint8_t *) &msg, sizeof(msg));
    Serial.println(status == ESP_OK ? "Message sent" : "Error sending message");
}

//Read wireless info package
WLmsg Wireless::read(){
    return WLMSG;
}

#else

//Initialise I2C communications
void I2C::init(){
    Wire.onRequest(this->sendMsg);
}

//Handler for sending I2C messages to master
void I2C::sendMsg(){
    char buf[21];
    sprintf(buf, "SI%03dXC%03dXU%03dXu%03dE", I2C_IR, I2C_COMP, I2C_US1, I2C_US2);
    Wire.write(buf);
}

#endif
