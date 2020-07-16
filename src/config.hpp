#pragma once

struct ControlConfig {
  double kp, ki, kd;
  double setpoint;
  double time_delay;
};

ControlConfig load_config();
void save_config(ControlConfig config);
