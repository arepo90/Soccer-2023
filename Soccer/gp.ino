/*
    Game Plan
    Priorities: Orientation, Line, Ball, Return
*/

//Strategy priority list
void gp(){
    if(!Comp.north()) orientation();
    else if(!inside()) line(0);
    else if(IR.read()) ball();
    else comeback();
}

//Orientation towards fake north (front)
void orientation(){
    double angle = Comp.read(1);
    double U = angle * (KPF - KPI) + KPI;
    if(angle < 0) rotate(int(-U));
    else rotate(int(U));
}

//Staying within boundaries (0: immediate response, 1: checks for change in IR)
//The short delay may or may not be necessary for state 1 - WIP
void line(int state){
    if(state == 0){
        if(L1.read()) fwd();
        else if(L2.read()) rig();
        else if(L3.read()) bwd();
        else if(L4.read()) lef();
    }
    else{
        if(L1.read() == 1){
            int past = IR.read();
            while(IR.read() == past){
                stop();
            }
            fwd();
            delay(50);
        }
        else if(L1.read() != 0) fwd();
        else if(L2.read() == 1){
            int past = IR.read();
            while(IR.read() == past){
                stop();
            }
            rig();
            delay(50);
        }
        else if(L2.read() != 0) rig();
        else if(L3.read() == 1){
            int past = IR.read();
            while(IR.read() == past){
                stop();
            }
            bwd();
            delay(50);
        }
        else if(L3.read() != 0) bwd();
        else if(L4.read() == 1){
            int past = IR.read();
            while(IR.read() == past){
                stop();
            }
            lef();
            delay(50);
        }
        else if(L4.read() != 0) lef();
    }
}

//Ball tracking - WIP - Check cases for best performance
void ball(){
    int direction = IR.read();
    switch(direction){
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

//Returning when no ball detected - WIP - Check ultrasonics' reliability
void comeback(){
    bwd();
}
