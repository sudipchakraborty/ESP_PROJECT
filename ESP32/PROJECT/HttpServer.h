#ifndef HTTPSERVER_H
#define HTTPSERVER_H

//////////////////////
#include "Arduino.h"

// Declare global variables (defined in HttpServer.cpp)
extern const char* ssid;
extern const char* password;


class HttpServer
{
    public:
               HttpServer();
          void init();
          void handle();

          void handleRoot();  
          void handleOpen();
          void handleClose();
          void handleStatus();  
          void SendResponse(String msg);
          bool Command_received(); 
          void Command_purge();

          String command="";  
          String value="";
          String Status="";  
    private: 
 
};

#endif
