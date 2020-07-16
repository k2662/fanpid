#include "pid.hpp"
#include "util.hpp"

PIDController::PIDController(double setpoint, double kp, double ki, double kd)
  : setpoint(setpoint),
    KP(kp),
    KI(ki),
    KD(kd),
    err(0.0),
    perr(0.0),
    d_err1(0.0),
    d_err2(0.0),
    pca(0.0)
{
  gettimeofday(&this->before, nullptr);
}

PIDController::PIDController(ControlConfig config)
  : setpoint(config.setpoint),
    KP(config.kp),
    KI(config.ki),
    KD(config.kd),
    err(0.0),
    perr(0.0),
    d_err1(0.0),
    d_err2(0.0),
    pca(0.0)
{
  gettimeofday(&this->before, nullptr);
}

void PIDController::update(ControlConfig config)
{
  this->KP = config.kp;
  this->KI = config.ki;
  this->KD = config.kd;
  this->setpoint = config.setpoint;
}

double PIDController::get_ca(double measurement)
{
  gettimeofday(&this->after, nullptr);

  double dt = get_dt(this->before, this->after);
  this->before = this->after;

  this->perr = this->err;
  this->err = measurement - this->setpoint;

  this->d_err2 = this->d_err1;
  this->d_err1 = this->err - this->perr;

  double rv = this->pca;

  if (this->KP)
    rv += this->KP*this->d_err1;

  if (this->KI)
    rv += dt*this->KI*this->err;

  if (this->KD)
    rv += this->KD*(this->d_err1 - this->d_err2)/dt;

  this->pca = rv;
  return rv;
}

void PIDController::set_setpoint(double setpoint)
{
  this->setpoint = setpoint;
}
