#ifndef EXTCOMM_H
#define EXTCOMM_H
//////////////////////
#define TX2 17  // TX Pin for Serial2
#define RX2 16  // RX Pin for Serial2
#include "Arduino.h"
/////////////////////
class ExtComm
{
    public:
           ExtComm();
           void init(); 
           void send(String msg); 
           void sendJSON(String msg);
           void receive();   // ✅ Add receive() function
           void parseJson(String jsonString);
    private: 
           String receivedData;  // ✅ Changed from `char` to `String`
};
///////////////////////

#endif
