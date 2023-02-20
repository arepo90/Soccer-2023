void mem_save(int n, int target){
    for(int i = target*4; i < 4*(target+1); i++){
        EEPROM.write(i, (n >= 255 ? 255 : n));
        if(n >= 255) n -= 255;
        else n = 0;
    }
}

int mem_read(int target){
    int temp = 0;
    for(int i = target*4; i < 4*(target+1); i++){
        temp += EEPROM.read(i);
    }
    return temp;
}

class Motor{
    private:
        int EN;
        int PWM_A;
        int PWM_B;
    public:
        Motor(int EN, int PWM_A, int PWM_B);
        void mover(int pot);
};

Motor::Motor(int EN, int PWM_A, int PWM_B){
    this->EN = EN;
    this->PWM_A = PWM_A;
    this->PWM_B = PWM_B;
    pinMode(EN, OUTPUT);
    pinMode(PWM_A, OUTPUT);
    pinMode(PWM_B, OUTPUT);
}

void Motor::mover(int pot){
    digitalWrite(EN, HIGH);
    analogWrite(PWM_A, (pot < 0 ? 0 : pot));
    analogWrite(PWM_B, (pot < 0 ? pot : 0));
}

class Light{
    private:
        int PIN_A;
        int PIN_B;
        int LIM_A;
        int LIM_B;
    public:
        Light(int PIN_A, int PIN_B);
        void calibrate(int LIM_A, int LIM_B);
        int read(int op);
        int line();
};

Light::Light(int PIN_A, int PIN_B){
    this->PIN_A = PIN_A;
    this->PIN_B = PIN_B;
    pinMode(PIN_A, INPUT);
    pinMode(PIN_B, INPUT);
}

void Light::calibrate(int LIM_A, int LIM_B){
    this->LIM_A = LIM_A;
    this->LIM_B = LIM_B;
}

int Light::read(int op){
    return analogRead(op == 0 ? PIN_A : PIN_B);
}

int Light::line(){
    bool temp1 = false, temp2 = false;
    if(analogRead(PIN_A) <= LIM_A) temp1 = true;
    if(analogRead(PIN_B) <= LIM_B) temp2 = true;
    if(temp1 && temp2) return 3;
    else if(temp2) return 2;
    else if(temp1) return 1;
    else return 0;
}

/*class Ultrasonic{
    int TRIG;
    int ECHO;
    int TIMEOUT;
    unsigned long prevMicros;
    public:
        Ultrasonic(int TRIG, int ECHO, int TIMEOUT = 20000UL);
        int read();
        void setTimeout(unsigned long TIMEOUT);
};

Ultrasonic::Ultrasonic(int TRIG, int ECHO, unsigned long TIMEOUT) {
    this->TRIG = TRIG;
    this->ECHO = ECHO;
    this->TIMEOUT = TIMEOUT;
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
}

int Ultrasonic::read(){
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    delay(30);
    prevMicros = micros();
    while(!digitalRead(ECHO) && (micros() - prevMicros) <= TIMEOUT);
    prevMicros = micros();
    while(digitalRead(ECHO)  && (micros() - prevMicros) <= TIMEOUT);
    return (micros() - prevMicros)/2;
}

void setTimeout(int TIMEOUT){
    this->TIMEOUT = TIMEOUT;
}

class Comp(){

    public:
        Comp();
        int leer();
}

Comp::Comp(){
  
}

int Comp::leer(){
    int angulo = 0;
    return angulo;
}*/
