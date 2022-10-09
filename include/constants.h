// all constants and parameters of the hover ball project
// Gunter Wiedemann
// 08.10.2022

/**********************************/
/* Arduino Due Port Definition    */
/**********************************/

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define HALLPIN                     A0


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
#define Volt2Digit                  4095.0/3.3

#endif