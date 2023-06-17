/*
    Helper functions and debugging
*/

//---------------General functions---------------

//Helper variables
ul before = 0, now = 0;

void getAddress(){
    WiFi.mode(WIFI_MODE_STA);
    Serial.println(WiFi.macAddress());
}

//Delay without stop (time in ms)
bool checkDelay(int time){
    now = millis();
    if(now - before < time) return false;
    else before = now;
    return true;
}

//Reads lines and returns true if any are active
bool readLines(){
    lin1 = L1.read();
    lin2 = L2.read();
    lin3 = L3.read();
    lin4 = L4.read();
    return (lin1 + lin2 + lin3 + lin4);
}

//Initialize program (0: Serial, 1: Wireless, 2: Compass, 3: Comms, 4: All)
void globalInit(int mode){
    Serial.begin(9600);
    Serial.println("Tacos ");
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    if(mode >= 2){
        if(ROBOT_ID == 0) Wire.begin();
        else Wire.begin(COMMS_ADDRESS);
        while(Wire.available() > 0){
            Wire.read();
        }
    }
    if(mode == 1) WL.init();
    #if ROBOT_ID == 1
        if(mode >= 3) Comms.init();
        if(mode == 1 || mode == 3) Comp.init();
    #endif
    Serial.print("de a ");
}

//---------------Debugging functions---------------

#if ROBOT_ID == 0

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

#elif

//Debug US distances
void usDebug(){
    U1.debug();
    U2.debug();
}

#endif
