#ifndef DYNAMIXEL_CONTROL
#define DYNAMIXEL_CONTROL

#include <string>
#include "DynamixelManager.h"
class DynamixelAXControl{
    // Metodos publicos
    public:
        // Constructor y destructor
        DynamixelAXControl(DynamixelManager* control, int idMotor);
        ~DynamixelAXControl();
        // Getters
        std::string get_message();
        // Funcionalidades para el motor.
        bool connect();
        bool setWheelMode();
        bool setJointMode(int cwLimit, int ccwLimit);
        bool setPosition(int position);
        bool setSpeed(int speed);
        int getPosition();
        int getSpeed();
        int getVoltaje();
        int getTemperature();
        bool getMoving();
    // Variables privadas
    private:
        //Metodos privados.
        int clamp(int value, int minLimit, int maxLimit);
        int convertAngleTOint(float angle, bool degrees);
        float convertIntTOangle(int angle);
        int convertRPMtoInt(float rpms);
        float convertINTtoRPM(int value);
        bool setCWAngleLimit(int nlimit);
        bool setCCWAngleLimit(int nlimit);
        //Variables y objetos privados.
        DynamixelManager* control;
        std::string sMessage;
        int idMotor;
        bool bWheelMode;
        int nCwlimit;
        int nCcwlimit;
};
#endif