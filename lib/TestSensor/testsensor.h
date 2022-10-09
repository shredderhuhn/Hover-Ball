/* Testsensor library tests the hall sensor in different ways

Gunter Wiedemann
09.10.2022

*/

#include <Arduino.h>

#ifndef TESTSENSOR_H
#define TESTSENSOR_H



/// @brief This function needs to be inserted in the main.cpp/setup()
void testSensorSetup();

/// @brief This function needs to be inserted in the main.cpp/loop()
void testSensorLoop();

#endif