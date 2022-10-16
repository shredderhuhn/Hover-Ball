/* 
Class HallSensor processes the hallsensor SS495 in the Hover Ball project,
i.e., it also provides derived distances from ball to magnet.

Gunter Wiedemann
08.10.2022

*/

#include <HallSensor.h>    //needs to be inserted, otherwise .h is not found
#include <Arduino.h>
#include <constants.h>
#include <math.h>


/*+*******************************************************************************/
/* Constructors                                                                  */
/*+*******************************************************************************/

// Constructor, defining the pin at Arduino Due with hallsensor and 12 bit resolution
HallSensor::HallSensor(int _pinOfHallSensor) {
    pinOfHallSensor = _pinOfHallSensor;
    analogReadResolution(12);
    pinMode(pinOfHallSensor, INPUT); //10 bit resolution is default
}

/// Constructor, defining hallsensor pin and distance between sensor and magnet
HallSensor::HallSensor(int _pinOfHallSensor, int _distanceBetweenPinAndMagnet, int _ballDiameter) {
    pinOfHallSensor = _pinOfHallSensor;
    analogReadResolution(12);
    pinMode(pinOfHallSensor, INPUT); //12 bit resolution is default
    distanceBetweenHallAndMagnet = _distanceBetweenPinAndMagnet;
    ballDiameter = _ballDiameter;
}

/*+*******************************************************************************/
/* Setter and Getter of private properties                                       */
/*+*******************************************************************************/


void HallSensor::SetdistanceBetweenPinAndMagnet(int _distanceBetweenPinAndMagnet) {
    distanceBetweenHallAndMagnet = _distanceBetweenPinAndMagnet;
}

int HallSensor::GetdistanceBetweenPinAndMagnet(void) {
    return distanceBetweenHallAndMagnet;
}

void HallSensor::SetBallDiameter(int _ballDiameter) {
    ballDiameter = _ballDiameter;
}

int HallSensor::GetBallDiameter(void) {
    return ballDiameter;
}

// just for diagnosis
void HallSensor::SetRawValue(int _rawValue) {
    rawValue = _rawValue;
}

int HallSensor::GetRawValue(void) {
    return rawValue;
}


/*+*******************************************************************************/
/* Public Methods                                                                */
/*+*******************************************************************************/


/// reads the rawValue and stores it in the instance
void HallSensor::ReadRawValue() {
    rawValue = analogRead(pinOfHallSensor);
};

// calculates the distance between hall sensor and ball with exponentiell approximation
int HallSensor::CalcDistanceSensorVsBallExact() {
    double uSens = VOLT2DIGIT * rawValue;
    double x = 1 / K4MAGCOEFF2 * log(KSENSEU2B / K4MAGCOEFF1 * (USENSE0 - uSens));
    return (int(x * 100));
};

// calculates the distance between hall sensor and ball with polynomial approximation
int HallSensor::CalcDistanceSensorVsBallPoly() {
    int branch0 = A0HAT;
    int branch1 = (rawValue * A1HAT) >> RIGHTSHIFTA1HAT;
    int branch2Temp = (rawValue * A2HAT) >> RIGHTSHIFTA21HAT;
    int branch2 = (branch2Temp * branch2Temp) >> RIGHTSHIFTA22HAT;
    return (branch0 + branch1 + branch2);
};

// calculates the distance between hall sensor and magnet with exponentiell approximation
int HallSensor::CalcDistanceMagnetVsBallExact() {
    return(distanceBetweenHallAndMagnet - CalcDistanceSensorVsBallExact() - ballDiameter);
};

// calculates the distance between hall sensor and magnet with polynomial approximation
int HallSensor::CalcDistanceMagnetVsBallPoly() {  
    return(distanceBetweenHallAndMagnet - CalcDistanceSensorVsBallPoly() - ballDiameter);
};

// display all values on serial monitor
void HallSensor::DispAllAtSerial() {
    Serial.print("Abstand Elektromagnet <-> Hallsensor = ");
    Serial.print(distanceBetweenHallAndMagnet);
    Serial.println(" 1/100 mm");
    Serial.print("Balldurchmesser =");
    Serial.print(ballDiameter);
    Serial.println(" !/100 mm");
    Serial.print("Rohwert = ");
    Serial.print(rawValue);
    Serial.println(" digits");
    Serial.print("Abstand Ball <-> Hallsensor (Polynom) = ");
    Serial.print(CalcDistanceSensorVsBallPoly());
    Serial.println(" 1/100 mm");
    Serial.print("Abstand Ball <-> Elektromagnet (Polynom) = ");
    Serial.print(CalcDistanceMagnetVsBallPoly());
    Serial.println(" 1/100 mm");
    Serial.print("Abstand Ball <-> Hallsensor (Exakt) = ");
    Serial.print(CalcDistanceSensorVsBallExact());
    Serial.println(" 1/100 mm");
    Serial.print("Abstand Ball <-> Elektromagnet (Exakt) = ");
    Serial.print(CalcDistanceMagnetVsBallExact());
    Serial.println(" 1/100 mm");

    
};
