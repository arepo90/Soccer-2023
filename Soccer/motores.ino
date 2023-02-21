/*
    Main movement functions
*/

void alto(){
    M1.move(0);
    M2.move(0);
    M3.move(0);
    M4.move(0);
}

void ade(){
    M1.move(POWER);
    M2.move(-POWER);
    M3.move(-POWER);
    M4.move(POWER);
}

void atras(){
    M1.move(-POWER);
    M2.move(POWER);
    M3.move(POWER);
    M4.move(-POWER);
}

void izq(){
    M1.move(-POWER);
    M2.move(-POWER);
    M3.move(POWER);
    M4.move(POWER);
}

void der(){
    M1.move(POWER);
    M2.move(POWER);
    M3.move(-POWER);
    M4.move(-POWER);
}

void diag_der(){
    M1.move(0);
    M2.move(POWER);
    M3.move(0);
    M4.move(-POWER);
}

void diag_izq(){
    M1.move(-POWER);
    M2.move(0);
    M3.move(POWER);
    M4.move(0);
}

void rotate(int n){
    M1.move(n);
    M2.move(n);
    M3.move(n);
    M4.move(n);
}

void movimientos(){
    ade();
    delay(1000);
    alto();
    delay(100);
    atras();
    delay(1000);
    alto();
    delay(100);
    izq();
    delay(1000);
    alto();
    delay(100);
    der();
    delay(1000);
    alto();
    delay(100);
}