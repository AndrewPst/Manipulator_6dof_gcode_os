#include "serial/fileSerial.h"

FileSerial::FileSerial(FILE *f, bool close_on_destroy) : _file(f), _close_on_destroy(close_on_destroy)
{
    // MBED_ASSERT(f == NULL);
}

FileSerial::~FileSerial()
{
    if (_close_on_destroy)
        fclose(_file);
}

size_t FileSerial::size() const
{
    fpos_t _curPos = ftell(_file);
    fseek(_file, 0, SEEK_END);
    size_t size = ftell(_file);
    fseek(_file, _curPos, SEEK_SET);
    return size;
}

void FileSerial::seek(uint32_t add)
{
    fseek(_file, 0, SEEK_SET);
}

fpos_t FileSerial::position()
{
    fpos_t _curPos{0};
    fgetpos(_file, &_curPos);
    return _curPos;
}

bool FileSerial::readable()
{
    bool r = static_cast<size_t>(position()) < size();
    return r;
}

ISerial &FileSerial::operator<<(int value)
{
    fprintf(_file, "%i", value);
    return *this;
}

ISerial &FileSerial::operator<<(double value)
{
    fprintf(_file, "%g", value);
    return *this;
}

ISerial &FileSerial::operator<<(const std::string &str)
{
    fprintf(_file, "%s", str.c_str());
    return *this;
}

ISerial &FileSerial::operator<<(const char *str)
{
    fprintf(_file, "%s", str);
    return *this;
}

ISerial &FileSerial::operator<<(char c)
{
    fprintf(_file, "%c", c);
    return *this;
}

ISerial &FileSerial::operator>>(int &value)
{
    fscanf(_file, "%i", &value);
    return *this;
}

ISerial &FileSerial::operator>>(double &value)
{
    fscanf(_file, "%lf", &value);
    return *this;
}

ISerial &FileSerial::operator>>(std::string &str)
{
    char buf[BUFFER_SIZE];
    fscanf(_file, "%s", buf);
    str = std::string(buf);
    return *this;
}

ISerial &FileSerial::operator>>(char *str)
{
    fscanf(_file, "%s", str);
    return *this;
}

size_t FileSerial::readUntil(char *buffer, size_t size, char symbol)
{
    size_t i = 0;
    while (i < size - 1)
    {
        int r = fgetc(_file);
        if (r == EOF)
            break;
        buffer[i] = r;
        if (buffer[i] == symbol)
            break;
        i++;
    }
    buffer[i] = '\0';
    return i;
}
