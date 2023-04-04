#pragma once

#include "models/manipulator.h"
#include "core/executionEnivroment.h"
#include "models/Command_t.h"
#include "interfaces/IExecutableCommand.h"
#include "core/executeState.h"
#include <bitset>

namespace executableCommands
{
    // Choose file and start execution
    // Arg = (string) file name
    struct m32 : public IExecutableCommand
    {
    public:
        ::ExecuteState_t started(core::ExecutionEnivroment &m, Command_t &args) override;
        ::ExecuteState_t execute(core::ExecutionEnivroment &, TimeDif_ms_t time) override;
        void ended() override;

        static IExecutableCommand *generate()
        {
            return new m32();
        }

        constexpr static const char *key()
        {
            return "M32";
        }
    };

    // files list
    struct m20 : public IExecutableCommand
    {
    public:
        ::ExecuteState_t started(core::ExecutionEnivroment &m, Command_t &args) override;
        ::ExecuteState_t execute(core::ExecutionEnivroment &, TimeDif_ms_t time) override;
        void ended() override;

        static IExecutableCommand *generate()
        {
            return new m20();
        }

        constexpr static const char *key()
        {
            return "M20";
        }
    };

    // enable sdcard
    struct m21 : public IExecutableCommand
    {
    public:
        ::ExecuteState_t started(core::ExecutionEnivroment &m, Command_t &args) override;
        ::ExecuteState_t execute(core::ExecutionEnivroment &, TimeDif_ms_t time) override;
        void ended() override;

        static IExecutableCommand *generate()
        {
            return new m21();
        }

        constexpr static const char *key()
        {
            return "M21";
        }
    };

    // disable sdcard
    struct m22 : public IExecutableCommand
    {
    public:
        ::ExecuteState_t started(core::ExecutionEnivroment &m, Command_t &args) override;
        ::ExecuteState_t execute(core::ExecutionEnivroment &, TimeDif_ms_t time) override;
        void ended() override;

        static IExecutableCommand *generate()
        {
            return new m22();
        }

        constexpr static const char *key()
        {
            return "M22";
        }
    };

    // Enable actuators
    struct m17 : public IExecutableCommand
    {
    private:
        TimeDif_ms_t _pause{0};
        TimeDif_ms_t _last{0};
        std::bitset<DOF + 1> _act;
        uint8_t _pos{0};
    public:
        ::ExecuteState_t started(core::ExecutionEnivroment &m, Command_t &args) override;
        ::ExecuteState_t execute(core::ExecutionEnivroment &, TimeDif_ms_t time) override;
        void ended() override;

        static IExecutableCommand *generate()
        {
            return new m17();
        }

        constexpr static const char *key()
        {
            return "M17";
        }
    };

    // disable actuators
    struct m18 : public IExecutableCommand
    {
    public:
        ::ExecuteState_t started(core::ExecutionEnivroment &m, Command_t &args) override;
        ::ExecuteState_t execute(core::ExecutionEnivroment &, TimeDif_ms_t time) override;
        void ended() override;

        static IExecutableCommand *generate()
        {
            return new m18();
        }

        constexpr static const char *key()
        {
            return "M18";
        }
    };

    // RED BUTTON
    struct m112 : public IExecutableCommand
    {
    public:
        ::ExecuteState_t started(core::ExecutionEnivroment &m, Command_t &args) override;
        ::ExecuteState_t execute(core::ExecutionEnivroment &, TimeDif_ms_t time) override;
        void ended() override;

        static IExecutableCommand *generate()
        {
            return new m112();
        }

        constexpr static const char *key()
        {
            return "M112";
        }
    };

    // Pause execution
    struct m25 : public IExecutableCommand
    {
    public:
        ::ExecuteState_t started(core::ExecutionEnivroment &m, Command_t &args) override;
        ::ExecuteState_t execute(core::ExecutionEnivroment &, TimeDif_ms_t time) override;
        void ended() override;

        static IExecutableCommand *generate()
        {
            return new m25();
        }

        constexpr static const char *key()
        {
            return "M25";
        }
    };

    // Start or resume execution
    struct m24 : public IExecutableCommand
    {
    public:
        ::ExecuteState_t started(core::ExecutionEnivroment &m, Command_t &args) override;
        ::ExecuteState_t execute(core::ExecutionEnivroment &, TimeDif_ms_t time) override;
        void ended() override;

        static IExecutableCommand *generate()
        {
            return new m24();
        }

        constexpr static const char *key()
        {
            return "M24";
        }
    };

    // Choose file
    struct m23 : public IExecutableCommand
    {
    public:
        ::ExecuteState_t started(core::ExecutionEnivroment &m, Command_t &args) override;
        ::ExecuteState_t execute(core::ExecutionEnivroment &, TimeDif_ms_t time) override;
        void ended() override;

        static IExecutableCommand *generate()
        {
            return new m23();
        }

        constexpr static const char *key()
        {
            return "M23";
        }
    };

}