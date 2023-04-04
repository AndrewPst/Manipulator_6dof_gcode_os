#ifndef MBED_SERVO_H
#define MBED_SERVO_H

#include "interfaces/iActuatorDriver.h"

#include <mbed.h>

#define _USE_MUTEX

constexpr const static int8_t AUTO_WAIT_RESOLUTION = 11;

#ifdef _USE_MUTEX
#include <rtos.h>
#endif

typedef struct
{
    const uint32_t *_pulseTable;
    uint8_t resolution{1};
} PulseTable_t;

const static uint32_t default_pulse[] = {500, 1500, 2500};
const static PulseTable_t default_table = {default_pulse, sizeof(default_pulse) / sizeof(default_pulse[0])};

class ServoDriver : public IActuatorDriver
{

public:
    ServoDriver(PinName pin, const PulseTable_t & = default_table);
    ServoDriver(PinName pin, double min, double max, const PulseTable_t & = default_table);

    void initializate(double angle);

    void setTable(const PulseTable_t &);
    const PulseTable_t &table() const;

    void setMin(double);
    double min() const;

    void setMax(double);
    double max() const;

    void setValue(double) override;
    double value() const override;

    void setAngleBySpeed(double, double);

    void setMicroseconds(int32_t);

    bool isRotation() const;

    void setInverse(bool);
    bool isInverse() const;

    void setPause(bool);

    void wait();
    void tick(TimeDiff_ms_t) override;

    void disable();
    void enable();
    void stop();

protected:
    PwmOut _pwm;
    double _min{0.0};
    double _max{180.0};
    double _angle{0.0};
    PulseTable_t _table;

    double _speed{0};
    double _target{0};
    int8_t _dir{1};
    bool _isActiveAction = false;

    bool _isInverse{false};
    bool _isPause{false};

    const uint32_t _driveStoke{180};

    //Timer timer;
    //TimeDiff_ms_t _move_elapsed_time{0};

#ifdef _USE_MUTEX
    mutable Mutex _mutex;
#endif
};

#endif