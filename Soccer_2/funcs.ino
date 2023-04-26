/*
    Helper functions and debugging
*/

//---------------General functions---------------

//Helper variables
ul before = 0, now = 0;

//Delay without stop (time in ms)
bool checkDelay(int time){
    now = millis();
    if(now - before < time) return false;
    else before = now;
    return true;
}

//Tracker function for checking program continuity
void tracker(){
    trackNow = millis();
    if(trackNow - trackBefore >= 500){
        digitalWrite(13, (led ? LOW : HIGH));
        led = !led;
        trackBefore = trackNow;
    }
}

//Check if robot is within boundaries
bool inside(){
    return !(L1.read() + L2.read() + L3.read() + L4.read());
}

//Initialize program (0: Serial port, 1: Compass, 2: IR, 3: All)
void globalInit(int mode){
    if(!arg1){
        L1.setLim(LIM_A1, LIM_B1);
        L2.setLim(LIM_A2, LIM_B2);
        L3.setLim(LIM_A3, LIM_B3);
        L4.setLim(LIM_A4, LIM_B4);
    }
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    Serial.begin(9600);
    Serial.print("Tacos ");
    if(mode % 2 == 1) Comp.init();
    if(mode >= 2) IR.init();
    Serial.print("de a ");
}

bool readLines(){
    lin1 = L1.read();
    lin2 = L2.read();
    lin3 = L3.read();
    lin4 = L4.read();
    return (lin1 + lin2 + lin3 + lin4);
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
}
