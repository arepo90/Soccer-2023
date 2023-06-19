/*
    Game Plan
    Very WIP
*/

#if ROBOT_ID == 0

//Motor control for a given angle [-1, 1]
void followPath(double angle){
    M1.move(int(POW1 * sin(PI*angle - PI/4.0)));
    M2.move(int(POW2 * sin(PI*angle - 3.0*PI/4.0)));
    M3.move(int(POW3 * sin(PI*angle + 3.0*PI/4.0)));
    M4.move(int(POW4 * sin(PI*angle + PI/4.0)));
}
/*
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
        M1.update(abs(POW1 * error * FACTOR));
        M2.update(abs(POW2 * error * FACTOR));
        M3.update(abs(POW3 * error * FACTOR));
        M4.update(abs(POW4 * error * FACTOR));
    }
}
*/
//---------------Game functions---------------

//Game plan (0: master, 1: slave)
void gp(int mode){
    
    return;    
}

//Staying within boundaries
//The delay system only works well with POWER < 60
void line(){
    /*if(lin3){
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
    else Serial.println("If you are reading this, there is a memory leak :p");*/
}

//Returning when no ball detected - WIP - Ultrasonics are NOT reliable
void comeback(){
    /*if(readLines()){
        line();
        return;
    }*/
    //vectorControl(1.0);
}
/*
//Returning when no ball is detected with US - WIP heavy testing required
void usComeback(){
    //int dis1 = U1.read(), dis2 = U2.read();
    if(dis1 == 0 || dis2 == 0) vectorControl(1.0);
    else if(dis1 < dis2){
        if(dis1 < MIN_DIS) vectorControl(0.5);
        else if(dis1 > MAX_DIS) vectorControl(-0.75);
        else vectorControl(1.0);
    }
    else{
        if(dis2 < MIN_DIS) vectorControl(-0.5);
        else if(dis2 > MAX_DIS) vectorControl(0.75);
        else vectorControl(1.0);
    }
}

//Ball tracking
void ball(){
    if(readLines()){
        line();
        return;
    }
    //double angle = IR.read(1);
    if(angle == double(NaN)) comeback();
    else{
        if(angle > 60.0) angle += 30.0;
        else if(angle < -60.0) angle -= 30.0;
        vectorControl(degToDec(angle));
    }
}

//Simple orientation
void orientation(){
    //double angle = Comp.read(1);
    rotate(int(angle * (KPF - KPI) + (angle / fabs(angle)) * KPI));
}
*/
#else

 

#endif
