#ifndef __CONTROLPID__H__DEFINED__
#define __CONTROLPID__H__DEFINED__

#include <stdint.h>

#define INTEGRAL_MAX 0.1

class controlPid{
public:
  controlPid(double Kp,double Ki, double Kd);
  controlPid();/*!< Default constructor */
  void update(double xnew,double dt);/*<! Receive a new state; update the control */
  double get_control();
  void set_integral_max(double val);
  void set_Kp(double);/*!< Set the proportional gain */
  void set_Ki(double);/*!< Set the integral gain */
  void set_Kd(double);/*!< Set the derivative gain */
private:
  double Kp;
  double Ki;
  double Kd;
  double integral_max;
  double f_term; /*!< integral term */
  double xlast;
  double u;
  int8_t init;
};

#endif
