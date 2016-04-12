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
}

void emilyControl::misc_tasks(){
  // add a timer here in the future
  new_value = 1;
  if (status->control_mode == CONTROL_MODE_PASSIVE){
    // write out zeros
    throttle = 0.0;
    rudder = 0.0;
  }
  if (status->control_mode == CONTROL_MODE_DIRECT){
    // read from status object
    throttle = status->control_throttle;
    rudder = status->control_rudder;
  }
  if (status->control_mode == CONTROL_MODE_INDIRECT){
    // not implemented yet: write out zeros
    throttle = 0.0;
    rudder = 0.0;
  }
}

void emilyControl::get_pwm(uint8_t* pwm_rudder,uint8_t* pwm_throttle){
  *pwm_rudder = scale_pwm(rudder,-1.0,1.0);
  *pwm_throttle = scale_pwm(throttle,0.0,1.0);
  new_value = 0;
}

uint8_t emilyControl::new_control(){
  return new_value;
}
