#pragma once

#include "models/mutexLocker.h"
#include "interfaces/ISerial.h"

#include "mbed.h"
#include "rtos.h"

#include <string>

constexpr static const uint8_t BUFFER_SIZE = 128;
constexpr static const uint8_t LOCK_TIME = 4;

template <typename _serialT>
class UartSerial : public ISerial
{
private:
    _serialT *_serial;


    char SEPARATORS[8] = {' ', ',', '\t', '\n', '\0', '\r', '\\', '/'};

    char *_buffer{0};
    Mutex _mutex;

    void clearBuf(size_t size)
    {
        memset(_buffer, 0, size);
    }

    bool is_separator(char s)
    {
        uint8_t i{0};
        while (i < sizeof(SEPARATORS) / sizeof(SEPARATORS[0]))
            if (SEPARATORS[i++] == s)
                return true;
        return false;
    }

    size_t readUntilSeparators(char *buf)
    {
        uint8_t i = 0;
        while (_serial->readable())
        {
            _serial->read((buf + i), 1);
            if (is_separator(buf[i]))
                return i + 1;
            thread_sleep_for(LOCK_TIME);
            i++;
        }
        return i;
    }

    size_t readAll(char *buf)
    {
        uint8_t i = 0;
        while (_serial->readable())
        {
            _serial->read((buf + i), 1);
            thread_sleep_for(LOCK_TIME);
            i++;
        }
        return i;
    }

public:
    UartSerial(_serialT &serial) : _serial(&serial), _buffer(new char[BUFFER_SIZE]) {}

    bool readable() override
    {
        return _serial->readable();
    }

    ISerial &operator<<(int value) override
    {
        std::string str = std::to_string(value);
        _serial->write(str.c_str(), str.size());
        return *this;
    }

    ISerial &operator<<(double value) override
    {
        std::string str = std::to_string(value);
        _serial->write(str.c_str(), str.size());
        return *this;
    }

    ISerial &operator<<(const std::string &str) override
    {
        _serial->write(str.c_str(), str.size());
        return *this;
    }

    ISerial &operator<<(const char *str) override
    {
        _serial->write(str, strlen(str));
        return *this;
    }

    ISerial &operator<<(char c) override
    {
        _serial->write(&c, 1);
        return *this;
    }

    ISerial &operator>>(int &value) override
    {
        MutexLocker<Mutex> m(_mutex);
        clearBuf(BUFFER_SIZE);
        readUntilSeparators(_buffer);
        value = atoi(_buffer);
        return *this;
    }

    ISerial &operator>>(double &value) override
    {
        MutexLocker<Mutex> m(_mutex);
        clearBuf(BUFFER_SIZE);
        readUntilSeparators(_buffer);
        value = atof(_buffer);
        return *this;
    }

    ISerial &operator>>(std::string &str) override
    {
        MutexLocker<Mutex> m(_mutex);
        clearBuf(BUFFER_SIZE);
        readAll(_buffer);
        str = std::string(_buffer);
        return *this;
    }

    ISerial &operator>>(char *str) override
    {
        MutexLocker<Mutex> m(_mutex);
        readAll(str);
        return *this;
    }

    size_t readUntil(char *buffer, size_t size, char symbol) override
    {
        MutexLocker<Mutex> m(_mutex);
        size_t i = 0;
        char last_s = 0;
        while (_serial->readable() && last_s != symbol && i < size - 1)
        {
            _serial->read(&last_s, 1);
            buffer[i++] = last_s;
            thread_sleep_for(LOCK_TIME);
        }
        buffer[i] = '\0';
        return i;
    }

    void read()
    {
        MutexLocker<Mutex> m(_mutex);
        clearBuf(BUFFER_SIZE);
        readAll(_buffer);
    }

    const char *buffer()
    {
        MutexLocker<Mutex> m(_mutex);
        return _buffer;
    }

    ~UartSerial()
    {
        if (_buffer)
            delete[] _buffer;
    }
};
