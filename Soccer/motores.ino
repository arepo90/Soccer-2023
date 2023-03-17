/*
    Main movement functions
    Positive POWER moves robot counterclockwise
*/

//Stop movement (0: regular, 1: hard brake)
void stp(int mode){
    if(mode == 0){
        M1.move(0);
        M2.move(0);
        M3.move(0);
        M4.move(0);
    }
    else{
        M1.brake(BRAKE);
        M2.brake(BRAKE);
        M3.brake(BRAKE);
        M4.brake(BRAKE);
    }
}

//Move forward
void fwd(){
    M1.move(-POWER);
    M2.move(-POWER);
    M3.move(POWER);
    M4.move(POWER);
}

//Move backwards
void bwd(){
    M1.move(POWER);
    M2.move(POWER);
    M3.move(-POWER);
    M4.move(-POWER);
}

//Move left
void lef(){
    M1.move(-POWER);
    M2.move(POWER);
    M3.move(POWER);
    M4.move(-POWER);
}

//Move right
void rig(){
    M1.move(POWER);
    M2.move(-POWER);
    M3.move(-POWER);
    M4.move(POWER);
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
    stp(0);
    delay(100);
    bwd();
    delay(1000);
    stp(0);
    delay(100);
    lef();
    delay(1000);
    stp(0);
    delay(100);
    fwd();
    delay(1000);
    stp(0);
    delay(100);
}
