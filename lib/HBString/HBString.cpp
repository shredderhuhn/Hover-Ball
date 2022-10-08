#include "HBString.h"    //muss hier eingefügt werden, findet .h sonst nicht
#include <arduino.h>

HBString::HBString(void)
{
  ClearString();
  wholeString = "";
}

HBString::HBString(String aString)
{
  ClearString();
  wholeString = aString;
  ParseWholeString();
}

void HBString::ClearString(void)
{
  stringIsValid = false;  //zeigt an, ob der wholeString gültig ist
  wholeString = "";
  cmd = "";
  set = 0; //set=0 -> ? set=1 ->!
  get = 0; //get=0 -> ! get=1 ->?
  number[0] = -1; //only valid for date! and time!
  number[1] = -1;
  number[2] = -1;
  
}

void HBString::SetString(String aString)
{
  ClearString();
  wholeString = aString;
  ParseWholeString();
}

void HBString::ParseWholeString(void)
{
  //zuerst get oder set
  String restOfString;
  int delimiterIndex = wholeString.indexOf('!'); //set?
  if (delimiterIndex >= 0) //set?
  {
    set = true;
    get = false;
    //Kommando rausloesen
    cmd = wholeString.substring(0, delimiterIndex);
    restOfString = wholeString.substring(delimiterIndex + 1); // To the end of the string

    //restlichen String auf Eingaben untersuchen
    //zunaechst ersetzen wir ggf. Doppelpunkte durch Einzelpunkte,
    //damit wir später nur einen Delimiter haben
    restOfString.replace(":", ".");
    int i = 0;
    while (i < 3) // max. Anzahl der durch Delimiters getrennte Groessen
    {
      delimiterIndex = restOfString.indexOf(".");
      if (delimiterIndex > -1)
      {
        number[i] = restOfString.substring(0,delimiterIndex).toInt();
        restOfString = restOfString.substring(delimiterIndex+1);
        i++;
      } 
      else
      {
        //if (isDigit(restOfString[0]) && (restOfString.length() > 0))
        if (isDigit(restOfString[0]))
        {
          number[i] = restOfString.toInt();
        }
        i = 3;
      }
    }
  } 
  else //get?
  {
    delimiterIndex = wholeString.indexOf('?'); //get wird automatisch angenommen
    set = false;
    get = true;
    cmd = wholeString.substring(0, delimiterIndex);
  }
  //Wenn kein Keywort identifiziert wurde, dann Hilfe ausgeben
  if (InKeyWord(cmd) == false) cmd = "help";
  stringIsValid = true;
}

bool HBString::InKeyWord(String aString)
{
 for (int i = 0; i < nKeyWords; i++)
  {
    if (aString.equals(validKeyWord[i])) return true;
  }
  return false;
}
