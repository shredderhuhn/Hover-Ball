/* 
Library "interaction" contains MMI functions for the hover ball project
Gunter Wiedemann
15.10.2022
*/ 

#include <HallSensor.h>
#include <constants.h>
#include <HBString.h>

#ifndef INTERACTION_H
#define INTERACTION_H



void serialinteraction(Status &status, HallSensor &hall);

void setLED(const Status &status, const HallSensor &hall);

#endif