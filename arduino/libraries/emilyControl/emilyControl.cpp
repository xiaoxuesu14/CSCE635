#include "emilyControl.h"
#include <stdint.h>

uint8_t scale_pwm(float val,float low, float high){
  return uint8_t(((val-low)/(high-low))*255.0);
}

emilyControl::emilyControl(emilyStatus*st){
  status = st;
  rudder = 0.0;
  throttle = 0.0;
  new_value = 0;
  millis_last = 0;
}

void emilyControl::misc_tasks(uint32_t millis){
  if (status->control_mode == CONTROL_MODE_PASSIVE){
    // write out zeros
    throttle = 0.0;
    rudder = 0.0;
    // FLAG that there is new data in the control object
    new_value = 1;
  }
  if (status->control_mode == CONTROL_MODE_DIRECT){
    // read from status object
    throttle = status->control_throttle;
    rudder = status->control_rudder;
    // FLAG that there is new data in the control object
    new_value = 1;
  }
  if (status->control_mode == CONTROL_MODE_INDIRECT){
    // check the time
    if (millis - millis_last > EMILYCONTROL_RATE_MILLIS){
      millis_last = millis;
      // check if the GPS data are new
      // copy local GPS
      // perform smoothing or estimation
      // compute PID control
      // not implemented yet: write out zeros
      throttle = 0.0;
      rudder = 0.0;
      // FLAG that there is new data in the control object
      new_value = 1;
    }
  }
}

void emilyControl::get_pwm(uint8_t* pwm_rudder,uint8_t* pwm_throttle){
  *pwm_rudder = scale_pwm(rudder,-1.0,1.0);
  *pwm_throttle = scale_pwm(throttle,0.0,1.0);
  // FLAG that the control value has been accessed
  new_value = 0;
}

uint8_t emilyControl::new_control(){
  return new_value;
}
