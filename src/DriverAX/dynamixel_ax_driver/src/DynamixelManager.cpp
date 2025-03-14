#include "DynamixelManager.h"

    // Constructor y destructor
    DynamixelManager::DynamixelManager(const std::string& port, int baudrate, float protocol): port(port),baudrate(baudrate){
        portHandler = dynamixel::PortHandler::getPortHandler(port.c_str());
        packetHandler = dynamixel::PacketHandler::getPacketHandler(protocol);
        dxl_error = 0;
        dxl_comm_result = COMM_TX_FAIL;
        bConnection = false;
    }

    DynamixelManager::~DynamixelManager(){
        disconnect();
        delete portHandler;
        delete packetHandler;
    }

        // Getters
    int DynamixelManager::get_dxl_comm_result(){
        return this->dxl_comm_result;
    }

    uint8_t DynamixelManager::get_dxl_error(){
        return this->dxl_error;
    }

    bool DynamixelManager::getStatusConnection(){
        return bConnection;
    }

    dynamixel::PortHandler*DynamixelManager::getPortHandler(){
        return portHandler;
    }

    dynamixel::PacketHandler*DynamixelManager::getPacketHandler(){
        return packetHandler;
    }

        // Funcionalidades
    int DynamixelManager::connect(){
        if(bConnection)
            return 0;
        else{
            dxl_comm_result = portHandler->openPort();
            if (dxl_comm_result==false)
                return 1;
            else{
                dxl_comm_result = portHandler->setBaudRate(baudrate);
                if (dxl_comm_result==false)
                    return 2;
                else{
                    bConnection = true;
                    return 0;
                }
            }
        }
    }

    void DynamixelManager::disconnect(){
        portHandler->closePort();
    }

    bool DynamixelManager::pingServo(int idServo){
        if(!bConnection)
            return false;
        dxl_comm_result = packetHandler->ping(portHandler, idServo, &dxl_error);
            if (dxl_comm_result != COMM_SUCCESS)
                return false;
            else
                return true;
    }

    bool DynamixelManager::write1byte(int idServo, int address, int value){
        if(!bConnection)
            return false;
        dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, idServo, (uint16_t)address, (uint8_t)value, &dxl_error);
        if (dxl_comm_result!=COMM_SUCCESS)
            return false;
        else
            return true;
    }

    bool DynamixelManager::write2byte(int idServo, int address, int value){
        if(!bConnection)
            return false;
        dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, idServo, (uint16_t)address, (uint16_t)value, &dxl_error);
        if (dxl_comm_result!= COMM_SUCCESS)
            return false;
        else
            return true;
    }

    int DynamixelManager::read1byte(int idServo, int address){
        if(!bConnection)
            return -2;
        int value = 0;
        dxl_comm_result = packetHandler->read1ByteTxRx(portHandler, idServo, (uint16_t)address , (uint8_t*)&value, &dxl_error);
        if(dxl_comm_result==COMM_SUCCESS)
            return value;
        else    
            return -1;
    }

    int DynamixelManager::read2byte(int idServo, int address){
        if(!bConnection)
            return -2;
        int value = 0;
        dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, idServo, (uint16_t)address , (uint16_t*)&value, &dxl_error);
        if(dxl_comm_result==COMM_SUCCESS)
            return value;
        else    
            return -1;
    }

