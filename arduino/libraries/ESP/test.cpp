#include <cstdlib>
#include <stdint.h>
#include <string.h>
#include <cstdio>
#include "ESP.h"

double gen_random(double low, double high){
  return ( double(rand()) * (high-low)/(double(RAND_MAX)+1) + low );
}

int test_control(int n){
  int counter = 0;
  for (int k = 0;k<n;k++){
    uint8_t msg[256];
    float t1 = gen_random(0.0,1.0),
    h1 = gen_random(-1.0,1.0),
    t=0,
    h=0;
    int8_t flag;
    flag = esp_pack_control(msg,h1,t1);
    printf("Raw: rudder = %g, speed = %g\n",h1,t1);
    printf("Packed %d bytes into message\n",flag);
    if (esp_unpack_control(msg,&h,&t) > 0){
      printf("Message: rudder = %g, speed = %g\n",h,t);
      counter++;
    }
  }
  return counter;
}

int test_command(int n){
  int counter = 0;
  for (int k = 0;k<n;k++){
    uint8_t msg[256];
    float t1 = 0.8,
    h1 = .909812916276,
    t=0,
    h=0;
    int8_t flag;
    flag = esp_pack_command(msg,h1,t1);
    printf("Raw: heading = %g, speed = %g\n",h1,t1);
    printf("Packed %d bytes into message\n",flag);
    if (esp_unpack_command(msg,&h,&t) > 0){
      printf("Message: heading = %g, speed = %g\n",h,t);
      counter++;
    }
  }
  return counter;
}

int test_pid(int n){
  int counter = 0;
  for (int k = 0;k<n;k++){
    uint8_t msg[256];
    uint8_t ch1 = 0,ch = -1;
    float kp1 = 0.8,
    ki1 = .909812916276,
    kd1 = -1.2324,
    kp=0,
    ki=0,
    kd=0;
    int8_t flag;
    flag = esp_pack_set_pid(msg,ch1,kp1,ki1,kd1);
    printf("Raw: ch=%d,kp=%g,ki=%g,kd=%g\n",ch1,kp1,ki1,kd1);
    printf("Packed %d bytes into message\n",flag);
    if (esp_unpack_set_pid(msg,&ch,&kp,&ki,&kd) > 0){
      printf("Message: ch=%d,kp=%g,ki=%g,kd=%g\n",ch,kp,ki,kd);
      counter++;
    }
  }
  return counter;
}

int main(){
  int monte = 1;
  printf("%d/%d valid messages in test_control\n",test_control(monte),monte);
  printf("%d/%d valid messages in test_command\n",test_command(monte),monte);
  printf("%d/%d valid messages in test_pid\n",test_pid(monte),monte);
  return 0;
}
