#ifndef DYNAMIXEL_CONTROL
#define DYNAMIXEL_CONTROL

#include "dynamixel_sdk/dynamixel_sdk.h"
#include <string>
class DynamixelManager
{
    public:
        DynamixelManager(const std::string& sPort, int nBaudrate, float protocol);
        ~DynamixelManager();
        bool connect();
        bool disconnect();
        bool pingServo(int idServo);
        bool write1byte(int idServo, int address, int value);
        bool write2byte(int idServo, int address, int value);
        int read1byte(int idServo, int address);
        int read2byte(int idServo, int address);
    private:
        dynamixel::PortHandler* portHandler;
        dynamixel::PacketHandler* packetHandler;
        bool bConnect;
        uint8_t dxl_error;
        int dxl_comm_result;
};
#endif