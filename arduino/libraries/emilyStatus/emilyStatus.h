#ifndef __EMILYSTATUS_H_DEFINED__
#define __EMILYSTATUS_H_DEFINED__

#include <stdint.h>

/** Constant for converting GPS (degrees) to position in meters.
 * @brief equals degrees2radians factor (x) earth radius (meters)
 */
#define GPSDATA_D2R_RE 111194926.645
/** Direct control mode - we are sending offboard commands for the rudder and throttle */
#define CONTROL_MODE_DIRECT 0
/** Direct control mode - we are sending heading and speed commands and do PID control locally */
#define CONTROL_MODE_INDIRECT 1
/** passive control mode - initialized, no commands received */
#define CONTROL_MODE_PASSIVE 2
/** communication status: healthy (rate >1 Hz messages of any type) */
//#define COMM_STATUS_HEALTHY 1
/** communication status: slow or weak (> 1 second since last message) */
//#define COMM_STATUS_WARNING 2
/** communciation status: lost link (> 10 seconds since last message) */
//#define COMM_STATUS_LOST 3

double deg2m(double);/*!< Convert a double in degrees to arc length in meters */

/** Enumated type for the communication health.
 * Meanings: HEALTHY less than 1 second since the last message from the master
 *           WARNING more than 1 second but less than 10 seconds since the last message
 *           LOST more than 10 seconds since the last message
 */
enum commStatus{
  COMM_STATUS_HEALTHY,
  COMM_STATUS_WARNING,
  COMM_STATUS_LOST
};

/*! Class for holding GPS data */
class gpsData{
public:
  gpsData();
  double lon; /*!< Longitude (degrees east) */
  double lat; /*!< Latitutde (degrees north) */
  double t; /*!< Time (sec) */
  double x; /*!< X = north (meters) data in north-east earth frame */
  double y; /*!< Y = east (meters) data in north-east earth frame */
  bool init; /*!< Status indicator so we can tell if the value of a GPS object has been set */
  void set(double lati,double longi,double ti); /*!< Set the value of GPS data object and compute x-y */
  void set(int32_t lati,int32_t longi,double ti); /*!< Set the value of GPS data object and compute x-y. Uses long inputs to be compatible with comm protocol */
};

class emilyStatus
{
public:
  emilyStatus();
  gpsData gpsNow;
  gpsData gpsCmd;
  double control_rudder;
  double control_throttle;
  double command_heading;
  double command_speed;
  uint8_t control_mode;
  commStatus comm_status;
};

#endif
