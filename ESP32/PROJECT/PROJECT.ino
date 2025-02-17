#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>

const char* ssid = "SUDIP_2.4G";
const char* password = "Sudip@07011977";

const char* remoteServer = "http://REMOTE_IP/receive_status"; // Change to actual remote server URL

WebServer server(80);
bool doorStatus = false; // false = closed, true = opened

const int ledPin = 13; // LED pin
const int buttonPin = 2; // Button pin
bool ledState = LOW;
unsigned long previousMillis = 0;
const long interval = 500; // 500ms interval for LED blink

// Static IP configuration
IPAddress staticIP(192, 168, 1, 100); // Desired static IP
IPAddress gateway(192, 168, 1, 1);    // Router's IP
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);         // DNS Server

void sendStatusToRemote() {
  HTTPClient http;
  http.begin(remoteServer);
  http.addHeader("Content-Type", "application/json");
  String statusJson = doorStatus ? "{\"status\": \"Door is Open\"}" : "{\"status\": \"Door is Closed\"}";
  int httpResponseCode = http.POST(statusJson);
  http.end();
}

void forwardToSerial(const String& message) {
  Serial.println(message);
}

void handleUnlock() {
  digitalWrite(buttonPin, HIGH);
  delay(500);
  digitalWrite(buttonPin, LOW);
  doorStatus = true;
  String response = "{\"status\": \"Door Opened\"}";
  server.send(200, "application/json", response);
  sendStatusToRemote();
  forwardToSerial(response);
}

void handleLock() {
  digitalWrite(buttonPin, LOW);
  doorStatus = false;
  String response = "{\"status\": \"Door Closed\"}";
  server.send(200, "application/json", response);
  sendStatusToRemote();
  forwardToSerial(response);
}

void handleStatus() {
  String response = doorStatus ? "{\"status\": \"Door is Open\"}" : "{\"status\": \"Door is Closed\"}";
  server.send(200, "application/json", response);
  sendStatusToRemote();
  forwardToSerial(response);
}

void handleRoot() {
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

void handleLedOn() {
  ledState = HIGH;
  digitalWrite(buttonPin, ledState);
  server.send(200, "application/json", "{\"status\": \"LED On\"}");
}

void handleLedOff() {
  ledState = LOW;
  digitalWrite(buttonPin, ledState);
  server.send(200, "application/json", "{\"status\": \"LED Off\"}");
}

void updateLED() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
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
  
  server.on("/", handleRoot);
  server.on("/open", handleUnlock);
  server.on("/close", handleLock);
  server.on("/status", handleStatus);
  server.on("/ledOn", handleLedOn);
  server.on("/ledOff", handleLedOff);
  
  server.begin();
}

void loop() {
  server.handleClient();
  updateLED();
}
