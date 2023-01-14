#pragma once

#include "interfaces/iSdCardManager.h"

#include "mbed.h"
#include "PinNames.h"
#include "inttypes.h"
#include "mbed_config.h"

#include "SDBlockDevice.h"
#include "FATFileSystem.h"
#include "stdio.h"


class SdCardManager : public ISdCardManager
{
private:
    SDBlockDevice *_sd;
    FATFileSystem *_fs;

public:
    SdCardManager();

    ~SdCardManager();

    int32_t init() override;
    int32_t deinit() override;

    int32_t mount() override;
    int32_t unmount() override;
    
    size_t size() override;
    FILE *openFile(const char *, const char *) override;
    int32_t closeFile(FILE *) override;
    DIR *openDir(const char *) override;
    int32_t closeDir(DIR *) override;

};