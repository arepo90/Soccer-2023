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
        digitalWriteFast(13, (led ? LOW : HIGH));
        led = !led;
        trackBefore = trackNow;
    }
}

//Check if robot is within boundaries (0: outside, 1: inside)
bool inside(){
    return !(L1.read() + L2.read() + L3.read() + L4.read());
}

//Initialize program (0: Serial port, 1: Compass, 2: IR, 3: All)
void globalInit(int mode){
    pinModeFast(13, OUTPUT);
    digitalWriteFast(13, HIGH);
    Serial.begin(9600);
    Serial.println("Tacos ");
    if(mode % 2 == 1) Comp.init();
    if(mode >= 2) IR.init();
    Serial.print("de a ");
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
