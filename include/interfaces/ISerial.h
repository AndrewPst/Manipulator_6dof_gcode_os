#pragma once

#include <inttypes.h>
#include <string>

class ISerial
{
public:
    virtual bool readable() = 0;

    virtual ISerial &operator<<(int value) = 0;
    virtual ISerial &operator<<(double value)= 0;
    virtual ISerial &operator<<(const std::string &str)= 0;
    virtual ISerial &operator<<(const char *str) = 0;
    virtual ISerial &operator<<(char c) = 0;
    virtual ISerial &operator>>(int &value) = 0;
    virtual ISerial &operator>>(double &value) = 0;
    virtual ISerial &operator>>(std::string &str) = 0;
    virtual ISerial &operator>>(char *str) = 0;
    virtual size_t readUntil(char *buffer, size_t size, char symbol) = 0;

    virtual ~ISerial() = default;
};
