#include <Arduino.h>
#include <DueTimer.h>
//#include <HBStringTest.h>
#include <HallSensor.h>
#include <constants.h>

// Variables need to be global to get into setup and loop
HallSensor hall(HALLPIN, DISTANCESENSORMAGNET, BALLDIAMETER);

void setup() {
  // put your setup code here, to run once:
  // hbStringSetup();
  Serial.begin(9600);
    while(!Serial.available()) {} //warten, dass der serielle Monitor online ist
    Serial.readString();
    Serial.println("Jetzt geht es los.");
    hall.SetRawValue(2077); //2077
    hall.DispAllAtSerial(); 
}

void loop() { 
  // put your main code here, to run repeatedly:
  // hbStringLoop();
}