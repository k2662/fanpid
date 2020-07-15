#include <iostream>
#include <smcpp/smc.hpp>

#include "fan.hpp"
#include "pid.hpp"

int main(void)
{
  SMC smc;

  PIDController pidc(50.0, 500.0, 0.0, 0.0);

  Fan f0(0), f1(1);
  try {
    f0.init_control();
    f1.init_control();
  }
  catch (const SMCError &e) {
    std::cerr << "Could not set control mode of fans: are you running as root?" << std::endl;
  }

  while (true) {
    double cpu_temp = smc.read("TC0P");
    double fan_speed = pidc.get_ca(cpu_temp);

    std::cerr << cpu_temp << "ºC " << fan_speed << " RPM" << std::endl;

    f0.set_target(fan_speed);
    f1.set_target(fan_speed);
    usleep(1000000);
  }
}
