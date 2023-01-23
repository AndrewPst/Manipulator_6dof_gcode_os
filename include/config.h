#pragma once

constexpr const int DOF = 6;

//Debug params
constexpr const int DEBUG_SERIAL_BOUD = 9600;
constexpr const PinName DEBUG_PORT_TX = USBTX;
constexpr const PinName DEBUG_PORT_RX = USBRX;


constexpr static const char* CONFIG_FILE_PATH = "/sd/config.json";

#define __USE_SDCARD 1

#define __AUTO_SD_INIT 1

#define __PRINT_PARSED_INPUT 0


#define __DEBUG_LEVEL_NONE 0
#define __DEBUG_LEVEL_INFO 1
#define __DEBUG_LEVEL_ERRORS 2
#define __DEBUG_LEVEL_VERBOSE 3

#define __DEBUG_LEVEL __DEBUG_LEVEL_VERBOSE