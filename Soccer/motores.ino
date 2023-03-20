/*
    Main movement functions
    Positive DEF moves robot counterclockwise
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
    M1.move(-DEF);
    M2.move(-DEF);
    M3.move(DEF);
    M4.move(DEF);
}

//Move backwards
void bwd(){
    M1.move(DEF);
    M2.move(DEF);
    M3.move(-DEF);
    M4.move(-DEF);
}

//Move left
void lef(){
    M1.move(-DEF);
    M2.move(DEF);
    M3.move(DEF);
    M4.move(-DEF);
}

//Move right
void rig(){
    M1.move(DEF);
    M2.move(-DEF);
    M3.move(-DEF);
    M4.move(DEF);
}

//Move towards the backward right diagonal
void rigDiag(){
    M1.move(0);
    M2.move(DEF);
    M3.move(0);
    M4.move(-DEF);
}

//Move towards the backward left diagonal
void lefDiag(){
    M1.move(-DEF);
    M2.move(0);
    M3.move(DEF);
    M4.move(0);
}

//Rotate with given DEF (DEF > 0 is counterclockwise)
void rotate(int n){
    M1.move(n);
    M2.move(n);
    M3.move(n);
    M4.move(n);
}

void movements(){
    rig();
    delay(500);
    stp(0);
    delay(500);
    bwd();
    delay(500);
    stp(0);
    delay(500);
    lef();
    delay(500);
    stp(0);
    delay(500);
    fwd();
    delay(500);
    stp(0);
    delay(500);
}
