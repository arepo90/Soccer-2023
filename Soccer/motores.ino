/*
    Main movement dynamics
    Positive power translates to counterclockwise rotation (of the robot)
*/

//---------------Movement functions---------------

#if ROBOT_ID == 0

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
void fwd(double fact){
    M1.move(-fact*DEF);
    M2.move(-fact*DEF);
    M3.move(fact*DEF);
    M4.move(fact*DEF);
}

//Move backwards
void bwd(double fact){
    M1.move(fact*DEF);
    M2.move(fact*DEF);
    M3.move(-fact*DEF);
    M4.move(-fact*DEF);
}

//Move left
void lef(double fact){
    M1.move(-fact*DEF);
    M2.move(fact*DEF);
    M3.move(fact*DEF);
    M4.move(-fact*DEF);
}

//Move right
void rig(double fact){
    M1.move(fact*DEF);
    M2.move(-fact*DEF);
    M3.move(-fact*DEF);
    M4.move(fact*DEF);
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

//Rotate with given power
void rotate(int n){
    M1.move(n);
    M2.move(n);
    M3.move(n);
    M4.move(n);
}

#endif
