/* 
Library "interaction" contains MMI functions for the hover ball project
Gunter Wiedemann
15.10.2022
*/ 

#include <HallSensor.h>
#include <constants.h>
#include <HBString.h>
#include <controller.h>

#ifndef INTERACTION_H
#define INTERACTION_H




/// @brief parses serial input and processes it
/// @param status status structure containing state machine, setpoint, offset
/// @param hall hallsensor instance
void serialinteraction(Status &status, HallSensor &hall, void (*handlerToTest)(void));

void setLED(const Status &status, const HallSensor &hall);

#endif