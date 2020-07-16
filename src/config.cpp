#include <fstream>
#include <sstream>
#include <iostream>

#include "config.hpp"

static const std::string config_path = ".fanpid_config";

ControlConfig default_config()
{
  ControlConfig config = {
    .kp = 1.0,
    .ki = 1.0,
    .kd = 0.0,
    .setpoint = 50.0,
    .time_delay = 1.0
  };
  return config;
}

void safe_parse_value_to_double_for_key(const std::string &value, const std::string &key, double &d)
{
  try {
    d = std::stof(value);
  }
  catch (std::exception) {
    std::cerr << "Error reading double value '" << value << "' for key '" << key << "': ignoring." << std::endl;
  }
}

ControlConfig load_config()
{

  std::ifstream ifs(config_path);

  ControlConfig config = default_config();
  if (ifs.fail()) {
    return config;
  }

  std::string line;
  while (std::getline(ifs, line)) {
    std::stringstream ss(line);
    std::string key, value;

    std::getline(ss, key, ':');
    std::getline(ss, value, ':');

    if (key.compare("kp") == 0) {
      safe_parse_value_to_double_for_key(value, key, config.kp);
    }
    else if (key.compare("ki") == 0) {
      safe_parse_value_to_double_for_key(value, key, config.ki);
    }
    else if (key.compare("kd") == 0) {
      safe_parse_value_to_double_for_key(value, key, config.kd);
    }
    else if (key.compare("setpoint") == 0) {
      safe_parse_value_to_double_for_key(value, key, config.setpoint);
    }
    else if (key.compare("time_delay") == 0) {
      safe_parse_value_to_double_for_key(value, key, config.time_delay);
    }
    else {
      std::cerr << "Unknown key '" << key << "' ignoring." << std::endl;
    }
  }

  return config;
}

void save_config(ControlConfig config)
{
  ControlConfig defaults = default_config();

  std::ofstream ofs(config_path);
  if (ofs.fail()) {
    std::cerr << "Could not write config out to file." << std::endl;
  }

  if (config.kp != defaults.kp) {
    ofs << "kp:" << config.kp << std::endl;
  }

  if (config.ki != defaults.ki) {
    ofs << "ki:" << config.ki << std::endl;
  }

  if (config.kd != defaults.kd) {
    ofs << "kd:" << config.kd << std::endl;
  }

  if (config.setpoint != defaults.setpoint) {
    ofs << "setpoint:" << config.setpoint << std::endl;
  }

  if (config.time_delay != defaults.time_delay) {
    ofs << "time_delay:" << config.time_delay << std::endl;
  }
}
