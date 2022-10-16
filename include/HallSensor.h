/* 
Class HallSensor processes the hallsensor SS495 in the Hover Ball project,
i.e., it also provides derived distances from ball to magnet.

Gunter Wiedemann
08.10.2022

*/

//don't forget the endif-Statement
#ifndef HALLSENSOR_h
#define HALLSENSOR_h

//#include <Arduino.h>


/// @brief contains all functions and variables to handle sensor data
class HallSensor {
    private:
        int pinOfHallSensor;
        int distanceBetweenHallAndMagnet; // [1/100mm]
        int ballDiameter; 
        int rawValue = 0; // raw value of the pin
        
    public:
        // empty constructor
        // Constructor, defining the pin at Arduino Due with hallsensor and 12 bit resolution
        HallSensor(int _pinOfHallSensor);
        /// Constructor, defining hallsensor pin and distance between sensor and magnet
        HallSensor(int _pinOfHallSensor, int _distanceBetweenPinAndMagnet, int _ballDiameter);
        /// reads the rawValue and stores it in the instance
        void ReadRawValue(); // [digit]
        // calculates the distance between hall sensor and ball with exponentiell approximation
        int CalcDistanceSensorVsBallExact(); // [1/100mm]
        // calculates the distance between hall sensor and ball with polynomial approximation
        int CalcDistanceSensorVsBallPoly(); // [1/100mm]
        // calculates the distance between hall sensor and magnet with exponentiell approximation
        int CalcDistanceMagnetVsBallExact(); // [1/100mm]
        // calculates the distance between hall sensor and magnet with polynomial approximation
        int CalcDistanceMagnetVsBallPoly(); // [1/100mm]
        // display all values on serial monitor
        void DispAllAtSerial();

    
        //setter and getter
        void SetdistanceBetweenPinAndMagnet(int);   // [1/100mm]
        int GetdistanceBetweenPinAndMagnet(void);   // [1/100mm]
        void SetBallDiameter(int);                  // [1/100mm]
        int GetBallDiameter(void);                  // [1/100mm]
        void SetRawValue(int);  // [digit] // just a debaug function
        int GetRawValue(void);  // [digit]
        double GetVoltageValue(void); // 

};


#endif