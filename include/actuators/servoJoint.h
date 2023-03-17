#pragma once

#include "interfaces/iJoint.h"
#include "actuators/servo.h"

class ServoJoint : public IJoint
{
private:
    const std::vector<ServoDriver *> _servo;

    double _offset{90.0};

public:
    ServoJoint(const ServoJoint &) = default;
    explicit ServoJoint(const std::vector<ServoDriver *> &servo);

    void initialize() override;

    void enable() override;
    void disable() override;
    void stop() override;

    double move(double pos, double speed) override;
    double position() const override;

    bool posAchievable(double) override;

    void setPause(bool p) override;
    JointState_t state() override;
};