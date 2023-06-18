/*
    Helper functions and debugging
*/

//Initialize program (0: Serial, 1: Comms, 2: Compass, 3: Wireless, 4: All)
void globalInit(int mode){
    Serial.begin(9600);
    Serial.println("Tacos ");
    if(mode == 1 || mode == 4){
        if(ROBOT_ID == 0) Wire.begin();
        else Wire.begin(COMMS_ADDRESS);
        while(Wire.available() > 0){
            Wire.read();
        }
    }
    #if ROBOT_ID == 0
        if(mode == 2 || mode == 4) Comp.init();
        if(mode >= 3) WL.init();
    #endif
    Serial.print("de a ");
}

#if ROBOT_ID == 0

//Prints the MAC Address
void getAddress(){
    WiFi.mode(WIFI_MODE_STA);
    Serial.println(WiFi.macAddress());
}

//Reads lines and returns true if any are active
bool readLines(){
    lin1 = L1.read();
    lin2 = L2.read();
    lin3 = L3.read();
    lin4 = L4.read();
    return (lin1 + lin2 + lin3 + lin4);
}

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

#else

//Debug US distances
void usDebug(){
    U1.debug();
    U2.debug();
}

#endif
