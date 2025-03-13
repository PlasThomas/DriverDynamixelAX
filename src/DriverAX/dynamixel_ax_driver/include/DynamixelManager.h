#ifndef DYNAMIXEL_SDK_CONTROL
#define DYNAMIXEL_SDK_CONTROL

#include "dynamixel_sdk/dynamixel_sdk.h"
#include <string>
#include <memory>
class DynamixelManager
{
    public:
            // Constructor y destructor
        DynamixelManager(const std::string& sPort, int nBaudrate, float protocol);
        ~DynamixelManager();
            // Getters
        int get_dxl_comm_result();
        uint8_t get_dxl_error();
        bool getStatusConnection();
        dynamixel::PortHandler*getPortHandler();
        dynamixel::PacketHandler*getPacketHandler();
            // Funcionalidades
        int connect();
        void disconnect();
        bool pingServo(int idServo);
        bool write1byte(int idServo, int address, int value);
        bool write2byte(int idServo, int address, int value);
        int read1byte(int idServo, int address);
        int read2byte(int idServo, int address);
    private:
        dynamixel::PortHandler* portHandler;
        dynamixel::PacketHandler* packetHandler;
        bool bConnection;
        uint8_t dxl_error;
        int dxl_comm_result;
        int baudrate;
        std::string port;
};
#endif