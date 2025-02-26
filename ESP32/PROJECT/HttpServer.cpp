#include "HttpServer.h"
#include <WiFi.h>
#include <WebServer.h>

///////////////////////////
// ✅ Define global variables **only here**
const char* ssid = "SUDIP_LAB";  // ✅ Properly defined here
const char* password = "Anindita@01092016";  // ✅ Properly defined here

WebServer server(80);

// Static IP configuration
IPAddress staticIP(192, 168, 0, 100); // Desired static IP
IPAddress gateway(192, 168, 0, 1);    // Router's IP
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);         // DNS Server

HttpServer::HttpServer()  // Constructor
{   
}

//__________________________________________________________________________________________________
void HttpServer::init()
{
  WiFi.mode(WIFI_STA);
  if (!WiFi.config(staticIP, gateway, subnet, dns)) {
    Serial.println("Failed to configure static IP");
  }
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) { 
    delay(1000); 
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("Assigned IP Address: ");
  Serial.println(WiFi.localIP());
  
  // Use lambda functions to reference class member functions
  server.on("/", [this]() { this->handleRoot(); });
  server.on("/open", [this]() { this->handleUnlock(); });
  server.on("/close", [this]() { this->handleLock(); });
  server.on("/status", [this]() { this->handleStatus(); });
  server.on("/ledOn", [this]() { this->handleLedOn(); });
  server.on("/ledOff", [this]() { this->handleLedOff(); });
  
  server.begin();
}

//__________________________________________________________________________________________________
void HttpServer::handle()
{
   server.handleClient();
}

//__________________________________________________________________________________________________
void HttpServer::handleUnlock() {
  String response = "{\"status\": \"Door Opened\"}";
  server.send(200, "application/json", response);
}

//__________________________________________________________________________________________________
void HttpServer::handleLock() {
  String response = "{\"status\": \"Door Closed\"}";
  server.send(200, "application/json", response);
}

//__________________________________________________________________________________________________
void HttpServer::handleStatus() {
  String response = "{\"status\": \"OK\"}";
  server.send(200, "application/json", response);
}

//__________________________________________________________________________________________________
void HttpServer::handleRoot() {  
  Serial.println("Request Received..");

  String html = "<!DOCTYPE html><html><head>"
                "<title>Smart Door Control</title>"
                "<meta name='viewport' content='width=device-width, initial-scale=1'>"
                "<script>"
                "function toggleLED(state) {"
                " fetch(state === 'on' ? '/ledOn' : '/ledOff');"
                "}"
                "</script>"
                "</head><body>"
                "<h2>Smart Door Control</h2>"
                "<button onclick=\"toggleLED('on')\">ON</button>"
                "<button onclick=\"toggleLED('off')\">OFF</button>"
                "</body></html>";
  server.send(200, "text/html", html);
}

//__________________________________________________________________________________________________
void HttpServer::handleLedOn() {
  server.send(200, "application/json", "{\"status\": \"LED On\"}");
}

//__________________________________________________________________________________________________
void HttpServer::handleLedOff() {
  server.send(200, "application/json", "{\"status\": \"LED Off\"}");
}
