#include <stdint.h>
#include "controlPid.h"

controlPid::controlPid(){
  controlPid(0.0,0.0,0.0);
}

controlPid::controlPid(double Kpi,double Kii,double Kdi){
  Kp = Kpi;
  Ki = Kii;
  Kd = Kdi;
  init = 0;
  integral_max = INTEGRAL_MAX;
}

void controlPid::update(double xnew,double dt){
  if (!init){
    xlast = xnew;
    init;
  }
  // increment the integral term
  f_term += xnew*dt;
  if (f_term > integral_max)
    f_term = integral_max;
  if (f_term < -integral_max)
    f_term = -integral_max;
  // compute control
  u = -Kp*xnew-Kd*(xnew-xlast)/dt-Ki*f_term;
}

double controlPid::get_control(){
  return u;
}

void controlPid::set_integral_max(double val){
  integral_max = val;
}

void controlPid::set_Kp(double Kpi){
  Kp = Kpi;
}

void controlPid::set_Ki(double Kii){
  Ki = Kii;
}

void controlPid::set_Kd(double Kdi){
  Kd = Kdi;
}
