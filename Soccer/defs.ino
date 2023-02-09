void save(int n, int target){
    for(int i = target*4; i < 4*(target+1); i++){
        EEPROM.write(i, (n >= 255 ? 255 : n));
        if(n >= 255) n -= 255;
        else n = 0;
    }
}

int leer(int target){
    int temp = 0;
    for(int i = target*4; i < 4*(target+1); i++){
        temp += EEPROM.read(i);
    }
    return temp;
}

class Motor{
    int EN;
    int PWM_A;
    int PWM_B;

    public:
        Motor(int en, int pwma, int pwmb);
        void mover(int pot);
};

Motor::Motor(int en, int pwma, int pwmb){
    this->EN = en;
    this->PWM_A = pwma;
    this->PWM_B = pwmb;
    pinMode(EN, OUTPUT);
    pinMode(PWM_A, OUTPUT);
    pinMode(PWM_B, OUTPUT);
}

void Motor::mover(int pot){
    digitalWrite(EN, HIGH);
    analogWrite(PWM_A, (pot < 0 ? 0 : pot));
    analogWrite(PWM_B, (pot < 0 ? pot : 0));
}

class Luz(){
    int PIN;

    public:
        Luz(int pin);
        int leer();
}

Luz::Luz(int pin){
    this->PIN = pin;
    pinMode(PIN, INPUT);
}

int Luz::leer(){
    return analogRead(PIN);
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
}
