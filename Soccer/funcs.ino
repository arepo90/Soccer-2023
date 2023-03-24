/*
    Helper functions and debugging
*/

//---------------General functions---------------

//Tracker functions for program continuity
void tracker1(){
    if(now == 0 || (!led && now - before >= 500)){
        digitalWrite(13, HIGH);
        before = now;
        led = true;
    }
    now = millis();
}
void tracker2(){
    if(led && now - before >= 500){
        digitalWrite(13, LOW);
        before = now;
        led = false;
    }
    now = millis();
}

//Check if robot is within boundaries (0: outside, 1: inside)
bool inside(){
    return !(L1.read() + L2.read() + L3.read() + L4.read());
}

//Initialize program (0: Default, 1: Compass, 2: IR, 3: All)
void globalInit(int mode){
    pinMode(13, OUTPUT);
    Serial.begin(9600);
    if(mode % 2 == 1) Comp.init();
    if(mode >= 2) IR.init();
    Serial.print("Tacos de a ");
}

//---------------Debugging functions---------------

//Debug motor power
void motorDebug(){
    M1.debug();
    M2.debug();
    M3.debug();
    M4.debug();
}

//Debug light values
void lightDebug(){
    L1.debug();
    L2.debug();
    L3.debug();
    L4.debug();
}

//Debug US distances
void usDebug(){
    U1.debug();
    U2.debug();
}

//Debug EEPROM memory
void memDebug(){
    for(int i = 0; i < 32; i++){
        Serial.print(EEPROM.read(i));
        Serial.print(" ");
    }
    Serial.println();
    for(int i = 0; i < 8; i++){
        Serial.print(" MEM");
        Serial.print(i);
        Serial.print(": ");
        Serial.print(memRead(i));
    }
}

//Debug all functions
void globalDebug(){
    motorDebug();
    lightDebug();
    usDebug();
    Comp.debug();
    IR.debug();
    Serial.println();
}
