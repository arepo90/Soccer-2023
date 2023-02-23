/*
    General functions
    Memory slots 0 -> 7 are reserved for light sensor limits
*/

//Save 4-byte number to EEPROM at target slots (0-indexed)
void memSave(int n, int target){
    for(int i = 4*target; i < 4*(target+1); i++){
        EEPROM.update(i, (n >= 255 ? 255 : n));
        if(n >= 255) n -= 255;
        else n = 0;
    }
}

//Read 4-byte number from EEPROM target slots (0-indexed)
int memRead(int target){
    int ans = 0;
    for(int i = 4*target; i < 4*(target+1); i++){
        ans += EEPROM.read(i);
    }
    return ans;
}

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
