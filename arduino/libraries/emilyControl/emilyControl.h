#ifndef __EMILYCONTROL_H_DEFINED__
#define __EMILYCONTROL_H_DEFINED__

#include <stdint.h>
#include "emilyStatus.h"

/** Scale a floating point 'val between 'low' and 'high' to the PWM output range of [0,255] */
uint8_t scale_pwm(float val,float low, float high);

class emilyControl{
public:
  /** Constructor
   *
   * @param[in] st pointer to the global status object
   */
  emilyControl(emilyStatus*st);
  /** Compute periodic tasks
   *
   * Write out control value, or compute new value & write if fly-by-wire.
   */
  void misc_tasks();
  /** Return the PWM values to apply for the rudder and throttle.
   *
   * @param[out] pwm_rudder rudder PWM expressed in [0,255].
   * @param[out] pwm_throttle throttle PWM expressed in [0,255]
   */
  void get_pwm(uint8_t* pwm_rudder,uint8_t* pwm_throttle);
  uint8_t new_control();/*<! Return 1 if the control value has been updated */
private:
  emilyStatus* status; /*<! Pointer to the global status object */
  float rudder;
  float throttle;
  uint8_t new_value;
};

#endif
