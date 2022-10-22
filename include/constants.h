// all constants and parameters of the hover ball project
// Gunter Wiedemann
// 08.10.2022

#ifndef CONSTANTS_H
#define CONSTANTS_H

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

/***************************************/
/* failure definition                  */
/***************************************/
#define BALLUPSIDEDOWN              1
#define BALLATMAGNET                2
#define BALLDOWN                    4

/**********************************/
/* General Constants              */
/**********************************/

#define ON                          1
#define OFF                         0
#define VALID                       1
#define INVALID                     0
#define MINSTATE                    0
#define MAXSTATE                    5   //Achtung, muss geändert werden, wenn weitere Zustände hinzukommen

/**********************************/
/* Arduino Due Port Definition    */
/**********************************/


#define HALLPIN                     A0
#define LED1R                       2
#define LED1G                       3
#define LED1B                       4
#define LED2R                       5
#define LED2G                       6
#define LED2B                       7
#define LED3R                       8
#define LED3G                       9
#define LED3B                       10
#define MOTOROFFSETPIN              DAC0
#define MOTORCURRENTPIN             DAC1



/**********************************/
/* Parameter of the Hover Ball    */
/**********************************/

#define DISTANCESENSORMAGNET        11000        // [1/100 mm]
#define BALLDIAMETER                8000         // [1/100 mm]


/**********************************/
/* Parameter of the Hall Sensor   */
/**********************************/

// for polynomial approximation
#define A0HAT                       1572
#define A1HAT                       -260
#define A2HAT                       695
#define RIGHTSHIFTA1HAT             8
#define RIGHTSHIFTA21HAT            6
#define RIGHTSHIFTA22HAT            18

// for exact caculation
#define K4MAGCOEFF1                 1548.1
#define K4MAGCOEFF2                 -0.1248
#define KSENSEU2B                   320.0
#define USENSE0                     2.5
#define VOLT2DIGIT                  4095.0/3.3

// Sensor limits
#define TOOMUCHVOLTAGE              2.7         // [V]
#define TOOCLOSETOSENSOR            1.0         // [V]
#define TOOCLOSETOOMAGNET           2.1         // [V]

/************************************/
/* Parameter of the Control Loop    */
/************************************/

#define INITIALSETPOINT             1600        // [1/100 mm]
#define MINSETPOINT                 1000        // [1/100 mm]
#define MAXSETPOINT                 2000        // [1/100 mm]
/// @todo find out right INITIALCURRENTOFFSET for INITIALSETPOINT
/// @todo find out whether it is better to work with Amps instead of digits
#define INITIALCURRENTOFFSET        0           // [digit]
#define MINCURRENTOFFSET            0           // [digit]
#define MAXCURRENTOFFSET            4095        // [digit]
#define SAMPLETIME                  1000        // [µs]      // 1kHz Takt für den Anfang       

#endif