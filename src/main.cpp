#undef MBED_CONF_NSAPI_PRESENT

#include "mbed.h"
#include "global.h"

#include "core/core.h"
#include "core/coreTaskManager.h"

#include "models/dhTable.h"
#include "models/angleConverter.h"
#include "kinematics/kinematics.h"

#include "actuatorsLive.h"
#include "strParser.h"
#include "serial/uartSerial.h"
#include "out.h"
#include "gcodeLexer.h"
#include "sdCardManager.h"
#include "serial/fileSerial.h"
#include "periphery/buttons.h"

namespace debug_mess
{
#if __DEBUG_LEVEL >= __DEBUG_LEVEL_INFO
  constexpr static const char *START_INIT = "Start of initialization";
  constexpr static const char *INIT_DONE = "Initialization done";
  constexpr static const char *SD_INIT_RESULT = "Sd card init result: %i";
  constexpr static const char *SD_MOUNT_RESULT = "Sd card mount result: %i";
  constexpr static const char *READ_CONFIG_FILE = "Reading config file [%s]";
  constexpr static const char *READ_CONFIG_DONE = "Reading config file done";
  constexpr static const char *KINEMATICS_CALC_ERROR = "Kinematics calculation err";
#endif
};

ActuatorsLive<ThreadRegistrator> alive;
ThreadT athread;

core::ExecutionEnivroment env{
    .manipulator = Manipulator{
        .dhTable = {
            .theta = {0, degToRad(-90), 0, 0, 0, 0},
            .alfa = {degToRad(-90), 0, degToRad(-90), degToRad(90), degToRad(-90), 0},
            .d = {83.5, 0, 0, 105, 0, 100},
            .r = {0, 133, 10, 0, 0, 0},
        },
    },
};

ButtonsDriver _butDriver(env.manipulator);
ThreadT _butThread;

int main()
{

  DEBUG_INFO(debug_mess::START_INIT);

#if __USE_SDCARD == 1 && __AUTO_SD_INIT == 1
  int init_sd_result{0};
  sdCard.unmount();
  sdCard.deinit();

  init_sd_result = sdCard.init();
  DEBUG_INFO(debug_mess::SD_INIT_RESULT, init_sd_result);

  init_sd_result = sdCard.mount();
  DEBUG_INFO(debug_mess::SD_MOUNT_RESULT, init_sd_result);

  if (init_sd_result != 0)
  {
    init_sd_result = sdCard.format();
    DEBUG_INFO("Reformat result: %i", init_sd_result);
  }

#endif

  core::core.initializate();
  core::core.setWaitFunc(thread_sleep_for);

  core::ExecutionContext &context = core::coreTaskManager.getContext(core::CoreTaskKey::TASK_INPUT_UART);
  context.setEnivroment(env);

#if __USE_SDCARD == 1
  core::ExecutionContext &context_file = core::coreTaskManager.getContext(core::CoreTaskKey::TASK_INPUT_FILE);
  context_file.setEnivroment(env);
#endif

  core::ExecutionContext &context_buttons = core::coreTaskManager.getContext(core::CoreTaskKey::TASK_INPUT_BUTTONS);
  context_buttons.setEnivroment(env);

  _butDriver.initialize<ThreadRegistrator, ThreadT>(_butThread);

  DEBUG_INFO(debug_mess::READ_CONFIG_FILE, CONFIG_FILE_PATH);
  core::core.readConfigFile<ActuatorsLive<ThreadRegistrator>>(alive, context);
  alive.startThread<ThreadT>(athread);
  DEBUG_INFO(debug_mess::READ_CONFIG_DONE);

  core::core.initJoints(context);

  kinematics::kinematics6dof.setDHTable(env.manipulator.dhTable);
  std::vector<double> angles(DOF);
  for (uint8_t i = 0; i < DOF; i++)
    angles[i] = degToRad(env.manipulator.joints[i].get()->position());
  auto result = kinematics::kinematics6dof.forward(angles, env.manipulator.effectorPos);
  if (result == kinematics::CalculationResult_t::CALC_ERROR)
    DEBUG_ERR(debug_mess::KINEMATICS_CALC_ERROR);

#if __DEBUG_LEVEL >= __DEBUG_LEVEL_VERBOSE
  debug_out << "Position: ";
  debug_out << env.manipulator.effectorPos.x << '\t'
            << env.manipulator.effectorPos.y << '\t'
            << env.manipulator.effectorPos.z << '\t'
            << env.manipulator.effectorPos.a << '\t'
            << env.manipulator.effectorPos.b << '\t'
            << env.manipulator.effectorPos.g;
  debug_out << '\n';
#endif
  context.setInput(debug_out);
  context.setOutput(debug_out);

  DEBUG_INFO(debug_mess::INIT_DONE);

  core::coreTaskManager.startExecutionInThread(core::CoreTaskKey::TASK_INPUT_UART, core::BehaviourFlags::DEHAVIOUR_WAIT_INPUT);
  while (true)
  {
    ThisThread::yield();
  }
}
