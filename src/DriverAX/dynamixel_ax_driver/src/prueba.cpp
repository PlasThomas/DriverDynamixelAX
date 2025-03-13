#include "DynamixelAXControl.h"
#include "DynamixelManager.h"
#include <unistd.h>
#include <string>
#include <iostream>

int main() {
    const int baudrate = 1000000;
    const std::string device = "/dev/ttyUSB0";
    const float protocol = 1.0;
    DynamixelManager puerto(device,baudrate,protocol);
    int c = puerto.connect();
    
    if(c == 1)
        std::cout<<"Fallo al abrir el puerto.\n";
    else if (c == 2)
        std::cout<<"Fallo al establcer baudrate.\n";
    else
        std::cout << "Conexion exitosa.\n";
    DynamixelAXControl motor1(&puerto, 1);
    DynamixelAXControl motor2(&puerto, 3);
    motor1.connect();
    std::cout<<motor1.get_message();
    if(!motor1.setWheelMode())
        return 1;
    std::cout<<motor1.get_message();
    int delay = 2000000;
    motor1.setSpeed(1400);
    usleep(delay);
    motor1.setSpeed(400);
    usleep(delay);
    motor1.setSpeed(0);
    usleep(delay);

    //fin
    puerto.disconnect();
    return 0;
}

/*
motor2.connect();
    std::cout<<motor2.get_message();
    if(!motor1.setWheelMode())
        return 1;
    std::cout<<motor1.get_message();
    if(!motor2.setWheelMode())
        return 1;
    std::cout<<motor2.get_message();
    for(int i = 0; i < 2 ; i ++){
        if(!motor1.setSpeed(1600))
            return 1;
        std::cout<<motor1.get_message();
        if(!motor2.setSpeed(400))
            return 1;
        std::cout<<motor2.get_message();
        usleep(delay);
        if(!motor2.setSpeed(1600))
            return 1;
        std::cout<<motor2.get_message();
        if(!motor1.setSpeed(400))
            return 1;
        std::cout<<motor1.get_message();
        usleep(delay);
        if(!motor2.setSpeed(0))
            return 1;
        std::cout<<motor2.get_message();
        if(!motor1.setSpeed(0))
            return 1;
        std::cout<<motor1.get_message();
    }
*/