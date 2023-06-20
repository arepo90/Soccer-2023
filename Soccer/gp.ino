/*
    Game Plan
    Very WIP
*/

//Motor control for a given vector [-1, 1]
void followPath(double angle, double magnitude){
    M1.move(int(magnitude * POW1 * sin(PI*angle - PI/4.0)));
    M2.move(int(magnitude * POW2 * sin(PI*angle - 3.0*PI/4.0)));
    M3.move(int(magnitude * POW3 * sin(PI*angle + 3.0*PI/4.0)));
    M4.move(int(magnitude * POW4 * sin(PI*angle + PI/4.0)));
}

//Follow path for given vector with angle correction [-1, 1]
void vectorControl(double angle, double magnitude){
    double error = COMP_ANGLE;
    if(error > 0.0) error -= double(C_LIM) / 360.0;
    else error += double(C_LIM) / 360.0;
    followPath(angle, magnitude);
    if(fabs(error) > THRESHOLD){
        M1.update(abs(POW1 * error * FACTOR));
        M2.update(abs(POW2 * error * FACTOR));
        M3.update(abs(POW3 * error * FACTOR));
        M4.update(abs(POW4 * error * FACTOR));
    }
}
*/
//---------------Game functions---------------

#if ROBOT_ID == 0

//Game plan
void gp(){
    Comms.getInfo(IR_ANGLE, COMP_ANGLE, US1_DIS, US2_DIS);
    LINE_ANGLE = LS.read(1); LINE_MAG = LS.read(3);
    if(LINE_ANGLE != double(NaN)) lines();
    else if(IR_ANGLE != double(NaN)) ball();
    else usComeback();
}

#else

//Game plan
void gp(){
    Comms.getInfo(IR_ANGLE, COMP_ANGLE, US1_DIS, US2_DIS);
    LINE_ANGLE = LS.read(0); LINE_MAG = LS.read(3);
    if(LINE_ANGLE != double(NaN)){
        VECTOR_ANGLE = fmod(LINE_ANGLE + 180.0, 360.0);
        if(IR_ANGLE != double(NaN)){
            double diff = fmod(IR_ANGLE - LINE_ANGLE + 360.0, 360.0);
            if(fabs(diff) <= 90.0) VECTOR_ANGLE += diff;
            else VECTOR_ANGLE += 90.0 * (diff/fabs(diff));
            VECTOR_MAG = LINE_MAG;
        }
    }
    else if(IR_ANGLE != double(NaN)){
        VECTOR_ANGLE = IR_ANGLE;
        VECTOR_MAG = 1.0;
    }
    else{
        
    }
    vectorControl(VECTOR_ANGLE, VECTOR_MAG);
}

#endif

//Staying within boundaries
void lines(){
    VECTOR_ANGLE = LINE_ANGLE + (LINE_ANGLE > 0 ? -1.0 : 1.0);
    if(IR_ANGLE ) VECTOR_ANGLE = (VECTOR_ANGLE + IR_ANGLE) / 2.0;
    VECTOR_MAG = LINE_MAG;
}

//Returning when no ball detected
void comeback(){
    vectorControl(1.0);
}

//Returning when no ball is detected with US - WIP heavy testing required
void usComeback(){
    if(US1_DIS < US2_DIS){
        if(US1_DIS < MIN_DIS) vectorControl(0.5, 0.8);
        else if(dis1 > MAX_DIS) vectorControl(-0.75, 1.0);
        else vectorControl(1.0, 0.8);
    }
    else{
        if(US2_DIS < MIN_DIS) vectorControl(-0.5, 0.8);
        else if(US2_DIS > MAX_DIS) vectorControl(0.75, 1.0);
        else vectorControl(1.0, 0.8);
    }
}

//Ball tracking
void ball(){
    double angle = decToDeg(IR_ANGLE);
    if(angle > 60.0) angle += 30.0;
    else if(angle < -60.0) angle -= 30.0;
    vectorControl(degToDec(angle), 1.0);
}

//Simple orientation
void orientation(){
    rotate(int(IR_ANGLE * (KPF - KPI) + (IR_ANGLE/fabs(IR_ANGLE)) * KPI));
}
