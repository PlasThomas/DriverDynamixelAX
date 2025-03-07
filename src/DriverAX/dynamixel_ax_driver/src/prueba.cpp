#include "../include/DynamixelAXControl.h"
#include <unistd.h>
#include <string>
#include <iostream>

/*
pruebas
conect
disconnect
setWheelMode
setSpeed

*/

int main() {
    const int espera = 500000;
    const int idMotor = 1;
    const int baudrate = 1000000;
    const std::string puerto = "/dev/ttyUSB0";
    DynamixelAXControl motor1(puerto, baudrate, idMotor);

    // Conecta al puerto
    if (!motor1.connect()) {
        std::cout << motor1.get_message();
        return 1;
    }
    std::cout << motor1.get_message();
    
    // Configura el modo rueda
    if (!motor1.setJointMode(0,1000)) {
        std::cout << motor1.get_message();
        return 1;
    }
    std::cout << motor1.get_message();
    
    for(int i = 0; i < 3; i++){
        
        if(!motor1.setSpeed(700)){
            std::cout<<"fallo en asignar velocidad";
            return 1;
        }
        std::cout << motor1.get_message();
        
        if(!motor1.setPosition(500)){
            std::cout<<"fallo en asignar velocidad";
            return 1;
        }
        std::cout << motor1.get_message();
        usleep(espera);
        std::cout<< motor1.getPosition();
        if(!motor1.setPosition(950)){
            std::cout<<"fallo en asignar velocidad";
            return 1;
        }
        std::cout << motor1.get_message();
        usleep(espera);
        std::cout<< motor1.getPosition();
        if(!motor1.setPosition(0)){
            std::cout<<"fallo en asignar velocidad";
            return 1;
        }
        std::cout << motor1.get_message();
        usleep(espera);
        std::cout<< motor1.getPosition();
    }
    // Desconecta
    motor1.disconnect();
    return 0;
}