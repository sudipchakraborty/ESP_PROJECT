#include "ExtComm.h"
#include <ArduinoJson.h>  // Include ArduinoJson library
///////////////////////////

ExtComm::ExtComm()  // Constructor
{   
}

//__________________________________________________________________________________________________
void ExtComm::init()
{
    Serial2.begin(115200, SERIAL_8N1, RX2, TX2);  // Define TX2/RX2 for Serial2
}

//__________________________________________________________________________________________________
void ExtComm::sendJSON(String msg)
{
  StaticJsonDocument<200> doc;
  doc["message"] = msg;         
  doc["timestamp"] = millis(); 
  
  String jsonStr;
  serializeJson(doc, jsonStr);
  Serial2.println(jsonStr);  // Send JSON data to Serial2
}

//__________________________________________________________________________________________________
void ExtComm::send(String msg)
{
  Serial2.println(msg);  // ✅ Send raw message
}

//__________________________________________________________________________________________________
// ✅ Renamed function to `receive()`
void ExtComm::receive()
{
  if(Serial2.available()) 
  {
    char incomingChar = Serial2.read();  // ✅ Use local variable instead of global

    if (incomingChar == '\n') 
    { 
     // parseJson(receivedData);
      Serial.println(receivedData);
      receivedData = ""; // Clear buffer for next message
      
    } 
    else 
    {
      receivedData += incomingChar;
    }
  }
}

//__________________________________________________________________________________________________
void ExtComm::parseJson(String jsonString) 
{
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, jsonString);

  if (error) 
  {
    Serial.println("JSON Parsing Failed!");
    return;
  }

  String message = doc["message"];
  long timestamp = doc["timestamp"];
}
