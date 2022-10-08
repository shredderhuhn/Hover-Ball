/* 
 * Diese Library testet die Library/Klasse HBString.
 * 
 * Gunter Wiedemann
 * 04.10.2022
 * 
 */

#include <HBStringTest.h>
#include <HBString.h>
#include <Arduino.h>

String startString = "Test";
HBString zerlegterString(startString);
String msg = "";

void printQuote(String str) {
    Serial.print("\"");
    Serial.print(str);
    Serial.print("\"");
}

void printlnQuote(String str) {
    printQuote(str);
    Serial.println();
}

void printHBStringStatus(String str) {

    Serial.print("Der String lautet:");
    printlnQuote(str);
    Serial.println("Die Instanz von HBString hat folgende public Properties:");
    zerlegterString.ClearString();
    zerlegterString.SetString(str);
    Serial.print("zerlegterString.cmd = ");
    printlnQuote(zerlegterString.cmd);
    Serial.print("zerlegterString.stringIsValid = ");
    Serial.println(zerlegterString.stringIsValid);
    Serial.print("zerlegterString.number[0]");
    Serial.println(zerlegterString.number[0]);
    Serial.print("zerlegterString.number[1]");
    Serial.println(zerlegterString.number[1]);
    Serial.print("zerlegterString.number[2]");
    Serial.println(zerlegterString.number[2]);
    Serial.print("zerlegterString.set = ");
    Serial.println(zerlegterString.set);
    Serial.print("zerlegterString.get = ");
    Serial.println(zerlegterString.get);
    
}



// Diese Funktion muss in der main.ccp in die Funktion setup() eingebunden werden
 void hbStringSetup() {

    Serial.begin(9600);
    while(!Serial.available()) {} //warten, dass der serielle Monitor online ist
    Serial.readString();
    Serial.println("Jetzt geht es los.");

    Serial.print("Der String lautet:");
    printlnQuote(startString);
    Serial.print("zerlegterString.cmd = ");
    printlnQuote(zerlegterString.cmd);
    Serial.print("zerlegterString.stringIsValid = ");
    Serial.println(zerlegterString.stringIsValid);
    Serial.print("zerlegterString.number[0] = ");
    Serial.println(zerlegterString.number[0]);
    Serial.print("zerlegterString.number[1] = ");
    Serial.println(zerlegterString.number[1]);
    Serial.print("zerlegterString.number[2] = ");
    Serial.println(zerlegterString.number[2]);
    Serial.print("zerlegterString.set = ");
    Serial.println(zerlegterString.set);
    Serial.print("zerlegterString.get = ");
    Serial.println(zerlegterString.get);

    Serial.println("Drücke ENTER für Fortsetzung");
    while(!Serial.available()) {} //warten auf Enter
    Serial.readString();
    printHBStringStatus("help");

 }

// Diese Funktion muss in der main.ccp in die Funktion loop() eingebunden werden
 void hbStringLoop(){

    Serial.println("Gib einen String ein für Fortsetzung");
    while(!Serial.available()) {} //warten auf Enter
    msg = Serial.readString();
    Serial.print("Stringlänge = ");
    Serial.println(msg.length());
    Serial.print("Ascii 1. Zeichen: ");
    Serial.println((int)msg[0]);
    Serial.print("Ascii 2. Zeichen: ");
    Serial.println((int)msg[1]);
    printHBStringStatus(msg);

 }