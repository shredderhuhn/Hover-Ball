/*
 * Class HBString provides a parsed string with different substrings identified
 * It is chosen form the InputString-Lib of the Astrolab-Project - and modified
 * Changes to previous version: ENTER is not a valid "String"
 * 
 * Gunter Wiedemann
 *
 */

//don't forget the endif-Statement
#ifndef HBSTRING_h
#define HBSTRING_h

#include <arduino.h>


class HBString
{

private:

  void ParseWholeString(void);
  int nKeyWords = 12; //sizeof liefert Anzahl aller Zeichen (56 tbc)
  String validKeyWord[13] = {"help","ver","state","next","prev","stop","ref",\
                             "meas","offset","reset","failure","failure"};
  String wholeString;
  bool InKeyWord(String);

public:

  HBString();
  HBString(String);
  void ClearString();
  void SetString(String);
  bool stringIsValid;  //zeigt an, ob der wholeString gültig ist
  String cmd;
  bool set; //set=0 -> ? set=1 ->!
  bool get; //get=0 -> ! get=1 ->?
  int number[3]; //enthaelt bis zu 3 Werte danach - brauchen wir hier nicht (nur einen)

};



#endif
