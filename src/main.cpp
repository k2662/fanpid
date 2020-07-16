#include <iostream>
#include <atomic>
#include <signal.h>

#include <smcpp/smc.hpp>

#include "config.hpp"
#include "fan.hpp"
#include "pid.hpp"

std::atomic<bool> running(true);

void sighand(int signo)
{
  (void) signo;
  running.store(false);

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

  std::cerr << "Control begun!" << std::endl;
  while (running.load()) {
    double cpu_temp = 0.0;
    cpu_temp += smc.read("TC1C");
    cpu_temp += smc.read("TC2C");
    cpu_temp += smc.read("TC3C");
    cpu_temp += smc.read("TC4C");
    cpu_temp /= 4.0;
    double fan_speed = pidc.get_ca(cpu_temp);

    double f0_target = f0.set_target(fan_speed);
    double f1_target = f1.set_target(fan_speed);
    std::cerr
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
