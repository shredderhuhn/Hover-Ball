/* class WhiteLED controls a white LED in colors, mainly depending on hallsensor voltage
    As such, it is closely connect to the Hover -Ball-Project and not an independend class
    
    Gunter Wiedemann
    03.11.2022
*/

#ifndef WHITELED_H
#define WHITELED_H



/// @brief Color for the white LED (in 12 bits! - because all outputs are 12bits)
/// @param red Part of red in the white LED -  0 ... 4095 [digit]
/// @param green Part of green in the white LED -  0 ... 4095 [digit]
/// @param blue Part of blue in the white LED -  0 ... 4095 [digit]
struct LEDColor
{
    int red;
    int green;
    int blue;
};

extern const LEDColor LEDred, LEDgreen, LEDblue, LEDwhite, LEDyellow, LEDblack, LEDoff, LEDcyan, LEDpurple, LEDorange;

class WhiteLED {
    private:

        LEDColor color = LEDoff;
        int pinRed;
        int pinGreen;
        int pinBlue;
        bool ledStatus = 0; //0=off, 1=on

    public:

        /// @brief Constructor, defining the pin at Arduino Due with LED and 12 bit resolution
        WhiteLED(int _pinRed, int _pinGreen, int _pinBlue);

        /// @brief Setter for the private variable color
        void setColor(LEDColor _color);

        /// @brief Getter for the private variable color
        LEDColor getColor(void);

        void switchOnLED(void);

        void switchOffLED(void);

        void toggleLED(void);

        /// @brief switches the colors according to the state/position of the ball, limits to be found in constants.h
        /// @param errordigit control error (how many mm/100 is the distance between set value and measuere value )
        /// @param halldigit hall sensor readout
        /// @return 0 - ok, 1 - Ball falsch herum
        bool processControlState(int errordigit, int halldigit);

};

#endif