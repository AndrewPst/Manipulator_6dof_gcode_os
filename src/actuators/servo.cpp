#include "actuators/servo.h"

#include "models/mutexLocker.h"
#include "out.h"

#ifdef _USE_MUTEX
#define MUTEX MutexLocker<Mutex> ml(_mutex);
#define MUTEX_LOCK _mutex.lock();
#define MUTEX_UNLOCK _mutex.unlock();
#else
#define MUTEX
#define MUTEX_LOCK
#define MUTEX_UNLOCK
#endif

#define constrain(a, b, c) (((a) < (b)) ? (b) : ((a) > (c)) ? (c) \
                                                          : (a))
#define sign(x) ((x) >= 0 ? 1 : -1)

ServoDriver::ServoDriver(PinName pin, const PulseTable_t &table) : _pwm(pin), _table(table)
{
}

ServoDriver::ServoDriver(PinName pin, double min, double max, const PulseTable_t &table)
    : _pwm(pin), _min(min), _max(max), _table(table)
{
}

void ServoDriver::setMicroseconds(int32_t value)
{
    MUTEX
    _pwm.pulsewidth_us(value);
}

void ServoDriver::setValue(double angle)
{
    MUTEX_LOCK
    angle = constrain(angle, _min, _max);
    _angle = angle;
    int16_t i1 = (uint32_t)(angle - _min) / (_driveStoke / (_table.resolution - 1));
    uint32_t ticks = 0;
    int16_t i2 = (i1 + 1) >= _table.resolution ? i1 : i1 + 1;
    if (i1 == i2)
    {
        ticks = _table._pulseTable[i2];
    }
    else
    {
        double x1 = (_driveStoke) / (_table.resolution - 1) * i1;
        double x2 = (_driveStoke) / (_table.resolution - 1) * i2;
        ticks = _table._pulseTable[i1] + (_table._pulseTable[i2] - _table._pulseTable[i1]) * (angle - x1) / (x2 - x1);
    }
    if (_isInverse)
        ticks = _table._pulseTable[_table.resolution - 1] - ticks + _table._pulseTable[0];
    MUTEX_UNLOCK
    setMicroseconds(ticks);
}

double ServoDriver::value() const
{
    MUTEX
    return _angle;
}

void ServoDriver::setTable(const PulseTable_t &table)
{
    MUTEX
    _table = table;
}

const PulseTable_t &ServoDriver::table() const
{
    MUTEX
    return _table;
}

void ServoDriver::setMin(double v)
{
    MUTEX
    _min = v;
}

double ServoDriver::min() const
{
    MUTEX
    return _min;
}

void ServoDriver::setMax(double v)
{
    MUTEX
    _max = v;
}

double ServoDriver::max() const
{
    MUTEX
    return _max;
}

bool ServoDriver::isRotation() const
{
    MUTEX
    return _isActiveAction;
}

void ServoDriver::setInverse(bool isinverse)
{
    MUTEX
    _isInverse = isinverse;
}

bool ServoDriver::isInverse() const
{
    MUTEX
    return _isInverse;
}

void ServoDriver::tick(TimeDiff_ms_t timediff)
{
    MUTEX_LOCK
    if (_isActiveAction == false || _isPause == true)
    {
        MUTEX_UNLOCK
        return;
    }
    //double timediff = std::chrono::duration_cast<std::chrono::milliseconds>(timer.elapsed_time()).count();
    double step = _speed * (double)timediff / 1000.0;
    double angle{0};
    if (sign(_target - (_angle + step * _dir)) != _dir || (_angle + step * _dir) > _max || (_angle + step * _dir) < _min)
    {
        _isActiveAction = false;
        angle = _target;
    }
    else
    {
        angle = _angle + step * _dir;
    }
    MUTEX_UNLOCK
    setValue(angle);
}

void ServoDriver::setPause(bool pause)
{
    MUTEX
    // if (_isPause == pause)
    //     return;
    _isPause = pause;
    // if (_isPause)
    //     timer.stop();
    // else
    //     timer.start();
}

void ServoDriver::setAngleBySpeed(double pos, double speed)
{
    MUTEX
    if (pos == _angle)
        return;
    _isActiveAction = true;
    _speed = speed;
    _target = pos;
    _dir = sign(_target - _angle);

    //_move_elapsed_time = 0;
    // timer.reset();
    // timer.start();
}

void ServoDriver::wait()
{
    // while (isRotation())
    // {
    //     tick();
    //     thread_sleep_for(AUTO_WAIT_RESOLUTION);
    // }
}

void ServoDriver::stop()
{
    MUTEX_LOCK
    _isActiveAction = false;
    // timer.stop();
    _target = _angle;
    MUTEX_UNLOCK
}

void ServoDriver::disable()
{
    stop();
    setMicroseconds(0);
}

void ServoDriver::enable()
{
    setValue(_angle);
}
