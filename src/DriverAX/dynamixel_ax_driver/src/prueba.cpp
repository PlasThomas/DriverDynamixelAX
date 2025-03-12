#include "../include/DynamixelAXControl.h"
#include <unistd.h>
#include <string>
#include <iostream>

/*
pruebas:
conect//
disconnect//
setWheelMode//
setSpeed//

*/

int main() {
    const int espera = 2000000;
    const int baudrate = 1000000;
    const int id = 1;
    const std::string puerto = "/dev/ttyUSB0";
    DynamixelAXControl motor1(puerto, baudrate, id);
    // Conecta al puerto
    if (!motor1.connect()) {
        std::cout << "no se conecto";
        return 1;
    }
    std::cout << motor1.get_message();
    if (!motor1.setWheelMode()) {
        std::cout << "FALLO MODO RUEDA\n";
        return 1;
    }
    std::cout << motor1.get_message();

    if(!motor1.setSpeed(400)){
        std::cout << "FALLO MODO velocidad\n";
        return 1;
    }
    usleep(espera);
    if(!motor1.setSpeed(1500)){
        std::cout << "FALLO MODO velocidad\n";
        return 1;
    }
    usleep(espera);
    if(!motor1.setSpeed(0)){
        std::cout<<"fallo en asignar velocidad";
        return 1;
    }

    if (!motor1.setJointMode(0,10000)) {
        std::cout << "FALLO MODO articulacion\n";
        return 1;
    }
    std::cout << motor1.get_message();

    if(!motor1.setPosition(0)){
        std::cout<<"fallo en asignar posicion";
        return 1;
    }
    usleep(espera);
    
    if(!motor1.setPosition(1000)){
        std::cout<<"fallo en asignar posicion";
        return 1;
    }
    usleep(espera);

    if(!motor1.setPosition(0)){
        std::cout<<"fallo en asignar posicion";
        return 1;
    }
    usleep(espera);
    // Desconecta
    motor1.disconnect();
    return 0;
}