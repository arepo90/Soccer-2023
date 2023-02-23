/*
    General functions
    Memory slots 0 -> 7 are reserved for light sensor limits
*/

void motorDebug(){
    M1.debug();
    M2.debug();
    M3.debug();
    M4.debug();
}

void lightDebug(){
    L1.debug();
    L2.debug();
    L3.debug();
    L4.debug();
}

void usDebug(){
    U1.debug();
    U2.debug();
}

void globalDebug(){
    motorDebug();
    lightDebug();
    usDebug();
    Serial.println();
}

//I2C Communication - Not implemented yet
/*void wireInit(){
    Wire.begin();
    Wire.beginTransmission(compAddress);
    Wire.write(0x00);
    Wire.endTransmission();
    while(Wire.available() > 0){
        Wire.read();
    }
}*/
