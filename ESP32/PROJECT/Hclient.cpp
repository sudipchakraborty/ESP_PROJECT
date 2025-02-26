#include "Hclient.h"
#include <WiFi.h>
#include <HTTPClient.h>

// ///////////////////////////
HTTPClient http;
const char* remoteServer = "http://REMOTE_IP/receive_status"; // ✅ Defined only once

extern bool doorStatus;  // ✅ Declare doorStatus (it should be defined elsewhere)

Hclient::Hclient()  // Constructor
{   
}

//__________________________________________________________________________________________________
void Hclient::init()
{
    // Initialization code for pin (if needed)
}

//__________________________________________________________________________________________________
void Hclient::send() 
{
  http.begin(remoteServer);
  http.addHeader("Content-Type", "application/json");
  String statusJson = doorStatus ? "{\"status\": \"Door is Open\"}" : "{\"status\": \"Door is Closed\"}";
  int httpResponseCode = http.POST(statusJson);
  http.end();
}
