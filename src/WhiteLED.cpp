/* class WhiteLED controls a white LED in colors, mainly depending on hallsensor voltage
    As such, it is closely connect to the Hover -Ball-Project and not an independend class
    
    Gunter Wiedemann
    03.11.2022
*/

#include <WhiteLED.h>
#include <Arduino.h>
#include <constants.h>

LEDColor const LEDred{4095,0,0};
LEDColor const LEDgreen{0,4095,0};
LEDColor const LEDblue{0,0,4095};
LEDColor const LEDwhite{4095,4095,4095};
LEDColor const LEDyellow{4095,2000,0};
LEDColor const LEDblack{0,0,0};
LEDColor const LEDoff{0,0,0};
LEDColor const LEDcyan{0,4095,4095};
LEDColor const LEDpurple{4095,0,4095};
LEDColor const LEDorange{4095,512,0};

/*+*******************************************************************************/
/* Constructor                                                                   */
/*+*******************************************************************************/

WhiteLED::WhiteLED(int _pinRed, int _pinGreen, int _pinBlue) {
    pinRed = _pinRed;
    pinGreen = _pinGreen;
    pinBlue = _pinBlue;
    // Setup pins
    pinMode(_pinRed, OUTPUT);
    pinMode(_pinGreen, OUTPUT);
    pinMode(_pinBlue, OUTPUT);
    analogWriteResolution(12);
}

void WhiteLED::setColor(LEDColor _color) {
    color.red = _color.red;
    color.green = _color.green;
    color.blue = _color.blue;
}

LEDColor WhiteLED::getColor() {
    return color;
}



void WhiteLED::switchOnLED(void){
    analogWrite(pinRed,color.red);
    analogWrite(pinGreen,color.green);
    analogWrite(pinBlue,color.blue);
    ledStatus = true;
}

void WhiteLED::switchOffLED(void){
    analogWrite(pinRed,0);
    analogWrite(pinGreen,0);
    analogWrite(pinBlue,0);
    ledStatus = false;
}

void WhiteLED::toggleLED(void){
    if (ledStatus) {
        switchOffLED();
    } else {
        switchOnLED();
    }
}

bool WhiteLED::processControlState(int errordigit, int halldigit){
    if ((errordigit > -100) && (errordigit <100)) {
        setColor(LEDblue);
        switchOnLED();
        return 0;
    } else if ((errordigit > -500) && (errordigit <500) && (halldigit > 1000)) {
        setColor(LEDgreen);
        switchOnLED();
        return 0;
    } else if (halldigit > BALLWRONGDIGIT) {
        setColor(LEDred);
        switchOnLED();
        return 1;
    } else if ((halldigit < NOBALLMAX) && (halldigit > NOBALLMIN)) {
        setColor(LEDyellow);
        switchOnLED();
        return 0;
    } else {
        setColor(LEDgreen);
        switchOnLED();
        return 0;
    };
    return 0;
}