// all constants and parameters of the hover ball project
// Gunter Wiedemann
// 08.10.2022

#ifndef CONSTANTS_H
#define CONSTANTS_H


/**********************************/
/* General Constants              */
/**********************************/

#define ON                          1
#define OFF                         0
#define VALID                       1
#define INVALID                     0
#define MINSTATE                    0
#define MAXSTATE                    2   //Achtung, muss geändert werden, wenn weitere Zustände hinzukommen

/**********************************/
/* Arduino Due Port Definition    */
/**********************************/


#define HALLPIN                     A0
#define LED1R                       2
#define LED1G                       3
#define LED1B                       4
#define SWITCHR                     6
#define SWITCHG                     7



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
#define DIGIT2VOLT                  3.3/4095.0
#define VOLT2DIGIT                  4095.0/3.3

// Sensor limits
#define TOOMUCHVOLTAGE              2.7         // [V]
#define TOOCLOSETOSENSOR            1.0         // [V]
#define TOOCLOSETOOMAGNET           2.1         // [V]

// Sensor limits in digits (is used, and explicitely calculated for speed)
#define BALLWRONGDIGIT              3350        // [digit]
#define BALLTOOLOWDIGIT             1241        // [digit]
#define BALLTOOHIGHDIGIT            2606        // [digit]
#define NOBALLMAX                   3150        // [digit]
#define NOBALLMIN                   3050        // [digit]

/************************************/
/* Parameter of the Control Loop    */
/************************************/

#define INITIALSETPOINT             1600        // [1/100 mm]
#define MINSETPOINT                 1000        // [1/100 mm]
#define MAXSETPOINT                 2000        // [1/100 mm]
#define INITIALCURRENTOFFSET        1000        // vorher: 780         // [digit]
#define MINCURRENTOFFSET            0           // [digit]
#define MAXCURRENTOFFSET            4095        // [digit]
#define SAMPLETIME                  1000        // [µs]      // 1kHz Takt für den Anfang  
#define K0Z                         2362      
#define K1Z                         -3024
#define K2Z                         1376
#define K3Z                         10
#define UDAC0                       844         // [digit] 844 entspricht 1V


#endif