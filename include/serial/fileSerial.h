#pragma once

#include "interfaces/ISerial.h"

#include "mbed.h"

class FileSerial : public ISerial
{
private:
    FILE *_file;

    bool _close_on_destroy{true};

    constexpr static const uint8_t BUFFER_SIZE = 64;

public:
    explicit FileSerial(FILE *f, bool close_on_destroy = true);
    ~FileSerial();

    fpos_t position();

    size_t size() const;
    void seek(uint32_t);

    bool readable() override;

    ISerial &operator<<(int value) override;
    ISerial &operator<<(double value) override;
    ISerial &operator<<(const std::string &str) override;
    ISerial &operator<<(const char *str) override;
    ISerial &operator<<(char c) override;
    ISerial &operator>>(int &value) override;
    ISerial &operator>>(double &value) override;
    ISerial &operator>>(std::string &str) override;
    ISerial &operator>>(char *str) override;
    size_t readUntil(char *buffer, size_t size, char symbol) override;
};