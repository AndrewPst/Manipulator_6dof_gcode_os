#pragma once

#include "defines.h"

#include "core/executeState.h"
#include "core/executionEnivroment.h"
#include "interfaces/ISerial.h"


namespace core
{

struct ExecutionContext
{
private:
    ISerial *_input{nullptr};
    ISerial *_output{nullptr};

    ExecuteState_t _state{ExecuteState_t::STATE_FINISHED};

    ExecutionEnivroment* _enivroment;

    mutable MutexT _stateMut;
    mutable MutexT _inputMut;
    mutable MutexT _outputMut;
    mutable MutexT _enivMut;

public:
    void setOutput(ISerial &s);
    ISerial &output() const;

    void setEnivroment(ExecutionEnivroment&);
    ExecutionEnivroment& enivroment();

    void setInput(ISerial &s);
    ISerial &input() const;

    void setState(core::ExecuteState_t state);
    core::ExecuteState_t state() const;
};

}
