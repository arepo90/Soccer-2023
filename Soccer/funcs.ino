/*
    General functions
*/

//Check if robot is within boundaries (0: outise, 1: inside)
bool inside(){
    return !(L1.read() + L2.read() + L3.read() + L4.read());
}

void globalInit(){
    Serial.begin(9600);
    //IR.init();
    Comp.init();
    Serial.print("Tacos de a ");
}

void defInit(){
    Serial.begin(9600);
    Serial.print("Tacos de a ");
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
