#pragma once

#include "config.h"
#include "global.h"
#include "models/manipulator.h"

#include "mbed.h"

class ButtonsDriver
{

private:
    DigitalIn _stopBut = DigitalIn(STOP_BUTTON_PIN);
    DigitalIn _reinitBut = DigitalIn(REINIT_BUTTON_PIN);

    Manipulator &_man;

    void stopButtTriggered();
    void reinitButtTriggered();

    void updateButtonsStateCycle();

public:
    template <typename _threadRegT, typename _ThrT>
    void initialize(_ThrT &thread)
    {
        _stopBut.mode(PinMode::PullUp);
        _reinitBut.mode(PinMode::PullUp);
        _threadRegT::start(thread, [&]() -> void
                           { updateButtonsStateCycle(); });
    }

    ButtonsDriver(Manipulator &);
};