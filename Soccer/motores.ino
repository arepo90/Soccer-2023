/*
    Main movement functions
    Positive power moves robot counterclockwise
*/

//Stop movement
void stop(){
    M1.move(0);
    M2.move(0);
    M3.move(0);
    M4.move(0);
}


//Move forwards
void fwd(){
    M1.move(POWER);
    M2.move(-POWER);
    M3.move(-POWER);
    M4.move(POWER);
}

//Move backwards
void bwd(){
    M1.move(-POWER);
    M2.move(POWER);
    M3.move(POWER);
    M4.move(-POWER);
}

//Move left
void lef(){
    M1.move(-POWER);
    M2.move(-POWER);
    M3.move(POWER);
    M4.move(POWER);
}

//Move right
void rig(){
    M1.move(POWER);
    M2.move(POWER);
    M3.move(-POWER);
    M4.move(-POWER);
}

//Move towards the backward right diagonal
void rigDiag(){
    M1.move(0);
    M2.move(POWER);
    M3.move(0);
    M4.move(-POWER);
}

//Move towards the backward left diagonal
void lefDiag(){
    M1.move(-POWER);
    M2.move(0);
    M3.move(POWER);
    M4.move(0);
}

//Rotate with given power (power > 0 is counterclockwise)
void rotate(int n){
    M1.move(n);
    M2.move(n);
    M3.move(n);
    M4.move(n);
}

void movements(){
    rig();
    delay(1000);
    stop();
    delay(100);
    bwd();
    delay(1000);
    stop();
    delay(100);
    lef();
    delay(1000);
    stop();
    delay(100);
    fwd();
    delay(1000);
    stop();
    delay(100);
}