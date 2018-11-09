#include <TM1637.h>
#include "Ultrasonic.h"
/* Global Variables  */
int distance = 0;
int prevDist = 0;
int8_t bits[4] = {0};
int timeout = 0;
int dispNum = 0;

/* Pin setup for display and Ultrasonic Sensor */
TM1637 slcd(38, 37);
Ultrasonic ultrasonic(36);

void setup()
{
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(40, OUTPUT);      /* Sets output pin for LED(s)/Speaker */
  slcd.init();              /* Initalize display and uses standard brightness level */
  slcd.set(BRIGHT_TYPICAL);
}
void loop()
{
  if (timeout == 4) {
    digitalWrite(24, HIGH);
    digitalWrite(25, HIGH);
    digitalWrite(26, HIGH);
    digitalWrite(27, HIGH);
  }

  if (timeout == 3) {
    digitalWrite(24, HIGH);
    digitalWrite(25, HIGH);
    digitalWrite(26, HIGH);
    digitalWrite(27, LOW);
  }

  if (timeout == 2) {
    digitalWrite(24, HIGH);
    digitalWrite(25, HIGH);
    digitalWrite(26, LOW);
    digitalWrite(27, LOW);
  }

  if (timeout == 1) {
    digitalWrite(24, HIGH);
    digitalWrite(25, LOW);
    digitalWrite(26, LOW);
    digitalWrite(27, LOW);
  }

  if (timeout == 0) {
    digitalWrite(24, LOW);
    digitalWrite(25, LOW);
    digitalWrite(26, LOW);
    digitalWrite(27, LOW);
  }

  /* The distance the Ultrasonic sensor measures in cm; Approx. goes from 530 cm to 0 cm */
  distance = ultrasonic.MeasureInCentimeters();
  /* Once timeout is high enough, alarms sound */
  if (timeout > 3) {
    for (int i = 0; i < 10; i++) {
      digitalWrite(40, HIGH);
      delay(50);
      digitalWrite(40, LOW);
      delay(50);

    }
    timeout = 0;
  }
  /* Checks for delta dist. over 1 second; if it is less than 4 the timeout goes up */
  if (distance < 500 && abs(prevDist - distance) < 5) {
    timeout = timeout + 1;
  }
  else if (abs(prevDist - distance) > 5 && distance < 500 && timeout > 0) {
      timeout = timeout - 1;
  }
  /* Reduces timeout to account for people slowing down, then speeding up. Avoids many false alarms */

  /* Displays distance on screen. Mostly for testing purposes */
  memset(bits, 0, 4);
  dispNum = timeout;
  for (int i = 3; i >= 0; i--) {
    bits[i] = dispNum % 10;
    dispNum = dispNum / 10;
    slcd.display(i, bits[i]);
  }

  prevDist = distance; /* Assign previous distance to variable to calculate speed in the next loop */
  delay(1000); /* 1 second wait. Results in speed being in centimeters per second */
}
