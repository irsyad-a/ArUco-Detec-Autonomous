#pragma once
#include <string>

// Simple POSIX serial transport (USB or Bluetooth RFCOMM).
// Open with a device path like /dev/ttyUSB0, /dev/ttyACM0, or /dev/rfcomm0
class SerialTransport
{
public:
    SerialTransport();
    ~SerialTransport();

    bool open(const std::string &devicePath, int baudRate);
    void close();
    bool isOpen() const;
    bool sendLine(const std::string &line); // appends '\n' if missing

private:
    int fd;
    bool configurePort(int baudRate);
};


