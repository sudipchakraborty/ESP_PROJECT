#ifndef LEDST_H
#define LEDST_H
//////////////////////
#include "Arduino.h"


/////////////////////
class ledST
{
    public:
           ledST();
      void init(char pin);    
      void update(); 
    private:    

      int  ledPin = 2; // LED pin
      bool ledState = LOW;
      unsigned long previousMillis = 0;
      long interval = 500; // 500ms interval for LED blink
};
///////////////////////



#endif
