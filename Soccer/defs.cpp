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
    ledcAttachPin(PWM_A, (id*4));
    ledcAttachPin(PWM_B, (id*4+1));
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
Light::Light(int id, int PIN_A, int PIN_B, int LIM_A, int LIM_B){
    this->id = id;
    this->PIN_A = PIN_A;
    this->PIN_B = PIN_B;
    this->LIM_A = LIM_A;
    this->LIM_B = LIM_B;
    pinMode(PIN_A, INPUT);
    pinMode(PIN_B, INPUT);
}

//Read sensor block state (0: None, 1: Outer, 2: Both, 3: Outer)
int Light::read(){
    bool temp1 = false, temp2 = false;
    if(analogRead(PIN_A) >= LIM_A) temp1 = true;
    if(analogRead(PIN_B) >= LIM_B) temp2 = true;
    if(temp1 && temp2) return 2;
    else if(temp2) return 3;
    else if(temp1) return 1;
    else return 0;
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

//Compass sensor setup
Compass::Compass(int id, int C1, int C2, int LIM){
    this->id = id;
    this->LIM = LIM;
    ADDRESS = C1;
    REQ = C2;
}

//Angle read by mode (0: [0, 360], 1: [-1, 1])
double Compass::read(int mode){
    byte buffer[2];
    Wire.beginTransmission(ADDRESS);
    Wire.write(REQ);
    Wire.endTransmission();
    Wire.requestFrom(ADDRESS, 2); 
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
    if(angle < LIM || angle > 360-LIM) return true;
    else return false;
}

//Initialise I2C communication
void Compass::init(){
    OFFSET = this->read(0);
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
Infrared::Infrared(int id, int pins[], int Mux0, int Mux1, int Mux2, int Mux3, int Mux_IN, int SAMPLES, int DELAY){
    this->id = id;
    this->Mux0 = Mux0;
    this->Mux1 = Mux1;
    this->Mux2 = Mux2;
    this->Mux3 = Mux3;
    this->Mux_IN = Mux_IN;
    this->SAMPLES = SAMPLES;
    this->DELAY = DELAY;
    pinMode(Mux0, OUTPUT);
    pinMode(Mux1, OUTPUT);
    pinMode(Mux2, OUTPUT);
    pinMode(Mux3, OUTPUT);
    pinMode(Mux_IN, INPUT);
    for(int i = 0; i < 16; i++){
        PINS[i] = pins[i];
    }
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
    if(n < 0 || n > 1) Serial.println("Overflow in multiplexer");
}

//Takes samples of IR sensors with MUX to calculate the vector sum (0: [0, 360], 1: [-1, 1])
double Infrared::read(int mode){
    int arr[16], maxi = -1;
    bool flag = false;
    for(int i = 0; i < 16; i++){
        this->setMux(PINS[i]);
        int cont = 0;
        for(int j = 0; j < SAMPLES; j++){
            if(digitalRead(Mux_IN) == 0){
                flag = true;
                cont++;
            }
            delayMicroseconds(DELAY);
        }
        maxi = max(maxi, cont);
        arr[i] = cont;
    }
    if(!flag) return double(NaN);
    double angle, c_x = 0.0, c_y = 0.0;
    for(int i = 0; i < 16; i++){
        c_x += double(arr[i]) / double(SAMPLES) * sin(radians(double(i) * 22.5));
        c_y += double(arr[i]) / double(SAMPLES) * cos(radians(double(i) * 22.5));
    }
    angle = degrees(atan(c_y / c_x));
    if(c_x >= 0.0) angle = 90.0 - angle;
    else angle = 270 - angle;
    CERTAINTY = sqrt((c_x*c_x) + (c_y*c_y));
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

//Debug info in serial monitor
void I2C::debug(){
    Serial.print(" I2C IR: ");
    Serial.print(this->read("IR", 0));
    Serial.print(" COMP: ");
    Serial.print(this->read("COMP", 0));
    Serial.print(" US1: ");
    Serial.print(this->read("US", 0));
    Serial.print(" US2: ");
    Serial.println(this->read("US", 1));
}

//Wireless communication setup
Wireless::Wireless(int id, uint8_t *ADDRESS){
    this->id = id;
    for(int i = 0; i < 6; i++){
        this->ADDRESS[i] = ADDRESS[i];
    }
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
