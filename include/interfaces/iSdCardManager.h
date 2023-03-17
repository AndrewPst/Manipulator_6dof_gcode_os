#pragma once

#include "mbed.h"
#include "inttypes.h"
#include "stdio.h"

class ISdCardManager
{
public:

    virtual int32_t init() = 0;
    virtual int32_t deinit() = 0;

    virtual int32_t mount() = 0;
    virtual int32_t unmount() = 0;

    virtual int32_t format() = 0;

    virtual size_t size() = 0;

    virtual FILE* openFile(const char*, const char*) = 0;
    virtual int32_t closeFile(FILE*) = 0;

    virtual DIR* openDir(const char*) = 0;
    virtual int32_t closeDir(DIR*) = 0;

};