#pragma once

#include "mbed.h"

class ServoGripper;

using MutexT = rtos::Mutex;
using ThreadT = rtos::Thread; 
using TimerT = mbed::Timer;
using DebugType_t = BufferedSerial;
using EffectorArgType = double;
