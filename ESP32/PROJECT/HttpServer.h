#ifndef HTTPSERVER_H
#define HTTPSERVER_H

//////////////////////

// Declare global variables (defined in HttpServer.cpp)
extern const char* ssid;
extern const char* password;


class HttpServer
{
    public:
           HttpServer();
           void init();
           void handle();

           void handleUnlock();
           void handleLock();
           void handleStatus();
           void handleLedOn();
           void handleLedOff();
           void handleRoot();
           
    private:     
};

#endif
