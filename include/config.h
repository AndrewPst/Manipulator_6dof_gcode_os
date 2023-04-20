#pragma once


#include "PinNames.h"

constexpr const int DOF = 6;

//Debug params
constexpr const int DEBUG_SERIAL_BOUD = 9600;

//Debug serial pins
constexpr const PinName DEBUG_PORT_TX = USBTX;
constexpr const PinName DEBUG_PORT_RX = USBRX;
//buttons pins
constexpr const PinName STOP_BUTTON_PIN = PB_2;
constexpr const PinName REINIT_BUTTON_PIN = PB_12;

//Path to config file(currently not supported)
constexpr static const char* CONFIG_FILE_PATH = "/sd/config.json";

#define __USE_SDCARD 1

//Mount sd card at startup
#define __AUTO_SD_INIT 0

//Output additional information to the executed command
#define __PRINT_PARSED_INPUT 0

#define __DEBUG_LEVEL_NONE 0
#define __DEBUG_LEVEL_INFO 1
#define __DEBUG_LEVEL_ERRORS 2
#define __DEBUG_LEVEL_VERBOSE 3

//Debug level
#define __DEBUG_LEVEL __DEBUG_LEVEL_VERBOSE