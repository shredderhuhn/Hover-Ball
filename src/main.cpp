#include <Arduino.h>
#include <DueTimer.h>
#include <HallSensor.h>
#include <constants.h>
#include <interaction.h>
#include <controller.h>
#include <WhiteLED.h>

/*
Known Bugs:
- keine
*/

// Variables need to be global to get into setup and loop
HallSensor hall(HALLPIN, DISTANCESENSORMAGNET, BALLDIAMETER);
WhiteLED led(LED1R,LED1G,LED1B);
bool failurestickyflag = 0;
// further global variables from controller.h: ctrl, status


void controlHandler(void) {
  hall.ReadRawValue();
  calcController(hall.CalcDistanceMagnetVsBallPoly());
  setOutputValues();
  status.failure = led.processControlState(ctrl.error, hall.GetRawValue());
}

void setup() {
  // Setup the status struct
  resetState(status);
  resetController();

  // Setup Ausgabe
  analogWriteResolution(12); //Wiederholung, denn in WhiteLED-Konstruktor wird dies bereits aufgerufen
  resetOutputValues();

  //Setup Timer3 as timer from Duetimer
  Timer3.attachInterrupt(controlHandler);
  Timer3.setPeriod(SAMPLETIME);

  // Seriellen Monitor einschalten
  //Serial.begin(9600);
  //while(!Serial.available()) {} //warten, dass der serielle Monitor online ist
  //Serial.readString();
  //Serial.println("Jetzt geht es los.");
}

void loop() { 
  //status.state = setstate(); -> brauchen wir das???
  switch (status.state)
  {
  /* State 0: 
      initialer Zustand bzw. Zustand, wenn die Kugel ganz oben/unten hängt
      Kugel wird aufgesetzt, geprüft, ob sie richtig rum ist
      per Knopfdruck oder "next" in den State 1
      per "rotem" Knopf wird dieser Zustand immer erreicht
  */
  case 0: 
    Timer3.stop();
    resetController();
    led.setColor(LEDred);
    led.switchOnLED();
    status.state++;
    //serialinteraction(status, hall, controlHandler);    
    break;

  /* State 1:
      Anschalten des eingestellten Stromoffsets
      Einschalten des Reglers
      automatischer Wechsel in den State 2
  */
  case 1:
    /// @todo switch on current offset (Ausgabe am Pin)
    initController();
    setOutputValues();
    status.state++;
    Timer3.start();
    break;

  /* State 2:
      Regelung läuft
  */
  case 2:
    break;
  
  /* DEFAULT sollte nie erreicht werden */
  default:
    break;
  }
}