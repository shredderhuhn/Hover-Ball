#include <Arduino.h>
#include <constants.h>
#include <HallSensor.h>
#include <HBString.h>
#include <interaction.h>
#include <controller.h>


void printHelp() {
  Serial.println("Folgende Kommandos stehen zur Verfügung.");
  Serial.println("?               - Hilfe");
  Serial.println("ver?            - Version der Software: Datum und Zeit");
  Serial.println("diag?           - Diagnoseausgabe");
  Serial.println("state?          - Zustandsmaschinenstatus abfragen:");
  Serial.println("                  0 - Diagnosestatus: Interaktiv mit serieller Schnittstell");
  Serial.println("                  1 - Einschalten der Regelung");
  Serial.println("                  2 - Regelung läuft");
  Serial.println("state!x         - Zustand setzen x = 0..2");
  Serial.println("next!           - nächster Zustand)");
  Serial.println("prev!           - vorhergehender Zustand");
  Serial.println("reset!          - alle Variablen rücksetzen (und in Zustand 0)"); 
  Serial.println("meas?           - gibt Sensormesswert aus"); 
  Serial.println("meas!xxx        - gibt Messwerte kontinuierlich als Rohwert aus xxx = [ON|OFF]"); 
  Serial.println("ref?            - zeigt eingestellten Sollwert an [1/100 mmm]"); 
  Serial.println("ref!xxxx        - stellt Sollwert ein [1/100 mm] xxxx = 1000..2000"); 
  Serial.println("offset?         - zeigt eingestellten Stromoffset an [digit]");
  Serial.println("offset!xxxx     - stellt Stromoffset ein: xxxx = 0..4095 [digit]");
  Serial.println("curr!xxx        - schaltet den Strom mit dem offset-Wert ein/aus xxx = [ON|OFF]");
  Serial.println("test!xx.yy      - testet yy Iterationen des Controllers mit xx als RawValue des Hallsensors.");
  Serial.println("");
  Serial.println("Für alle Eingaben gilt: ON|VALID = 1 sowie OFF|INVALID = 0");
  
}

void printStatus(Status &status, HallSensor &hall){
    Serial.println("Werte des Statusregisters:");
    Serial.print("state = ");
    Serial.println(status.state);
    Serial.print("setpoint = ");
    Serial.println(status.setpoint);
    Serial.print("offset = ");
    Serial.println(status.offset);
    Serial.println("Alle Sensorwerte:");
    hall.DispAllAtSerial();
    Serial.println();
}

void printCtrl(void) {
  Serial.println("Werte der Controller-Variable:");
  Serial.print("setpoint = ");
  Serial.println(ctrl.setpoint);
  Serial.print("offset = "); 
  Serial.println(ctrl.offset);
  Serial.print("error (-(Soll-Ist) = ");
  Serial.println(ctrl.error);
  Serial.print("errorTilde[k] = ");
  Serial.println(ctrl.errorTildeK);
  Serial.print("errorTilde[k-1] = ");
  Serial.println(ctrl.errorTildeKM1);
  Serial.print("u1[k] = ");
  Serial.println(ctrl.u1K);
  Serial.print("u2[k] = ((ctrl.errorTildeKM1 * K1Z) >> 10) + ((ctrl.u2KM1 * K2Z) >> 12); =");
  Serial.println(ctrl.u2K);
  Serial.println("  bestehend aus: ");
  Serial.print("   ((ctrl.errorTildeKM1 * K1Z) >> 10) = ");
  Serial.println((ctrl.errorTildeKM1 * K1Z) >> 10);
  Serial.print("   ((ctrl.u2KM1 * K2Z) >> 12); = ");
  Serial.println((ctrl.u2KM1 * K2Z) >> 12);
  Serial.print("   (ctrl.u2KM1 * K2Z) = ");
  Serial.println((ctrl.u2KM1 * K2Z));
  Serial.print("u2[k-1] = ");
  Serial.println(ctrl.u2KM1);
  Serial.print("u[k] = ");
  Serial.println(ctrl.u);
  Serial.print("dac0 = ");
  Serial.println(ctrl.dac0);
  Serial.print("dac1 = ");
  Serial.println(ctrl.dac1);  
}

void testCtrl(int raw, int iter, HallSensor &hall) {
  int zwischenspeicher = hall.GetRawValue();
  hall.SetRawValue(raw);
  Serial.println("Folgende Sensorwerte werden simuliert:");
  hall.DispAllAtSerial();
  initController();
  Serial.println("Folgende Controllerwerte sind initial:");
  printCtrl();
  for(int i = 0; i<iter; i++) {
    Serial.println("");
    Serial.print("Durchlauf Nr. ");
    Serial.println(i+1);
    calcController(hall.CalcDistanceMagnetVsBallPoly());
    printCtrl();
    Serial.println("");
    Serial.println("Beliebige Taste drücken ...");
    Serial.println("");
    while(!Serial.available()) {};
    Serial.readString();
  }
  Serial.println("Hallsensor wird auf letzten Rohwert zurückgesetzt");
  hall.SetRawValue(zwischenspeicher);
}

void testHandler(void (*handlerToTest)(void))
{
        // Zeit testen
        Serial.println("");
        Serial.println("Beliebige Taste drücken zum Testen der Rechenzeit...");
        Serial.println("");
        while(!Serial.available()) {};
        Serial.readString();
        int starttime = millis();
        for (int i = 0; i<10000; i++) {
          (*handlerToTest)();
        }
        int endtime = millis();
        Serial.print("Zeit für 10000 iterationen: ");
        Serial.print(endtime - starttime);
        Serial.println("ms");
        Serial.println("");
        Serial.println("Beliebige Taste drücken zum Rücksetzen...");
        Serial.println("");
        while(!Serial.available()) {};
        Serial.readString();
}

void serialinteraction(Status &status, HallSensor &hall, void (*handlerToTest)(void)) {

  static HBString zerlegterString;
  static bool measFlag = 0;

  if (measFlag) {
    hall.ReadRawValue();
    Serial.print("Rohwert: ");
    Serial.println(hall.GetRawValue());
    Serial.print("Abstand vom Magnet [1/100 mm]: ");
    Serial.println(hall.CalcDistanceMagnetVsBallPoly());
  }

  delay(500);

  if (Serial.available()){        // Daten liegen an
    
    String msg = Serial.readString(); // Nachricht lesen
    Serial.print("Empfangene Message: ");
    Serial.println(msg);
    zerlegterString.ClearString();
    zerlegterString.SetString(msg); 
    
    // Switch-Anweisung funktioniert nur mit Integer, daher if-Anweisung
    if(zerlegterString.cmd == "help") {
      printHelp();
      
    } else if (zerlegterString.cmd == "ver") {
      Serial.print("Version vom: ");
      Serial.print(__DATE__);
      Serial.print("  ");
      Serial.println(__TIME__);
      
    } else if (zerlegterString.cmd == "diag" && zerlegterString.get) {
      printStatus(status, hall);
      printCtrl();
      
    } else if (zerlegterString.cmd == "state" && zerlegterString.set) {
      status.state = constrain(zerlegterString.number[0],MINSTATE, MAXSTATE);
      Serial.print("neuer state = ");
      Serial.println(status.state);
      
    } else if (zerlegterString.cmd == "state" && zerlegterString.get) {
      Serial.print("state = ");
      Serial.println(status.state);
      
    } else if ((zerlegterString.cmd == "next") && zerlegterString.set) {
      Serial.print("alter state = ");
      Serial.println(status.state);
      int tempState = status.state + 1;
      status.state = constrain(tempState,MINSTATE, MAXSTATE);
      Serial.print("neuer state = ");
      Serial.println(status.state);
      
    } else if ((zerlegterString.cmd == "prev") && zerlegterString.set) {
      Serial.print("alter state = ");
      Serial.println(status.state);
      int tempState = status.state - 1;
      status.state = constrain(tempState, MINSTATE, MAXSTATE);
      Serial.print("neuer state = ");
      Serial.println(status.state);
      
    } else if ((zerlegterString.cmd == "reset") && zerlegterString.set) {
      resetState(status);
      Serial.println("Statusvariable zurückgesetzt.");
      resetController();
      Serial.println("Controllervariable zurückgesetzt");

    } else if ((zerlegterString.cmd == "meas") && zerlegterString.get) {
      hall.ReadRawValue();
      hall.DispAllAtSerial();
      
    } else if ((zerlegterString.cmd == "meas") && zerlegterString.set) {
      if (zerlegterString.number[0] > 0) {
        measFlag = ON;
        Serial.print("Measurement-Status: ");
        Serial.println("ON");
      } else {
        measFlag = OFF;
        Serial.print("Measurement-Status: ");
        Serial.println("OFF");
      }
        
    } else if ((zerlegterString.cmd == "ref") && zerlegterString.get) {
        Serial.print("Sollwert Abstand Magnet - Ball = ");
        Serial.print(status.setpoint);
        Serial.println(" mm/100");
        
    } else if ((zerlegterString.cmd == "ref") && zerlegterString.set) {
      if (zerlegterString.number[0] > 0) {
        status.setpoint = constrain(zerlegterString.number[0], MINSETPOINT, MAXSETPOINT);
        Serial.print("Sollwert Abstand Magnet - Ball = ");
        Serial.print(status.setpoint);
        Serial.println(" mm/100");

      } else {
        Serial.println("keine Änderung des Sollwerts.");

        Serial.print("Sollwert Abstand Magnet - Ball = ");
        Serial.print(status.setpoint);
        Serial.println(" mm/100");
        
      }       
    } else if ((zerlegterString.cmd == "offset") && zerlegterString.get) {
        Serial.print("Stromoffset = ");
        Serial.print(status.offset);
        Serial.println(" digits");

    } else if ((zerlegterString.cmd == "offset") && zerlegterString.set) {
      if (zerlegterString.number[0] > 0) {
        status.offset = constrain(zerlegterString.number[0], MINCURRENTOFFSET, MAXCURRENTOFFSET);
        Serial.print("Stromoffset = ");
        Serial.print(status.offset);
        Serial.println(" digits");

      } else {
        Serial.println("keine Änderung des Stromoffsets.");
        Serial.print("Stromoffset = ");
        Serial.print(status.offset);
        Serial.println(" digits");
      }

    } else if ((zerlegterString.cmd == "curr") && zerlegterString.set) {
      if (zerlegterString.number[0] > 0) {
        setCurrent();
        Serial.println("Strom ist an.");
      } else {
        resetCurrent();
        Serial.println("Strom ist aus.");
      }


    
    } else if ((zerlegterString.cmd == "test") && zerlegterString.set) {
        int hallValue = constrain(zerlegterString.number[0], 0, 4095);
        int iterations = constrain(zerlegterString.number[1], 1, 100);
        testCtrl(hallValue, iterations, hall);
        testHandler(handlerToTest);
    } else {
      //sollte nie auftreten, da immer cmd mindestens immer help enthält
      Serial.println("Message nicht verstanden.");
      printHelp();
    }
  }
}
