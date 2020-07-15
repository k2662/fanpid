#include <iostream>
#include <atomic>
#include <signal.h>

#include <smcpp/smc.hpp>

#include "fan.hpp"
#include "pid.hpp"

std::atomic<bool> running(true);

void sighand(int signo)
{
  (void) signo;
  running.store(false);
}

double interp(double T, double Tlo, double Thi, double Wlo, double Whi)
{
  double m = (Whi - Wlo)/(Thi - Tlo);
  return Wlo + m*(T-Tlo);
}

int main(void)
{
  SMC smc;

  PIDController pidc(50.0, 500.0, 0.0, 0.0);

  std::cerr << "Starting up..." << std::endl;
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

  while (running.load()) {
    double cpu_temp = 0.0;
    cpu_temp += smc.read("TC1C");
    cpu_temp += smc.read("TC2C");
    cpu_temp += smc.read("TC3C");
    cpu_temp += smc.read("TC4C");
    cpu_temp /= 4.0;
    double fan_speed = interp(cpu_temp, 40, 80, 1000, 7000);
    //pidc.get_ca(cpu_temp);

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
}
