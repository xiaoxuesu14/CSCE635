#include "emilyStatus.h"

double deg2m(double x){
  return (x*GPSDATA_D2R_RE);
}

gpsData::gpsData(){
  init = false;
  lon = 0;
  lat = 0;
  t = 0;
  x = deg2m(lat);
  y = deg2m(lon);
}

void gpsData::set(double lati,double loni,double ti){
  init = true;
  lat = lati;
  lon = loni;
  t = ti;
  x = deg2m(lat);
  y = deg2m(lon);
}

void gpsData::set(long lati,long loni,double ti){
  /*lat = (1.0e-7*lati);lon = (1.0e-7*loni);t = ti;x = deg2m(lat);y = deg2m(lon);*/
  set((1.0e-7*lati),(1.0e-7*loni),ti);
}

emilyStatus::emilyStatus(){
  // initialize in passive control mode
  control_mode = CONTROL_MODE_PASSIVE;
}
