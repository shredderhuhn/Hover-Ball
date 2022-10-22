#include <Arduino.h>
#include <DueTimer.h>
#include <HallSensor.h>
#include <constants.h>
#include <interaction.h>


// Variables need to be global to get into setup and loop
HallSensor hall(HALLPIN, DISTANCESENSORMAGNET, BALLDIAMETER);
Status status;
Ctrl ctrl;



void controlHandler(void) {
  hall.ReadRawValue();
  ctrl.error = -(ctrl.setpoint - hall.CalcDistanceMagnetVsBallPoly());
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

}

void setup() {
  // Setup the status struct
  initState(status);

  //Setup Timer3 as timer from Duetimer
  Timer3.attachInterrupt(controlHandler);
  Timer3.setPeriod(SAMPLETIME);

  // Seriellen Monitor einschalten
  Serial.begin(9600);
  while(!Serial.available()) {} //warten, dass der serielle Monitor online ist
  Serial.readString();
  Serial.println("Jetzt geht es los.");
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
    setLED(status, hall);
    serialinteraction(status, hall);
    
    break;
  /* State 1:
      Händisches Kalibrieren der symmetrischen Magnetstellung unten
      minimale Spannung am Hallsensor, Nichtverändern der Spannung beim Drehen der Kugel
      per Knopfdruck oder "next" in den State 2
  */
  case 1:
    setLED(status, hall);
    serialinteraction(status, hall);
    break;
  /* State 2:
      Einstellen des Sollwertes: Die HOhlschraube wird nach oben gedreht bis der Sollwert erreicht ist
      per Knopfdruck oder "next" in den State 3
  */
  case 2:
    setLED(status, hall);  
    serialinteraction(status, hall);
    break;
  /* State 3:
      Anschalten des eingestellten Stromoffsets
      per Knopfdruck oder "next" in den State 4
  */
  case 3:
    setLED(status, hall);  
    /// @todo switch on current offset (Ausgabe am Pin)
    serialinteraction(status, hall);
    break;
  /* State 4:
      Sollwert = Messwert (+x) setzen
      Einschalten des Reglers
      automatischer Wechsel in den State 5
  */
  case 4:
    setLED(status, hall);  
    /// @todo Sollwert = Messwert (+x) setzen
    status.state++;
    Timer3.start();
    break;
  /* State 5:
      Regelung läuft
  */
  case 5:
    setLED(status, hall); 
    /// @todo Hier muss eine Funktion aufgerufen werden, die überprüft, ob die Regelung noch läuft, oder ob man oben/unter anschlägt 
    break;
  /* DEFAULT sollte nie erreicht werden */
  default:
    break;
  }
}