#include "fan.hpp"

Fan::Fan(int index)
  : selector(char(index+48)), control_mode(false)
{

  // Get this fan's minimum RPM
  char key[5] = {'F', selector, 'M', 'n'};
  this->min = this->smc.read(key);

  // Get this fan's maximum RPM
  key[3] = 'x';
  this->max = this->smc.read(key);
}

Fan::~Fan()
{
  // Set F0Md -> 0
  // Set fan control mode back to auto mode
  this->set_control_mode(false);
}

void Fan::init_control()
{
  this->set_control_mode(true);
}

void Fan::set_control_mode(bool value)
{
  if (value == this->control_mode)
    return;

  char key[5] = {'F', this->selector, 'M', 'd'};

  try {
    // Set control mode of the fan: 0 is auto, 1 is manual.
    this->smc.write(key, int(value));
  }
  catch (const SMCError &e) {
    throw SMCError("Could not set control mode: are you running as root?");
  }

  this->control_mode = value;
}

void Fan::set_target(double target)
{
  if (target < this->min)
    target = this->min;

  if (target > this->max)
    target = this->max;

  this->target = target;

  char key[5] = {'F', selector, 'T', 'g'};
  this->smc.write(key, target);
}
