#include "core/executionContext.h"

#include "models/mutexLocker.h"

using namespace core;

void ExecutionContext::setOutput(ISerial &s)
{
    MutexLocker<MutexT> lock(_outputMut);
    _output = &s;
}

ISerial &ExecutionContext::output() const
{
    MutexLocker<MutexT> lock(_outputMut);
    return *_output;
}

void ExecutionContext::setEnivroment(ExecutionEnivroment &e)
{
    MutexLocker<MutexT> lock(_enivMut);
    _enivroment = &e;
}

ExecutionEnivroment &ExecutionContext::enivroment()
{
    MutexLocker<MutexT> lock(_enivMut);
    return *_enivroment;
}

void ExecutionContext::setInput(ISerial &s)
{
    MutexLocker<MutexT> lock(_inputMut);
    _input = &s;
}

ISerial &ExecutionContext::input() const
{
    MutexLocker<MutexT> lock(_inputMut);
    return *_input;
}

void ExecutionContext::setState(core::ExecuteState_t state)
{
    MutexLocker<MutexT> lock(_stateMut);
    _state = state;
}
core::ExecuteState_t ExecutionContext::state() const
{
    MutexLocker<MutexT> lock(_stateMut);
    return _state;
}