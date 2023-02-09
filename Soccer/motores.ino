void alto(){
    M1.mover(0);
    M2.mover(0);
    M3.mover(0);
    M4.mover(0);
}

void ade(){
    M1.mover(POWER);
    M2.mover(-POWER);
    M3.mover(-POWER);
    M4.mover(POWER);
}

void atras(){
    M1.mover(-POWER);
    M2.mover(POWER);
    M3.mover(POWER);
    M4.mover(-POWER);
}

void izq(){
    M1.mover(-POWER);
    M2.mover(-POWER);
    M3.mover(POWER);
    M4.mover(POWER);
}

void der(){
    M1.mover(POWER);
    M2.mover(POWER);
    M3.mover(-POWER);
    M4.mover(-POWER);
}

void diag_der(){
    M1.mover(0);
    M2.mover(POWER);
    M3.mover(0);
    M4.mover(-POWER);
}

void diag_izq(){
    M1.mover(-POWER);
    M2.mover(0);
    M3.mover(POWER);
    M4.mover(0);
}

void rotate(int n){
    M1.mover(n);
    M2.mover(n);
    M3.mover(n);
    M4.mover(n);
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
