#pragma once

#include "mbed.h"
#include "rtos.h"

#include "defines.h"

#include "core/behaviourFlags.h"
#include "core/executeState.h"
#include "core/executionContext.h"

#include "commandExecutors/g_group.h"
#include "commandExecutors/m_group.h"

#include "interfaces/ISerial.h"
#include "interfaces/IExecutableCommand.h"
#include "models/manipulator.h"
#include "models/Command_t.h"
#include "models/cmp_str.h"
#include "models/mutexLocker.h"

#include "actuators/gripper.h"

#include <memory>
#include <map>

//----------Servo tables (TEMP)---------------
#include <actuators/servo.h>
#include <actuators/servoJoint.h>
const static uint32_t _pulse1[] = {620, 1520, 2440};
const static PulseTable_t _table1 = {_pulse1, sizeof(_pulse1) / sizeof(_pulse1[0])};

const static uint32_t _pulse2[] = {600, 1510, 2460};
const static PulseTable_t _table2 = {_pulse2, sizeof(_pulse2) / sizeof(_pulse2[0])};

const static uint32_t _pulse3[] = {620, 1525, 2440};
const static PulseTable_t _table3 = {_pulse3, sizeof(_pulse3) / sizeof(_pulse3[0])};

const static uint32_t _pulse4[] = {542, 1480, 2380};
const static PulseTable_t _table4 = {_pulse4, sizeof(_pulse4) / sizeof(_pulse4[0])};

namespace core
{
    typedef IExecutableCommand *(*ExecutorGenerator)();
    typedef void (*WaitFunction)(uint32_t);

    class Core
    {
    private:
        constexpr static const uint8_t DEBUG_BUFFER_SIZE = 128;
        constexpr static const uint8_t INPUT_BUFFER_SIZE = 64;
        constexpr static const uint8_t OUTPUT_BUFFER_SIZE = 128;

        constexpr static const char INPUT_SEPARATOR = '\n';

        std::unique_ptr<char[]> _debug_buf = std::unique_ptr<char[]>(new char[DEBUG_BUFFER_SIZE]);

        WaitFunction _waitFunc;

        mutable MutexT _stateMut;
        mutable MutexT _executorsMut;
        mutable MutexT _contextMapMut;

        const std::map<const char *, core::ExecutorGenerator, cmp_str> _avaiableExecutors{
            std::make_pair(::executableCommands::g0::key(), ::executableCommands::g0::generate),
            std::make_pair(::executableCommands::g1::key(), ::executableCommands::g1::generate),
            std::make_pair(::executableCommands::g5::key(), ::executableCommands::g5::generate),
            std::make_pair(::executableCommands::g4::key(), ::executableCommands::g4::generate),
            std::make_pair(::executableCommands::g6::key(), ::executableCommands::g6::generate),
            std::make_pair(::executableCommands::m32::key(), ::executableCommands::m32::generate),
            std::make_pair(::executableCommands::m21::key(), ::executableCommands::m21::generate),
            std::make_pair(::executableCommands::m22::key(), ::executableCommands::m22::generate),
            std::make_pair(::executableCommands::m17::key(), ::executableCommands::m17::generate),
            std::make_pair(::executableCommands::m18::key(), ::executableCommands::m18::generate),
            std::make_pair(::executableCommands::m112::key(), ::executableCommands::m112::generate),
            std::make_pair(::executableCommands::m25::key(), ::executableCommands::m25::generate),
            std::make_pair(::executableCommands::m24::key(), ::executableCommands::m24::generate),
            std::make_pair(::executableCommands::m23::key(), ::executableCommands::m23::generate),
        };

    public:
        Core();

        void initializate();

        void initJoints(ExecutionContext &);

        void setWaitFunc(WaitFunction w);

        WaitFunction waitFunc() const;

        template <typename _actRegT>
        uint8_t readConfigFile(_actRegT &actRegistrator, ExecutionContext &context)
        {
            context.enivroment().manipulator.joints.push_back(std::unique_ptr<ServoJoint>(new ServoJoint({&(actRegistrator.template registerActuator<ServoDriver>(D2))})));

            ServoDriver *servo = &(actRegistrator.template registerActuator<ServoDriver>(D3, _table1));
            servo->setInverse(true);
            context.enivroment().manipulator.joints.push_back(std::unique_ptr<ServoJoint>(new ServoJoint({servo, &(actRegistrator.template registerActuator<ServoDriver>(D4))})));

            servo = &(actRegistrator.template registerActuator<ServoDriver>(D5, _table3));
            servo->setMax(155);
            context.enivroment().manipulator.joints.push_back(std::unique_ptr<ServoJoint>(new ServoJoint({servo})));

            context.enivroment().manipulator.joints.push_back(std::unique_ptr<ServoJoint>(new ServoJoint({&(actRegistrator.template registerActuator<ServoDriver>(D6))})));

            servo = &(actRegistrator.template registerActuator<ServoDriver>(D7, _table4));
            servo->setInverse(true);
            context.enivroment().manipulator.joints.push_back(std::unique_ptr<ServoJoint>(new ServoJoint({servo})));

            context.enivroment().manipulator.joints.push_back(std::unique_ptr<ServoJoint>(new ServoJoint({&(actRegistrator.template registerActuator<ServoDriver>(D8))})));

            servo = &(actRegistrator.template registerActuator<ServoDriver>(PC_8));
            context.enivroment().manipulator.effector = std::unique_ptr<ServoGripper>(new ServoGripper(*servo));

            return 0;
        }

        void execute(Command_t &command, BehaviourFlag_t flag, ExecutionContext &context);

        template <typename _DataLineT>
        void startExecution(BehaviourFlag_t flag, ExecutionContext &context)
        {
            DEBUG_INFO("Execution started");
            _DataLineT line;
            std::unique_ptr<char[]> buffer = std::unique_ptr<char[]>(new char[INPUT_BUFFER_SIZE]);
            context.setState(core::ExecuteState_t::STATE_RUNNING);
            while (context.state() != core::ExecuteState_t::STATE_FINISHED)
            {
                if (context.input().readable() == false)
                {
                    if (flag & BehaviourFlags::DEHAVIOUR_WAIT_INPUT)
                        _waitFunc(1);
                    else
                        context.setState(core::ExecuteState_t::STATE_FINISHED);
                    continue;
                }
                size_t size = context.input().readUntil(buffer.get(), INPUT_BUFFER_SIZE, INPUT_SEPARATOR);
                Command_t command{0};
                uint8_t result = line.parse(buffer.get(), command);

#if __DEBUG_LEVEL >= __DEBUG_LEVEL_VERBOSE && __PRINT_PARSED_INPUT == 1
                debug_out << "Executed: " << command.Key << ':' << '\t';
                for (auto i = command.args.begin(); i != command.args.end(); i++)
                {
                    debug_out << *i << '\t';
                }
                debug_out << '\n';
#endif
                if (result != 0)
                {
                    DEBUG_ERR("[%s]-parse error\n", buffer.get())
                    if (flag & BehaviourFlags::DEHAVIOUR_FINISH_PROGRAM_IF_PARSE_ERROR)
                        context.setState(core::ExecuteState_t::STATE_FINISHED);
                    continue;
                }

                execute(command, flag, context);
                memset(buffer.get(), 0, size);
            }
            DEBUG_INFO("Execution ended");
        }
    };
}
