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
//#include <Python.h>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include "DynamixelAXControl.h"

// Direcciones para los registros de servomotores AX-12A/AX-18A
    //Direcciones en EEPROM
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
    //Direcciones en RAM
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
// Parametros de funcionamiento
#define MAX_ANGLE_LIMIT 1023
#define MIN_ANGLE_LIMIT 0
#define MIN_SPEED 0
#define MAX_JOINT_SPEED 1023
#define MAX_WHEEL_SPEED 2047
#define HEADER_MESSAGE "[ID: "+std::to_string(this->idMotor)+"] "

    DynamixelAXControl::DynamixelAXControl(DynamixelManager* control, int idMotor):control(control), idMotor(idMotor){
        int nCwlimit = 0;
        int nCcwlimit = 0;
        bWheelMode = false;
    }
    
    DynamixelAXControl::~DynamixelAXControl(){}

    int DynamixelAXControl::get_dxl_comm_result(){
        return control -> get_dxl_comm_result();
    }
    
    std::string DynamixelAXControl::get_message(){
        return this -> sMessage;
    }
    
    uint8_t DynamixelAXControl::get_dxl_error(){
        return control -> get_dxl_error();
    }
    
    bool DynamixelAXControl::connect(){
        if(!control -> getStatusConnection()){
            sMessage.append("Connection not available\n");
            return false;
        }
        if(!control -> pingServo(idMotor)){
            sMessage.assign("ERROR: Servo [id:"+std::to_string(this->idMotor)+"] not found!\n");
            return false;
        }
        nCwlimit = control -> read2byte(idMotor, ADDR_CW_ANGLE_LIMIT);
        nCcwlimit = control -> read2byte(idMotor, ADDR_CCW_ANGLE_LIMIT);
        if(nCwlimit < 0 || nCcwlimit < 0){
            sMessage.assign(HEADER_MESSAGE + "Fail to evaluate mode.\n");   
            return false;
        }else if(nCcwlimit == 0 || nCwlimit == 0){
            bWheelMode = true;
            sMessage.assign(HEADER_MESSAGE + "in wheel mode.\n");
        }else{
            bWheelMode = false;
            sMessage.assign(HEADER_MESSAGE + "Joint mode.\n");
        }
        return true;
            
    }

    bool DynamixelAXControl::setWheelMode(){
        if (!control -> write1byte(idMotor,ADDR_TORQUE_ENABLE, 0)) {
            sMessage.assign(HEADER_MESSAGE +"Failed to disable torque!\n");
            return false;
        }
        if(!setCWAngleLimit(0))
            return false;
        if(!setCCWAngleLimit(0))
            return false;
        if (!control -> write1byte(idMotor, ADDR_TORQUE_ENABLE, 1)) {
            sMessage.assign(HEADER_MESSAGE +"Failed to enable torque!\n");
            return false;
        }
        sMessage.assign(HEADER_MESSAGE +"Succeeded to set in wheel mode.\n");
        bWheelMode = true;
        return true;
    }
    
    bool DynamixelAXControl::setJointMode(int cwLimit, int ccwLimit){
        if (!control -> write1byte(idMotor, ADDR_TORQUE_ENABLE, 0)) {
            sMessage.assign(HEADER_MESSAGE +"Failed to disable torque!\n");
            return false;
        }
        if(!setCWAngleLimit(cwLimit))
            return false;
        if(!setCCWAngleLimit(ccwLimit))
            return false;
        if (!control -> write1byte(idMotor, ADDR_TORQUE_ENABLE, 1)) {
            sMessage.assign(HEADER_MESSAGE +"Failed to enable torque!\n");
            return false;
        }
        sMessage.assign(HEADER_MESSAGE +"Succeeded to set in joint mode.\n");
        bWheelMode = false;
        return true;
    }
    
    bool DynamixelAXControl::setPosition(int position){
        uint16_t goalPosition;
        if(this -> bWheelMode){
            sMessage.assign(HEADER_MESSAGE +"Not posible to configure\n");
            return false;
        }
        if(position > nCcwlimit || position < nCwlimit){
            sMessage.append(HEADER_MESSAGE +"Value out of range!\n");
        }
        goalPosition = (uint16_t) clamp(position,nCwlimit,nCcwlimit);
        if (!control -> write2byte(idMotor,ADDR_GOAL_POSITION, goalPosition)) {
            sMessage.append(HEADER_MESSAGE +"Failed to set position!\n");
            return false;
        }
        sMessage.append(HEADER_MESSAGE +"set in position: "+ std::to_string(goalPosition) +".\n");
        return true;
    }
    
    bool DynamixelAXControl::setSpeed(int speed){
        uint16_t goalSpeed;
        if(this -> bWheelMode){
            if(speed > MAX_WHEEL_SPEED || speed < MIN_SPEED){
                sMessage.assign(HEADER_MESSAGE +"Value out of range!\n");
            }
            goalSpeed = (uint16_t) clamp(speed,MIN_SPEED,MAX_WHEEL_SPEED);
        }else{
            if(speed > MAX_JOINT_SPEED || speed < MIN_SPEED){
                sMessage.assign(HEADER_MESSAGE +"Value out of range!\n");
            }
            goalSpeed = (uint16_t) clamp(speed,MIN_SPEED,MAX_JOINT_SPEED);
        }
        goalSpeed = (uint16_t) speed;
        if (!control -> write2byte(idMotor,ADDR_MOVING_SPEED, goalSpeed)) {
            sMessage.assign(HEADER_MESSAGE +"Failed to set speed!\n");
            return false;
        }
        sMessage.assign(HEADER_MESSAGE +"set speed to "+ std::to_string(goalSpeed) +".\n");
        return true;
    }
    
    int DynamixelAXControl::getPosition(){
        int position = control -> read2byte(idMotor,ADDR_PRESENT_POSITION);
        if (position != -1) 
            sMessage.assign(HEADER_MESSAGE +"Current position: "+std::to_string(position)+" \n");
         else 
            sMessage.assign(HEADER_MESSAGE +"Error getting present position.\n");
        return position;
    }
    
    int DynamixelAXControl::getSpeed(){
        int speed = control -> read2byte(idMotor,ADDR_PRESENT_SPEED);
        if (speed != -1) 
            sMessage.assign(HEADER_MESSAGE +"Present speed: "+std::to_string(speed)+" \n");
        else 
            sMessage.assign(HEADER_MESSAGE +"Error getting present speed.");
        return speed;
    }
    
    int DynamixelAXControl::getVoltaje(){
        int voltaje = control -> read1byte(idMotor, ADDR_PRESENT_VOLTAJE);
        if (voltaje != -1) {
            sMessage.assign(HEADER_MESSAGE +"Present voltaje: "+std::to_string(voltaje)+" \n");
        } else {
            sMessage.assign(HEADER_MESSAGE +"Error getting present voltaje.");
        }
        return voltaje;
    }
    
    int DynamixelAXControl::getTemperature(){
        int temperature = control -> read1byte(idMotor, ADDR_PRESENT_TEMPERATURE);
        if (temperature != -1) {
            sMessage.assign("[ID: "+std::to_string(this->idMotor)+"] Present temperature: "+std::to_string(temperature)+" \n");
        } else {
            sMessage.assign("[ID: "+std::to_string(this->idMotor)+"] Error getting present temperature.");
        }
        return temperature;
    }
    
    bool DynamixelAXControl::getMoving(){return true;}

    int DynamixelAXControl::clamp(int value, int minLimit, int maxLimit){
        return (value < minLimit) ? minLimit : (value > maxLimit) ? maxLimit : value;
    }
    
    bool DynamixelAXControl::setCWAngleLimit(int limit){
        uint16_t cwLimit;
        if(cwLimit > MAX_ANGLE_LIMIT || cwLimit < MIN_ANGLE_LIMIT){
            sMessage.assign(HEADER_MESSAGE +"Value out of range!\n");
        }
        cwLimit = (uint16_t) clamp(limit,MIN_ANGLE_LIMIT,MAX_ANGLE_LIMIT);
        if (!control -> write2byte(idMotor,ADDR_CW_ANGLE_LIMIT,cwLimit)){
            sMessage.assign(HEADER_MESSAGE +"Failed to set cw angle limit!\n");
            return false;
        }
        nCwlimit = cwLimit;
        return true;
    }

    bool DynamixelAXControl::setCCWAngleLimit(int limit){
        uint16_t ccwLimit;
        if(ccwLimit > MAX_ANGLE_LIMIT || ccwLimit < MIN_ANGLE_LIMIT){
            sMessage.assign(HEADER_MESSAGE +"Value out of range!\n");
            return false;
        }
        ccwLimit = (uint16_t) clamp(limit,MIN_ANGLE_LIMIT,MAX_ANGLE_LIMIT);
        if (!control -> write2byte(idMotor, ADDR_CW_ANGLE_LIMIT,ccwLimit)){
            sMessage.assign(HEADER_MESSAGE +"Failed to set cw angle limit!\n");
            return false;
        }
        nCcwlimit = ccwLimit;
        return true;
    }
