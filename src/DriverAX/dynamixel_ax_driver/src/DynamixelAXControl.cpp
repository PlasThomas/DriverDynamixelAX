/********************************************************************************************************
// Author: Plascencia Ramos Jose Manuel.
// Implementacion de la interfaz "dynamixelControl.h"
// Propocito:
// Simplificacion de la comunicacion usando el SDK de dynamixel.
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
*********************************************************************************************************/
//Invocacion de librerias
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include "DynamixelAXControl.h"

// Direcciones para los registros de servomotores AX-12A/AX-18A
    //Direcciones de la EEPROM
#define ADDR_MODEL_NUMBER 0             //REGISTROS DE SOLO LECTURA
#define ADDR_FIRMWARE_VERSION 2         //REGISTROS DE SOLO LECTURA
#define ADDR_ID 3 // De 0 a 253 
#define ADDR_BAUDRATE 4 // De 0 a 254
#define ADDR_RETURN_DELAY_TIME 5 // De 0 a 254
#define ADDR_CW_ANGLE_LIMIT 6 // De 0 a 1023
#define ADDR_CCW_ANGLE_LIMIT 8 // De 0 a 1023
#define ADDR_MAX_TORQUE 14 // De 0 a 1023
#define ADDR_STATUS_RETURN_LIMIT 16 // De 0 a 2
#define ADDR_ALARM_LED 17 // De 0 a 255
#define ADDR_SHUTDOWN 18 
    //Direcciones de la RAM
#define ADDR_TORQUE_ENABLE 24 
#define ADDR_LED 25 
#define ADDR_GOAL_POSITION 30 // De 0 a 1023
#define ADDR_MOVING_SPEED 32 // De 0 a 1023 en modo articulacion de 0 a 2047 en modo rueda
#define ADDR_TORQUE_LIMIT 34 // De 0 a 1023
#define ADDR_PRESENT_POSITION 36        //REGISTROS DE SOLO LECTURA
#define ADDR_PRESENT_SPEED 38           //REGISTROS DE SOLO LECTURA
#define ADDR_PRESENT_LOAD 40            //REGISTROS DE SOLO LECTURA
#define ADDR_PRESENT_VOLTAJE 42         //REGISTROS DE SOLO LECTURA
#define ADDR_PRESENT_TEMPERATURE 43     //REGISTROS DE SOLO LECTURA
#define ADDR_REGISTERED 44              //REGISTROS DE SOLO LECTURA
#define ADDR_MOVING 46                  //REGISTROS DE SOLO LECTURA
#define ADDR_LOCK 47 // De 0 o 1
#define ADDR_PUNCH 48 // De 0 a 1023
// Protocolo de comunicacion
#define PROTOCOL_VERSION 1.0
// Parametros de funcionamiento
#define MAX_ANGLE_LIMIT 1023
#define MIN_ANGLE_LIMIT 0
#define MIN_SPEED 0
#define MAX_JOINT_SPEED 1023
#define MAX_WHEEL_SPEED 2047

    DynamixelAXControl::DynamixelAXControl(const std::string& sPort, int nBaudrate, int idMotor):sPort(sPort), nBaudrate(nBaudrate), idMotor(idMotor){
        portHandler = dynamixel::PortHandler::getPortHandler(sPort.c_str());
        packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
        try{
            if(!pingServo())
                throw std::runtime_error(get_message());
        }
        catch(const std::exception& ex){
            std::cerr << "Error: " << ex.what() << std::endl;
        }
        int nCwlimit = 0;
        int nCcwlimit = 0;
        dxl_error = 0;
        dxl_comm_result = COMM_TX_FAIL;
        bWheelMode = false;
    }
    
    DynamixelAXControl::~DynamixelAXControl(){
        disconnect();
        delete portHandler;
        delete packetHandler;
    }
    
    int DynamixelAXControl::get_dxl_comm_result(){
        return this -> dxl_comm_result;
    }
    
    std::string DynamixelAXControl::get_message(){
        return this -> sMessage;
    }
    
    uint8_t DynamixelAXControl::get_dxl_error(){
        return this -> dxl_error;
    }
    
    bool DynamixelAXControl::connect(){
        dxl_comm_result = portHandler->openPort();
        if (dxl_comm_result==false) {
            sMessage.assign("Failed to open the port!\n");
            return false;
        }else{
            sMessage.assign("Succeeded to open the port.\n");
            dxl_comm_result = portHandler->setBaudRate(nBaudrate);
            if (dxl_comm_result==false) {
                sMessage.append("Failed to set the baudrate!\n");
                return false;
            }else{
                sMessage.append("Succeeded to set the baudrate.\n");
                return true;
            }
        }
    }
    
    void DynamixelAXControl::disconnect(){
        portHandler->closePort();
        sMessage.assign("Connection Finished.");
    }
    
    bool DynamixelAXControl::setWheelMode(){
        // Desactiva el torque
        dxl_comm_result = packetHandler->write1ByteTxRx(portHandler,idMotor,ADDR_TORQUE_ENABLE, 0, &dxl_error);
        if (dxl_comm_result!= COMM_SUCCESS) {
            sMessage.assign("Failed to disable torque!\n");
            return false;
        }
        sMessage.append("Succeeded to disable torque.\n");
        // Configura el modo rueda poniendo los limites de angulo en 0
        if(!setCWAngleLimit(0))
            return false;
        if(!setCCWAngleLimit(0))
            return false;
        // Activa el torque
        dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, idMotor, ADDR_TORQUE_ENABLE, 1, &dxl_error);
        if (dxl_comm_result!= COMM_SUCCESS) {
            sMessage.assign("Failed to enable torque!\n");
            return false;
        }
        sMessage.append("Succeeded to enable torque.\n");
        sMessage.append("Succeeded to set in wheel mode.\n");
        bWheelMode = true;
        return true;
    }
    
    bool DynamixelAXControl::setJointMode(int cwLimit, int ccwLimit){
        // Desactiva el torque
        dxl_comm_result = packetHandler->write1ByteTxRx(portHandler,idMotor,ADDR_TORQUE_ENABLE, 0, &dxl_error);
        if (dxl_comm_result!= COMM_SUCCESS) {
            sMessage.assign("Failed to disable torque!\n");
            return false;
        }
        // Se establecen los limites de 
        if(!setCWAngleLimit(cwLimit))
            return false;
        if(!setCCWAngleLimit(ccwLimit))
            return false;
        // Activa el torque
        dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, idMotor, ADDR_TORQUE_ENABLE, 1, &dxl_error);
        if (dxl_comm_result!= COMM_SUCCESS) {
            sMessage.assign("Failed to enable torque!\n");
            return false;
        }
        sMessage.assign("Succeeded to set in joint mode.\n");
        bWheelMode = false;
        return true;
    }
    
    bool DynamixelAXControl::setPosition(int position){
        uint16_t goalPosition;
        if(this -> bWheelMode){
            sMessage.assign("Not posible to configure\n");
            return false;
        }
        if(position > nCcwlimit || position < nCwlimit){
            sMessage.assign("Value out of range!\n");
            return false;
        }
        goalPosition = (uint16_t) position;
        dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, idMotor, ADDR_GOAL_POSITION, goalPosition, &dxl_error);
        if (dxl_comm_result!= COMM_SUCCESS) {
            sMessage.assign("Failed to set position!\n");
            return false;
        }
        sMessage.assign("Succeeded to set position.\n");
        return true;
    }
    
    bool DynamixelAXControl::setSpeed(int speed){
        uint16_t goalSpeed;
        if(this -> bWheelMode){
            if(speed > MAX_WHEEL_SPEED || speed < MIN_SPEED){
                sMessage.assign("Value out of range!\n");
                return false;
            }
        }else{
            if(speed > MAX_JOINT_SPEED || speed < MIN_SPEED){
                sMessage.assign("Value out of range!\n");
                return false;
            }
        }
        goalSpeed = (uint16_t) speed;
        dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, idMotor, ADDR_MOVING_SPEED, goalSpeed, &dxl_error);
        if (dxl_comm_result!= COMM_SUCCESS) {
            sMessage.assign("Failed to set speed!\n");
            return false;
        }
        sMessage.assign("Succeeded to set speed.\n");
        return true;
    }
    
    bool DynamixelAXControl::setCWAngleLimit(int limit){
        uint16_t cwLimit;
        if(cwLimit > MAX_ANGLE_LIMIT || cwLimit < MIN_ANGLE_LIMIT){
            sMessage.assign("Value out of range!\n");
        }
        cwLimit = (uint16_t) std::clamp(limit,MIN_ANGLE_LIMIT,MAX_ANGLE_LIMIT);
        dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, idMotor, ADDR_CW_ANGLE_LIMIT, cwLimit, &dxl_error);
        if (dxl_comm_result!= COMM_SUCCESS) {
            sMessage.assign("Failed to set cw angle limit!\n");
            return false;
        }
        nCwlimit = cwLimit;
        sMessage.append("Succeeded to set cw angle limit.\n");
        return true;
    }
    
    bool DynamixelAXControl::setCCWAngleLimit(int limit){
        uint16_t ccwLimit;
        if(ccwLimit > MAX_ANGLE_LIMIT || ccwLimit < MIN_ANGLE_LIMIT){
            sMessage.assign("Value out of range!\n");
            return false;
        }
        ccwLimit = (uint16_t) std::clamp(limit,MIN_ANGLE_LIMIT,MAX_ANGLE_LIMIT);
        dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, idMotor, ADDR_CCW_ANGLE_LIMIT, ccwLimit, &dxl_error);
        if (dxl_comm_result!= COMM_SUCCESS) {
            sMessage.assign("Failed to set ccw angle limit!\n");
            return false;
        }
        nCcwlimit = ccwLimit;
        sMessage.append("Succeeded to set ccw angle limit.\n");
        return true;
    }
    
    int DynamixelAXControl::getPosition(){
        int position = 0;
        dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, idMotor, ADDR_PRESENT_POSITION, (uint16_t*)&position);
        if (dxl_comm_result == COMM_SUCCESS) {
            sMessage.assign("ID: "+std::to_string(this->idMotor)+" Current position: "+std::to_string(position)+" \n");
        } else {
            sMessage.assign("Error getting present position.");
        }
        return position;
    }
    
    int DynamixelAXControl::getSpeed(){
        int speed = 0;
        dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, idMotor, ADDR_PRESENT_SPEED, (uint16_t*)&speed);
        if (dxl_comm_result == COMM_SUCCESS) {
            sMessage.assign("ID: "+std::to_string(this->idMotor)+" Present speed: "+std::to_string(speed)+" \n");
        } else {
            sMessage.assign("Error getting present speed.");
        }
        return speed;
    }
    
    int DynamixelAXControl::getVoltaje(){
        int voltaje = 0;
        dxl_comm_result = packetHandler->read1ByteTxRx(portHandler, idMotor, ADDR_PRESENT_VOLTAJE, (uint8_t*)&voltaje);
        if (dxl_comm_result == COMM_SUCCESS) {
            sMessage.assign("ID: "+std::to_string(this->idMotor)+" Present voltaje: "+std::to_string(voltaje)+" \n");
        } else {
            sMessage.assign("Error getting present voltaje.");
        }
        return voltaje;
    }
    
    int DynamixelAXControl::getTemperature(){
        int temperature = 0;
        dxl_comm_result = packetHandler->read1ByteTxRx(portHandler, idMotor, ADDR_PRESENT_TEMPERATURE, (uint8_t*)&temperature);
        if (dxl_comm_result == COMM_SUCCESS) {
            sMessage.assign("ID: "+std::to_string(this->idMotor)+" Present temperature: "+std::to_string(temperature)+" \n");
        } else {
            sMessage.assign("Error getting present temperature.");
        }
        return temperature;
    }
    
    bool DynamixelAXControl::pingServo(){
        dxl_comm_result = packetHandler->ping(portHandler, idMotor, &dxl_error);
        if (dxl_comm_result != COMM_SUCCESS) {
            sMessage.append("Servo [id:"+std::to_string(this->idMotor)+"] not found!\n");
            return false;
        }
        sMessage.assign("Servo [id:"+std::to_string(this->idMotor)+"] successfully found!\n");
            return false;
    }

    bool DynamixelAXControl::getMoving(){return true;}
