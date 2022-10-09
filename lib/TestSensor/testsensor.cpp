#include <testsensor.h>
#include <Arduino.h>

void testSensorSetup() {
    analogReadResolution(12);
    pinMode(A0, INPUT);
    Serial.begin(9600);
}

void testSensorLoop() {
    int rawvalue = analogRead(A0);
    Serial.print("rawvalue = ");
    Serial.println(rawvalue);
    Serial.print("Expected voltage = ");
    Serial.print(rawvalue*3.3/4096);
    Serial.println(" V");
    while(!Serial.available()) {} //warten, dass der serielle Monitor online ist
    Serial.readString();
}