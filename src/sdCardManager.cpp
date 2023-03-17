#include "sdCardManager.h"

#include "out.h"

SdCardManager::SdCardManager() : _sd(new SDBlockDevice()), _fs(new FATFileSystem("sd", _sd))
{
}

SdCardManager::~SdCardManager()
{
    deinit();
    unmount();
    delete _sd;
    delete _fs;
}

int32_t SdCardManager::init()
{
    int r = _sd->init();
    return r;
}

int32_t SdCardManager::format()
{
    return FATFileSystem::format(_sd);
}

int32_t SdCardManager::deinit()
{
    return _sd->deinit();
}

int32_t SdCardManager::mount()
{
    int r = _fs->mount(_sd);
    return r;
}

int32_t SdCardManager::unmount()
{
    return _fs->unmount();
}

size_t SdCardManager::size()
{
    return _sd->size();
}

FILE *SdCardManager::openFile(const char *path, const char *mode)
{
    return fopen(path, mode);
}

DIR *SdCardManager::openDir(const char *path)
{
    return opendir(path);
}

int32_t SdCardManager::closeFile(FILE *f)
{
    return fclose(f);
}

int32_t SdCardManager::closeDir(DIR *d)
{
    return closedir(d);
}