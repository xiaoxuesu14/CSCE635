#include <cstdio>
#include <stdint.h>
#include <string.h>
#include "emilyGPS.h"

#include <cstdlib>
#include <time.h>       /* time */
#include <math.h>

double gen_random(double low, double high){
  return ( double(rand()) * (high-low)/(double(RAND_MAX)+1) + low );
}

int main(){
  double lonf, latf;
  srand(time(NULL));//seed RNG
  int n = 100;
  double maxe = -100.0;
  for(int k = 0;k<n;k++){
    lonf = gen_random(0.0,180.0);
    latf = gen_random(0.0,90.0);
    char field[256];
    double min = 60.0*(lonf - int16_t(lonf));
    int16_t deg = int16_t(lonf);
    sprintf(field,"%03d%05.2f",deg,min);
    printf("Converting lon %f to std format.\n",lonf);
    printf("%s\n",field);
    int32_t lon = convertGPS(field);
    printf("Converted to %d\n",lon);

    deg = int16_t(latf);
    min = 60.0*(latf - deg);
    sprintf(field,"%03d%05.2f",deg,min);
    printf("Converting lat %f to std format.\n",latf);
    printf("%s\n",field);
    int32_t lat = convertGPS(field);
    printf("Converted to %d\n",lat);

    // compute percent error
    double pct1, pct2;
    pct1 = (lonf - double(1.0e-7*lon))/lonf;
    pct2 = (latf - double(1.0e-7*lat))/latf;
    printf("Percent errors (deg): %g, %g\n",pct1,pct2);
    if (fabs(pct1) > maxe)
      maxe = fabs(pct1);
    if (fabs(pct2) > maxe)
      maxe = fabs(pct2);
  }
  printf("Max percent error: %g\n",maxe);
  return 0;
}
