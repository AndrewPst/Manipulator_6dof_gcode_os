#pragma once

#include "models/manipulator.h"
#include "core/executionEnivroment.h"
#include "models/Command_t.h"
#include "interfaces/IExecutableCommand.h"
#include "core/executeState.h"

namespace executableCommands
{
    struct g5 : public IExecutableCommand
    {
    private:
        double _exTime{0};
        static double _speed;

    public:
        ::ExecuteState_t started(core::ExecutionEnivroment &m, Command_t &args) override;
        ::ExecuteState_t execute(core::ExecutionEnivroment &, TimeDif_ms_t time) override;
        void ended() override;

        static IExecutableCommand *generate()
        {
            return new g5();
        }

        constexpr static const char *key()
        {
            return "G5";
        }
    };

    struct g6 : public IExecutableCommand
    {
    private:
        static double _exTime;
        uint32_t _allTime{0};

    public:
        ::ExecuteState_t started(core::ExecutionEnivroment &m, Command_t &args) override;
        ::ExecuteState_t execute(core::ExecutionEnivroment &, TimeDif_ms_t time) override;
        void ended() override;

        static IExecutableCommand *generate()
        {
            return new g6();
        }

        constexpr static const char *key()
        {
            return "G6";
        }
    };

    struct g4 : public IExecutableCommand
    {
    private:
        static double pause_time;

    public:
        ::ExecuteState_t started(core::ExecutionEnivroment &m, Command_t &args) override;
        ::ExecuteState_t execute(core::ExecutionEnivroment &, TimeDif_ms_t time) override;
        void ended() override;

        static IExecutableCommand *generate()
        {
            return new g4();
        }

        constexpr static const char *key()
        {
            return "G4";
        }
    };
}