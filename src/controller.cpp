#include <controller.h>
#include <constants.h>
#include <HallSensor.h>

void calcController(int measpoint) {
  
  ctrl.error = -(ctrl.setpoint - measpoint);
  ctrl.errorTildeK = K0Z * ctrl.error;
  ctrl.u1K = (ctrl.errorTildeK * K3Z) >> 0;
  ctrl.u2KM1 = ctrl.u2K;
  ctrl.u2K = ((ctrl.errorTildeKM1 * K1Z) >> 10) + ((ctrl.u2KM1 * K2Z) >> 12);
  ctrl.u = (ctrl.u1K + ctrl.u2K) >> 7;
  ctrl.u = (ctrl.u1K + ctrl.u2K) >> 7;
  ctrl.dac0 = UDAC0;
  ctrl.dac1 = constrain(ctrl.u + ctrl.offset + UDAC0, 0, 4095);
  analogWrite(DAC0,ctrl.dac0);
  analogWrite(DAC1,ctrl.dac1);
}

void resetController(void) {
    ctrl.setpoint = status.setpoint;
    ctrl.offset = status.offset;
    ctrl.error = 0;
    ctrl.errorTildeK = 0;
    ctrl.errorTildeKM1 = 0;
    ctrl.u1K = 0;
    ctrl.u2K = 0;
    ctrl.u2KM1 = 0;
    ctrl.u = 0;
    ctrl.dac0 = 0;
    ctrl.dac1 = 0;
}

void initController(void) {
    ctrl.setpoint = status.setpoint;
    ctrl.offset = status.offset;
    ctrl.error = 0;
    ctrl.errorTildeK = 0;
    ctrl.errorTildeKM1 = 0;
    ctrl.u1K = 0;
    ctrl.u2K = 0;
    ctrl.u2KM1 = 0;
    ctrl.u = 0;
    ctrl.dac0 = UDAC0;
    ctrl.dac1 = UDAC0 + ctrl.offset;
    analogWrite(DAC0,ctrl.dac0);
    analogWrite(DAC1,ctrl.dac1);
}

void resetState(Status &status) {
  status.offset = INITIALCURRENTOFFSET;
  status.setpoint = INITIALSETPOINT;
  status.state = 0;
  status.failure = 0;
}