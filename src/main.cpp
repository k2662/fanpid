#include <iostream>
#include <atomic>
#include <signal.h>

#include <smcpp/smc.hpp>

#include "config.hpp"
#include "fan.hpp"
#include "pid.hpp"

std::atomic<bool> running(true), reload_config(false), control_on(true);

void sighand(int signo)
{
  switch (signo) {
    case SIGINT:
      running.store(false);
      break;

    case SIGUSR1:
      reload_config.store(true);
      break;

    case SIGUSR2:
      std::cerr << "Control toggled" << std::endl;
      control_on.store(!control_on.load());
      break;
  }
}

int main(void)
{
  std::cerr << "Setting up controller." << std::endl;
  SMC smc;

  auto config = load_config();

  //PIDController pidc(50.0, 500.0, 0.0, 0.0);
  PIDController pidc(config);

  Fan f0(0), f1(1);

  try {
    f0.init_control();
    f1.init_control();
  }
  catch (const SMCError &e) {
    std::cerr << "Could not set control mode of fans: are you running as root?" << std::endl;
    exit(1);
  }

  signal(SIGINT, sighand);
  signal(SIGUSR1, sighand);
  signal(SIGUSR2, sighand);

  std::cerr << "Control begun!" << std::endl;
  while (running.load()) {

    bool control_mode = control_on.load();

    if (reload_config.load()) {
      reload_config.store(false);
      config = load_config();
      pidc.update(config);
      std::cerr << "Config reloaded!" << std::endl;
    }

    f0.set_control_mode(control_mode);
    f1.set_control_mode(control_mode);

    double cpu_temp = 0.0;
    cpu_temp += smc.read("TC1C");
    cpu_temp += smc.read("TC2C");
    cpu_temp += smc.read("TC3C");
    cpu_temp += smc.read("TC4C");
    cpu_temp /= 4.0;
    double fan_speed = pidc.get_ca(cpu_temp);

    double f0_target = f0.set_target(fan_speed);
    double f1_target = f1.set_target(fan_speed);
    std::cout
      << cpu_temp << ", "
      << double(smc.read("F0Ac")) << ", "
      << double(smc.read("F1Ac")) << ", "
      << f0_target << ", "
      << f1_target << ", "
      << std::endl;

    usleep(2000000);
  }

  save_config(config);
  std::cerr << "Exiting..." << std::endl;
}
