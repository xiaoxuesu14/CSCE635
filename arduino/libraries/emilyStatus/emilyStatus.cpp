#include <stdint.h>
#include "emilyStatus.h"

/** Convert an arc value in (10^-7 deg) to length in meters */
double deg2m(int32_t x){
  return double(GPSDATA_D2R_RE*x);
}

gpsData::gpsData(){
  init = false;
  lon = 0;
  lat = 0;
  t = 0;
  x = 0.0;
  y = 0.0;
  lat_home = GPS_HOME_LAT_DEFAULT;
  lon_home = GPS_HOME_LON_DEFAULT;
}

void gpsData::set(double lati,double loni,double ti){
  set( int32_t(lati*1e7),int32_t(loni*1e7), ti);
  /*
  lat = lati;
  lon = loni;
  t = ti;
  x = deg2m(lat-lat_home);
  y = deg2m(lon-lon_home);
  */
}

void gpsData::set(int32_t lati,int32_t loni,double ti){
  init = true;
  lat = lati;
  lon = loni;
  t = ti;
  x = deg2m(lat-lat_home);
  y = deg2m(lon-lon_home);
}

void gpsData::set_home(int32_t lat1,int32_t lon1){
  lat_home = lat1;
  lon_home = lon1;
}

emilyStatus::emilyStatus(){
  // initialize in passive control mode
  control_mode = CONTROL_MODE_PASSIVE;
}
