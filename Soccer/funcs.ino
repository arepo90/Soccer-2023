/*
    General functions
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

void globalDebug(){
    motorDebug();
    lightDebug();
    usDebug();
    Comp.debug();
    IR.debug();
    Serial.println();
}
