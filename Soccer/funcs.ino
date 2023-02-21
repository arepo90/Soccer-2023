//Save 4-bit number to EEPROM at target slots (0-indexed)
void memSave(int n, int target){
    for(int i = target*4; i < 4*(target+1); i++){
        EEPROM.write(i, (n >= 255 ? 255 : n));
        if(n >= 255) n -= 255;
        else n = 0;
    }
}

//Read 4-bit number from EEPROM target slots (0-indexed)
int memRead(int target){
    int temp = 0;
    for(int i = target*4; i < 4*(target+1); i++){
        temp += EEPROM.read(i);
    }
    return temp;
}


/*I2C Communication - Not implemented yet
void wireInit(){
    Wire.begin();
    Wire.beginTransmission(compAddress);
    Wire.write(0x00);
    Wire.endTransmission();
    while(Wire.available() > 0){
        Wire.read();
    }
}*/
