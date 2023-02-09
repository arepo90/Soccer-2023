void juego(){
    if(!norte) orientacion();
    else if(!dentro) linea();
    else if(!pelota) follow();
    else regreso();
}

void orientacion(){
    int angulo = Comp.leer();
    rotate(map(angulo, -180, 180, -100, 100));
}
