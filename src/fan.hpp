#pragma once
#include <smcpp/smc.hpp>

class Fan {
  private:
    SMC smc;
    char selector;
    double min, max, target;
    bool control_mode;


  public:
    Fan(int index);
    ~Fan();

    void init_control();
    void set_control_mode(bool value=true);
    double set_target(double value);
};
