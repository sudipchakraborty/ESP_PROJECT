#include "SmartDoor.h"
#include "debug.h"
#include "HttpServer.h"
#include <ArduinoJson.h>
#include "ExtComm.h"
////////////////////////////////////////////////////
debug dbg;
HttpServer srv;
ExtComm mega;
//////////////////////////////////////
SmartDoor::SmartDoor()  // Constructor
{   
}
//__________________________________________________________________________________________________________________________________________________________________
void SmartDoor::init()
{
  dbg.init();
  srv.init();  
  mega.init();
}
//__________________________________________________________________________________________________________________________________________________________________
 void SmartDoor::Handle()
{
    srv.handle();
   // mega.send("dsfsdfsdfsdf");
    if(srv.Command_received())
    {
        StaticJsonDocument<200> doc;
        doc["command"] = srv.command;   
        doc["value"] = srv.value;       
        doc["timestamp"] = millis(); 
     
        String jsonStr;
        serializeJson(doc, jsonStr);
        mega.send(jsonStr);   
        srv.Command_purge();
        Serial.println(jsonStr);
    }

    mega.receive();
     
}
//__________________________________________________________________________________________________________________________________________________________________

