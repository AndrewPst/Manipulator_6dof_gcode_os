#include "periphery/buttons.h"
#include "out.h"
#include "core/coreTaskManager.h"

ButtonsDriver::ButtonsDriver(Manipulator &man) : _man(man)
{
}

void ButtonsDriver::updateButtonsStateCycle()
{
    if (_stopBut.is_connected() == false)
        DEBUG_ERR("Stop button not found");

    if (_reinitBut.is_connected() == false)
        DEBUG_ERR("Reinit button not found");

    uint32_t _timeDiff{0};
    while (true)
    {
        if (_timeDiff > 30)
        {
            if (_stopBut.read() == false)
            {
                _timeDiff = 0;
                stopButtTriggered();
                DEBUG_INFO(">Stop triggered<");
            } else if(_reinitBut.read() == false)
            {
                _timeDiff = 0;
                reinitButtTriggered();
                DEBUG_INFO(">Reinit triggered<");
            }
        }
        thread_sleep_for(10);
        _timeDiff += 1;
    }
}

void ButtonsDriver::stopButtTriggered()
{
    for (auto &j : _man.joints)
    {
        j->disable();
    }
    _man.effector->disable();
    core::coreTaskManager.getContext(core::CoreTaskKey::TASK_INPUT_FILE).setState(core::ExecuteState_t::STATE_FINISHED);
    core::coreTaskManager.getContext(core::CoreTaskKey::TASK_INPUT_UART).setState(core::ExecuteState_t::STATE_FINISHED);
}

void ButtonsDriver::reinitButtTriggered()
{
    // for (auto &j : _man.joints)
    // {
    //     j->enable();
    // }
    // _man.effector->enable();
    core::coreTaskManager.startExecutionInThread(core::CoreTaskKey::TASK_INPUT_UART, core::BehaviourFlags::DEHAVIOUR_WAIT_INPUT);
    // core::coreTaskManager.getContext(core::CoreTaskKey::TASK_INPUT_FILE).setState(core::ExecuteState_t::STATE_FINISHED);
    // core::coreTaskManager.getContext(core::CoreTaskKey::TASK_INPUT_UART).setState(core::ExecuteState_t::STATE_FINISHED);
}