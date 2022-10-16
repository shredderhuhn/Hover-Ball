#include <Arduino.h>
#include <constants.h>
#include <HallSensor.h>
#include <HBString.h>

void printHelp() {
  Serial.println("Folgende Kommandos stehen zur Verfügung.");
  Serial.println("?               - Hilfe");
  Serial.println("ver?            - Version der Software");
  Serial.println("diag?           - Diagnoseausgabe");
  Serial.println("state?          - Zustandsmaschinenstatus abfragen:");
  Serial.println("                  0 - Einsetzen der Kugel/Init/Failure");
  Serial.println("                  1 - Kalibrierung der Kugel (minimale Sensorspannung");
  Serial.println("                  2 - Hochdrehen der HOhlschraube bis zum Sollwert");
  Serial.println("                  3 - Einschalten des Stromoffsets");
  Serial.println("                  4 - Soll=Ist und Einschalten der Regelung (autom. nach 5)");
  Serial.println("                  5 - Laufende Regelung");
  Serial.println("state!x         - Zustand setzen x = 0..5");
  Serial.println("next!           - nächster Zustand)");
  Serial.println("prev!           - vorhergehender Zustand");
  Serial.println("reset!          - alle Variablen rücksetzen (und in Zustand 0)"); 
  Serial.println("meas?           - gibt Sensormesswert aus"); 
  Serial.println("meas!xxx        - gibt Messwerte kontinuierlich als Rohwert aus xxx=[ON|OFF]"); 
  Serial.println("ref?            - zeigt eingestellten Sollwert an [1/100 mmm]"); 
  Serial.println("ref!xxxx        - stellt Sollwert ein [1/100 mm] xxxx = 1000..2000"); 
  Serial.println("offset?         - zeigt eingestellten Stromoffset an [digit]");
  Serial.println("offset!xxxx     - stellt Stromoffset ein: xxxx = 0..4095 [digit]");
  Serial.println("failure?        - Status der erkannten Fehler:");
  Serial.println("                  1 - Kugel verkehrt herum (Sensorspannung > 2.7V)");
  Serial.println("                  2 - Abstand der Kugel zum Magneten zu klein (Sensorspannung >2.3V");  
  Serial.println("                  3 - Abstand der Kugel zum Magneten zu groß (Sensorspannung <1.0V");  
  Serial.println("failure!        - Löscht alle erkannten Fehler");
  Serial.println("");
  Serial.println("Für alle Eingaben gilt: ON|VALID = 1 sowie OFF|INVALID = 0");
  
}

/// @brief 
/// @param status 
/// @param hall 
void serialinteraction(Status &status, HallSensor &hall){
      if (Serial.available()){        // Daten liegen an
    
    String msg = Serial.readString(); // Nachricht lesen
    Serial.print("Empfangene Message: ");
    Serial.println(msg);
    // parsen des Strings "msg" 
    //es könnte zeitaufwendig sein, jedes Mal eine neue Klasse zu instanziieren
    //daher machen wir die Klasse global (s. weiter oben)
    //InputString zerlegterString; 
    zerlegterString.ClearString();
    zerlegterString.SetString(msg); 
    
    // Switch-Anweisung funktioniert nur mit Integer, daher if-Anweisung
    if(zerlegterString.cmd == "help") {
      printHelp();
      
    } else if (zerlegterString.cmd == "ver") {
      SerialChn.print("Version: ");
      SerialChn.println(__FILE__);
      
    } else if (zerlegterString.cmd == "diag" && zerlegterString.get) {
      SerialChn.print("Diagnosestatus: ");
      if (diagnosis == ON) SerialChn.println("ON");
      if (diagnosis == OFF) SerialChn.println("OFF");
      
    } else if (zerlegterString.cmd == "diag" && zerlegterString.set) {
      if (zerlegterString.number[0] > 0) {
        diagnosis = ON;
        SerialChn.print("Diagnosestatus: ");
        SerialChn.println("ON");
      } else {
        diagnosis = OFF;
        SerialChn.print("Diagnosestatus: ");
        SerialChn.println("OFF");
      }
      
    } else if ((zerlegterString.cmd == "time" || zerlegterString.cmd == "date") && zerlegterString.get) {
      //gibt aktuelle Zeit und Zeitstatus und Datumsstatus aus
      printTimeToSet(rtcDay, rtcMonth, rtcYear, rtcHour, rtcMinute, rtcSecond);
      
    } else if ((zerlegterString.cmd == "time") && zerlegterString.set) {
      if (zerlegterString.number[0] > -1 && zerlegterString.number[1] > -1) {
        if (zerlegterString.number[2] == -1) zerlegterString.number[2] = 0;
        rtcHour = zerlegterString.number[0];
        rtcMinute = zerlegterString.number[1];
        rtcSecond = zerlegterString.number[2];
        printTimeToSet(rtcDay, rtcMonth, rtcYear, rtcHour, rtcMinute, rtcSecond);
      } else {
        SerialChn.println("falsche Zeitangabe.");
      }
      
    } else if ((zerlegterString.cmd == "date") && zerlegterString.set) {
      if (zerlegterString.number[0] > -1 && zerlegterString.number[1] > -1 && zerlegterString.number[2] > -1) {
        rtcDay = zerlegterString.number[0];
        rtcMonth = zerlegterString.number[1];
        rtcYear = zerlegterString.number[2];
        printTimeToSet(rtcDay, rtcMonth, rtcYear, rtcHour, rtcMinute, rtcSecond);
      } else {
        SerialChn.println("falsche Datumsangabe.");
      }
      
    } else if ((zerlegterString.cmd == "rtc") && zerlegterString.get) {
      //gibt aktuelle RTC - Zeit und Status aus
      printRTCTime(rtc.now());
      SerialChn.print("Zeit gültig: ");
      if (rtc.lostPower()) {
        SerialChn.println("nein");
        failure |= (uint8_t)1 << NOTIME;    //Fehlerbit ein
      } else {
        SerialChn.println("ja");   
        failure &= ~(uint8_t)1 << NOTIME;   //Fehlerbit aus
      }

    } else if ((zerlegterString.cmd == "rtc") && zerlegterString.set) {
      rtc.adjust(DateTime(rtcYear, rtcMonth, rtcDay, rtcHour, rtcMinute, rtcSecond));
      //gibt aktuelle RTC - Zeit und Status aus
      SerialChn.print("Zeit gesetzt. ");
      printRTCTime(rtc.now());
      
    } else if ((zerlegterString.cmd == "goCal") && zerlegterString.set) {
        goCal = TRUE;
        SerialChn.println("goCal gesetzt");
        failure |= (uint8_t)rtc.lostPower() << NOTIME; //prüfen, ob RTC gültig ist
        
    } else if ((zerlegterString.cmd == "goSun") && zerlegterString.set) {
        goSun = TRUE;
        SerialChn.println("goSun gesetzt");
        failure |= (uint8_t)rtc.lostPower() << NOTIME; //prüfen, ob RTC gültig ist
        
    } else if ((zerlegterString.cmd == "goStar") && zerlegterString.set) {
        goStar = TRUE;
        SerialChn.println("goStar gesetzt");
        failure |= (uint8_t)rtc.lostPower() << NOTIME; //prüfen, ob RTC gültig ist
        
    } else if ((zerlegterString.cmd == "auto") && zerlegterString.set) {
        //Timer initialisieren
        Timer1.initialize(); //1Mio us ist voreingestellt
        Timer1.attachInterrupt(countSeconds); //attachInterrupt startet den Zähler
        autoState = TRUE;
        failure |= (uint8_t)rtc.lostPower() << NOTIME; //prüfen, ob RTC gültig ist

    } else if ((zerlegterString.cmd == "failure") && zerlegterString.get) {
        SerialChn.print("Failure-Wert: ");
        SerialChn.println(failure);

    } else if ((zerlegterString.cmd == "failure") && zerlegterString.set) {
        failure = 0;
        SerialChn.println("Alle Fehler gelöscht");
    
    } else if ((zerlegterString.cmd == "clear") && zerlegterString.set) {
        goCal = FALSE;         // kalibriert die Uhr erst calStar, dann calSun
        goStar = FALSE;        // geht in Status calStar über
        goSun = FALSE;         // geht in Status calSun über
        validSun = FALSE;      // Sonnenzeiger ist kalibriert
        validStar = FALSE;     // Sternenscheibe ist kalibriert
        autoState = OFF;       // Variable zum automatischenn Weiterschalten 
        diagnosis = ON;        // Variable zur erweiterten Diagnose
        SerialChn.println("alle Controlvariablen zurückgesetzt");
        
        
    } else {
      //sollte nie auftreten, da immer cmd mindestens immer help enthält
      SerialChn.println("Message nicht verstanden.");
      printHelp();
    }
  }
}

/// @brief 
/// @param status 
/// @param hall 
void setLED(const Status &status, const HallSensor &hall){}