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

    int32_t format() override;
    
    size_t size() override;
    FILE *openFile(const char *, const char *) override;
    int32_t closeFile(FILE *) override;
    DIR *openDir(const char *) override;
    int32_t closeDir(DIR *) override;

    template<typename _outT>
    size_t printFiles(const char* dir, _outT& out)
    {
        DIR* dir_ = opendir(dir);
        dirent *de;
        size_t count{0};
        while((de = readdir(dir_)) != NULL)
        {
            out << &(de->d_name)[0] << '\n';
            ++count;
        }
        closedir(dir_);
        return count;
    }
};