#ifndef DYNAMIXEL_CONTROL
#define DYNAMIXEL_CONTROL

#include <string>
#include "dynamixel_sdk/dynamixel_sdk.h"
class DynamixelAXControl{
    // Metodos publicos
    public:
        // Constructor y destructor
        DynamixelAXControl(const std::string& sPort, int nBaudrate, int idMotor);
        ~DynamixelAXControl();
        //getters
        int get_dxl_comm_result();
        std::string get_message();
        uint8_t get_dxl_error();
        // Funcionalidades para el motor.
        bool connect();//
        void disconnect();//
        bool setWheelMode();//
        bool setJointMode(int cwLimit, int ccwLimit);
        bool setPosition(int position);
        bool setSpeed(int speed);//
        int getPosition();
        int getSpeed();
        int getVoltaje();
        int getTemperature();
        bool getMoving();
    // Variables privadas
    private:
        //Metodos privados.
        int clamp(int value, int minLimit, int maxLimit);//
        bool pingServo();//
        bool setCWAngleLimit(int nlimit);//
        bool setCCWAngleLimit(int nlimit);//
        //Variables y objetos privados.
        std::string sMessage;
        std::string sPort;
        int nBaudrate;
        int idMotor;
        bool bWheelMode;
        int nCwlimit;
        int nCcwlimit;
        dynamixel::PortHandler* portHandler;
        dynamixel::PacketHandler* packetHandler;
        uint8_t dxl_error;
        int dxl_comm_result;
};
#endif