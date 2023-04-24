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
    line();
}

//Orientation towards fake north (front)
void orientation(){
    double angle = Comp.read(1);
    rotate(int(angle * (KPF - KPI) + (angle / fabs(angle)) * KPI));
}

//Staying within boundaries - WIP - Delays may be necessary
//Currently works in close proximity, fails with momentum
void line(){
    if(L3.read()) bwd();
    else if(L1.read()) fwd();
    else if(L2.read()) rig();
    else if(L4.read()) lef();
    else{
        //ball(0);
        vectorControl(0.0);
    }
}

//Returning when no ball detected - WIP - Ultrasonics are NOT reliable
void comeback(){
    vectorControl(1.0);
}

//Ball tracking (0: Move to ball angle, 1: Particular cases)
void ball(int mode){
    if(mode == 0){
        double angle = IR.read(1);
        if(angle == double(NaN)) comeback();
        else{
            if(angle > 60.0) angle += 30.0;
            else if(angle < -60.0) angle -= 30.0;
            vectorControl(degToDec(angle));
        }
    }
    else{
        switch(IR.read(0)){
            case 0:
                comeback();
                break;
            case 1: case 9:
                bwd();
                break;
            case 2: case 3:
                lef();
                break;
            case 4:
                lefDiag();
                break;
            case 5:
                fwd();
                break;
            case 6:
                rigDiag();
                break;
            case 7: case 8:
                rig();
                break;
        }
    }
}

void circleFunc(int updt){
    if(checkDelay(100)) Comp.setOffset((Comp.getOffset() + updt) % 360);
    vectorControl(-0.5);
}
