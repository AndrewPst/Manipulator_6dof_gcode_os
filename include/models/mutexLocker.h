#pragma once

template <typename _mT>
class MutexLocker
{
public:
    MutexLocker(_mT &m, bool unlock = true) : _m(&m), _unlock(unlock)
    {
        lock();
    }

    void lock();
    void unlock();
    bool tryLock();

    _mT *mutex() const
    {
        return _m;
    }

    ~MutexLocker()
    {
        if (_unlock)
            unlock();
    }

private:
    _mT *_m;
    bool _unlock;
};
