#undef MBED_CONF_NSAPI_PRESENT

#include "mbed.h"
#include "global.h"

#include "core/core.h"
#include "core/coreTaskManager.h"

#include "actuatorsLive.h"
#include "strParser.h"
#include "serial/uartSerial.h"
#include "out.h"
#include "gcodeLexer.h"
#include "sdCardManager.h"
#include "serial/fileSerial.h"

namespace debug_mess
{
#if __DEBUG_LEVEL >= __DEBUG_LEVEL_INFO
  constexpr static const char *START_INIT = "Start of initialization";
  constexpr static const char *INIT_DONE = "Initialization done";
  constexpr static const char *SD_INIT_RESULT = "Sd card init result: %i";
  constexpr static const char *SD_MOUNT_RESULT = "Sd card mount result: %i";
  constexpr static const char *READ_CONFIG_FILE = "Reading config file [%s]";
  constexpr static const char *READ_CONFIG_DONE = "Reading config file done";
#endif
};

ActuatorsLive alive;
Thread athread;

core::ExecutionEnivroment env;

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

#endif

  core::core.initializate();
  core::core.setWaitFunc(thread_sleep_for);

  core::ExecutionContext &context = core::coreTaskManager.getContext(core::CoreTaskKey::TASK_INPUT_UART);
  context.setEnivroment(env);

#if __USE_SDCARD == 1
  core::ExecutionContext &context_file = core::coreTaskManager.getContext(core::CoreTaskKey::TASK_INPUT_FILE);
  context_file.setEnivroment(env);
#endif

  DEBUG_INFO(debug_mess::READ_CONFIG_FILE, CONFIG_FILE_PATH);
  core::core.readConfigFile<ActuatorsLive>(alive, context);
  alive.startThread<ThreadT>(athread);
  DEBUG_INFO(debug_mess::READ_CONFIG_DONE);

  core::core.initJoints(context);

  context.setInput(debug_out);
  context.setOutput(debug_out);

  DEBUG_INFO(debug_mess::INIT_DONE);

  core::coreTaskManager.startExecutionInThread(core::CoreTaskKey::TASK_INPUT_UART, core::BehaviourFlags::DEHAVIOUR_WAIT_INPUT);
  while (true)
  {
    ThisThread::yield();
  }
}
