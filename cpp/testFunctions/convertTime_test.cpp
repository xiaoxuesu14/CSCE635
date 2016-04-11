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
  int timef;
  srand(time(NULL));//seed RNG
  int n = 1;
  for(int k = 0;k<n;k++){
    timef = int(gen_random(0.0,86400.0));
    char field[256];
    int secrem = (timef % 3600);
    int hr = int(timef/3600);
    int min = int(secrem/60);
    int sec = (secrem%60);
    printf("Time: %d sec = %d:%d.%d\n",timef,(int)hr,(int)min,(int)sec);
    sprintf(field,"%2d%2d%2d",hr,min,sec);
    int32_t times = convertTime(field);
    printf("Converted: %d\n",times);
  }
  return 0;
}
