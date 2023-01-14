#include "out.h"

DebugType_t s(DEBUG_PORT_TX, DEBUG_PORT_RX, DEBUG_SERIAL_BOUD);
UartSerial<DebugType_t> debug_out(s);