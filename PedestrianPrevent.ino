#include <TM1637.h>
#include "Ultrasonic.h"
/* Global Variables */
int distance = 0;
int prevDist = 0;
int8_t bits[4] = {0};
int timeout = 0;

/* Pin setup for display and Ultrasonic Sensor */
TM1637 slcd(38, 37);
Ultrasonic ultrasonic(36);

void setup()
{
  pinMode(40, OUTPUT);      /* Sets output pin for LED(s)/Speaker */
  slcd.init();              /* Initalize display and uses standard brightness level */
  slcd.set(BRIGHT_TYPICAL);
}
void loop()
{
  /* The distance the Ultrasonic sensor measures in cm; Approx. goes from 530 cm to 0 cm */
  distance = ultrasonic.MeasureInCentimeters();
  /* Once timeout is high enough, alarms sound */
  if (timeout >= 6) {
    for (int i = 0; i < 10; i++) {
      digitalWrite(40, HIGH);
      delay(50);
      digitalWrite(40, LOW);
      delay(50);
      timeout = 3;
    }
  }
  /* Checks for delta dist. over 1 second; if it is less than 4 the timeout goes up */
  else if (distance < 500 && abs(prevDist - distance) < 4) {
    timeout = timeout + 1;
  }
  /* Reduces timeout to account for people slowing down, then speeding up. Avoids many false alarms */
  else {
    if (timeout > 0) {
      timeout = timeout - 1;
    }
  }
  /* Displays distance on screen. Mostly for testing purposes */
  memset(bits, 0, 4);
  for (int i = 3; i >= 0; i--) {
    bits[i] = distance % 10;
    distance = distance / 10;
    slcd.display(i, bits[i]);
  }

  prevDist = distance; /* Assign previous distance to variable to calculate speed in the next loop */
  delay(1000); /* 1 second wait. Results in speed being in centimeters per second */
}
