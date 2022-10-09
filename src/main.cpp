#include <Arduino.h>
#include <DueTimer.h>
//#include <HBStringTest.h>
//#include <HallSensor.h>
#include <constants.h>
#include <testsensor.h>

// Variables need to be global to get into setup and loop
//HallSensor hall(HALLPIN, DISTANCESENSORMAGNET, BALLDIAMETER);

void setup() {
  // put your setup code here, to run once:
  // hbStringSetup();
  testSensorSetup();
}

void loop() { 
  // put your main code here, to run repeatedly:
  // hbStringLoop();
  testSensorLoop();
}