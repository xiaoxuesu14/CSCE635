
/** @file */
/** Simple sketch that tests a voltage divider on pin 9 
 * 
 * @brief Test that the voltage divider correctly steps down the voltage to 3.3V from 5.
 */

int pinHigh = 9;

void setup() {
  // put your setup code here, to run once:
  pinMode(pinHigh,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(pinHigh,HIGH);
}
