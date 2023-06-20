/*
    Helper functions and debugging
*/

//Initialize program (0: Serial, 1: Comms, 2: Compass, 3: Wireless, 4: All)
void globalInit(int mode){
    Serial.begin(9600);
    Serial.println("Tacos ");
    if(mode == 1 || mode == 4){
        if(ROBOT_ID == 0) Wire.begin();
        else{
            Wire.begin(COMMS_ADDRESS);
            Comms.init();
        }
    }
    #if ROBOT_ID == 0
        if(mode >= 3) WL.init();
    #else
        if(mode == 2 || mode == 4) Comp.init();
    #endif
    Serial.print("de a ");
}

#if ROBOT_ID == 0

//Print the MAC Address (only call this in void setup)
void getAddress(){
    WiFi.mode(WIFI_MODE_STA);
    Serial.println(WiFi.macAddress());
}

//Debug motor power
void motorDebug(){
    M1.debug();
    M2.debug();
    M3.debug();
    M4.debug();
}

#else

//Debug US distances
void usDebug(){
    U1.debug();
    U2.debug();
}

#endif
