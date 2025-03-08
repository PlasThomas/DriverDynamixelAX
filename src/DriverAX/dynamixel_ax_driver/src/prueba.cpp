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
    const int espera = 500000;
    const int espera1 = 2000000;
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
    
    if (!motor1.setJointMode(0,1000)) {
        std::cout << motor1.get_message();
        return 1;
    }
    std::cout << motor1.get_message();
    
    for(int i = 0; i < 2; i++){
        
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

    if (!motor1.setWheelMode()) {
        std::cout << motor1.get_message();
        return 1;
    }
    std::cout << motor1.get_message();

    for(int j = 0; j < 2; j++){
        if(!motor1.setSpeed(600)){
            std::cout<<"fallo en asignar velocidad";
            return 1;
        }
        std::cout << motor1.get_message();
        std::cout<< motor1.getSpeed();
        usleep(espera1);

        if(!motor1.setSpeed(100)){
            std::cout<<"fallo en asignar velocidad";
            return 1;
        }
        std::cout << motor1.get_message();
        std::cout<< motor1.getSpeed();
        usleep(espera1);

        if(!motor1.setSpeed(1400)){
            std::cout<<"fallo en asignar velocidad";
            return 1;
        }
        std::cout << motor1.get_message();
        std::cout<< motor1.getSpeed();
        usleep(espera1);
    }
    
    // Desconecta
    motor1.disconnect();
    return 0;
}