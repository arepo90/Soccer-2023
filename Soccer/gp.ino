/*
    Game Plan
    Priorities: Orientation, Line, Ball, Return (WIP)
*/

//----------Movement helper functions----------

//Motor control for a given angle [-1, 1]
void followPath(double angle){
    M1.move(int(POW1 * sin(PI*angle - PI/4.0)));
    M2.move(int(POW2 * sin(PI*angle - 3.0*PI/4.0)));
    M3.move(int(POW3 * sin(PI*angle + 3.0*PI/4.0)));
    M4.move(int(POW4 * sin(PI*angle + PI/4.0)));
}

//Follow path for given direction with angle correction [-1, 1]
void vectorControl(double angle){
    if(readLines()){
        line();
        return;
    }
    double error = Comp.read(1);
    if(error > 0.0) error -= double(C_LIM)/360.0;
    else error += double(C_LIM)/360.0;
    followPath(angle);
    if(fabs(error) > 0.0){
        M1.update(int(abs(M1.getPow()) * error * FACTOR));
        M2.update(int(abs(M2.getPow()) * error * FACTOR));
        M3.update(int(abs(M3.getPow()) * error * FACTOR));
        M4.update(int(abs(M4.getPow()) * error * FACTOR));
    }
}

//---------------Game functions---------------

//Game plan
void gp(){
    ball();
}

//Staying within boundaries
//The delay system only works well with POWER < 60
void line(){
    if(lin3){
        bwd((lin3 == 1 ? 2 : lin3));
        if(lin3 >= 2) delay(DEL);
    }
    else if(lin1){
        fwd((lin1 == 1 ? 2 : lin1));
        if(lin1 >= 2) delay(DEL);
    }
    else if(lin2){
        rig((lin2 == 1 ? 2 : lin2));
        if(lin2 >= 2) delay(DEL);
    }
    else if(lin4){
        lef((lin4 == 1 ? 2 : lin4));
        if(lin4 >= 2) delay(DEL);
    }
    else Serial.println("If you are reading this, there is a memory leak :p");
}

//Returning when no ball detected - WIP - Ultrasonics are NOT reliable
void comeback(){
    if(readLines()){
        line();
        return;
    }
    vectorControl(1.0);
}

//Ball tracking
void ball(){
    if(readLines()){
        line();
        return;
    }
    double angle = IR.read(1);
    if(angle == double(NaN)) comeback();
    else{
        if(angle > 60.0) angle += 30.0;
        else if(angle < -60.0) angle -= 30.0;
        vectorControl(degToDec(angle));
    }
}

//Simple orientation
void orientation(){
    double angle = Comp.read(1);
    rotate(int(angle * (KPF - KPI) + (angle / fabs(angle)) * KPI));
}

//Irrelevant - Orbits a point (w = 360/updt * del)
void circleFunc(int del, int updt){
    if(checkDelay(del)) Comp.setOffset((Comp.getOffset() + updt) % 360);
    vectorControl(-0.5);
}
