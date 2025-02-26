#ifndef DEBUG_H
#define DEBUG_H
//////////////////////
 #include "Arduino.h"
/////////////////////
class debug
{
    public:
          debug();
      void init(); 
      void send(String msg);
      void send();
      void send(String msg, int val);
    private:        
      unsigned int count=0;
};
///////////////////////



#endif
