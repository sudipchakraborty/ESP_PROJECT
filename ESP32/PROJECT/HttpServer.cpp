#include "HttpServer.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

const char* ssid = "SUDIP_LAB";
const char* password = "Anindita@01092016";

WebServer server(80);

// Static IP configuration
IPAddress staticIP(192, 168, 0, 100);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);

HttpServer::HttpServer() {}

// Initialize the server module
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

    // Route Handlers
    server.on("/", [this]() { this->handleRoot(); });
    server.on("/open", [this]() { this->handleOpen(); });
    server.on("/close", [this]() { this->handleClose(); });
    server.on("/status", [this]() { this->handleStatus(); });

    server.begin();
}

// Handle server requests
void HttpServer::handle() {
    server.handleClient();
}

// Root Page with Buttons for Door Control
void HttpServer::handleRoot() {
    Serial.println("Root Request Received..");

    String html = "<!DOCTYPE html><html><head>"
                  "<title>Smart Door Control</title>"
                  "<meta name='viewport' content='width=device-width, initial-scale=1'>"
                  "<script>"
                  "function toggleDoor(state) {"
                  " fetch(state === 'open' ? '/open' : '/close');"
                  "}"
                  "</script>"
                  "</head><body>"
                  "<h2>Smart Door Control</h2>"
                  "<button onclick=\"toggleDoor('open')\">Door Open</button>"
                  "<button onclick=\"toggleDoor('close')\">Door Close</button>"
                  "</body></html>";

    server.send(200, "text/html", html);
}

// Handle Open Door
void HttpServer::handleOpen() {
    command = "Door";
    value = "Open";
    Status = "Open"; // Store status
    SendResponse("Door Opened");
    Serial.println("Door Open Request Received.");
}

// Handle Close Door
void HttpServer::handleClose() {
    command = "Door";
    value = "Close";
    Status = "Close"; // Store status
    SendResponse("Door Closed");
    Serial.println("Door Close Request Received.");
}

// Handle Status Request
void HttpServer::handleStatus() {
    SendResponse(Status);
    Serial.println("Door Status Request Received.");
}

// Send JSON Response
void HttpServer::SendResponse(String msg) {
    StaticJsonDocument<200> doc;
    doc["message"] = msg;
    doc["status"] = Status; // Include door status in response
    doc["timestamp"] = millis();

    String jsonStr;
    serializeJson(doc, jsonStr);
    server.send(200, "application/json", jsonStr);
}

// Check if a command was received
bool HttpServer::Command_received() {
    return !command.isEmpty();
}

// Clear received command
void HttpServer::Command_purge() {
    command = "";
    value = "";
}
