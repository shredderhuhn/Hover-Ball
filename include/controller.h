// All Controller variables and functions
// no class to avoid overhead
// Gunter Wiedemann
// 23.10.2022

#include <constants.h>
#include <HallSensor.h>
#include <Arduino.h>

#ifndef CONTROLLER_H
#define CONTROLLER_H

/***************************************/
/* Structures that aren't constants    */
/***************************************/

/// @brief contains all variables that can be changed by interaction
/// @param state State of the state machine - int 0...10
/// @param setpoint Setpoint of the control loop - int 1000 ... 2000 [1/100 mm]
/// @param offset Current offset of the control output -  0 ... 4095 [digit]
/// @param failure failure variable for wrong/lost ball position
struct Status
{
    int state;
    int setpoint;
    int offset;
    int failure;
};

struct Ctrl
{
    int setpoint;
    int offset;
    int error;
    int errorTildeK;
    int errorTildeKM1;
    int u1K;
    int u2K;
    int u2KM1;
    int u;
    int dac0;
    int dac1;
    //int controllerOut;
};


Status status;
Ctrl ctrl;


// Functions

void calcController(int measpoint);

void resetController(void);

void initController(void);

void setOutputValues(void);

void resetState(Status &status);

#endif