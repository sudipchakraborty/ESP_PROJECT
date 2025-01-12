#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "AirFiber-Sudip";
const char* password = "Sudip@07011977";


#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

// Define server and LED pin
WiFiServer server(80);
const int ledPin = 2;
const int switchPin = 4; // GPIO pin for the switch

bool ledState = false;
bool switchState = false;

// Server IP for the client to send data to
String serverURL = "http://localhost"; // Replace "localhost" with the ESP32 server IP

// Function to handle client-side switch status updates
void sendSwitchStatus() {
  static unsigned long lastSendTime = 0;
  unsigned long currentMillis = millis();

  // Send the switch status every 1 second
  if (currentMillis - lastSendTime >= 1000) {
    lastSendTime = currentMillis;

    // Read the switch state
    switchState = digitalRead(switchPin);

    // Send the switch state to the server
    HTTPClient http;
    http.begin(serverURL + "/switch-status");
    http.addHeader("Content-Type", "application/json");

    // Create JSON payload
    StaticJsonDocument<128> payload;
    payload["switchState"] = switchState;
    String jsonString;
    serializeJson(payload, jsonString);

    // POST request
    int httpResponseCode = http.POST(jsonString);
    if (httpResponseCode > 0) {
      Serial.println("Switch status sent: " + String(jsonString));
      Serial.println("Response: " + http.getString());
    } else {
      Serial.println("Error in sending switch status: " + String(httpResponseCode));
    }

    http.end();
  }
}

void setup() {
  // Initialize Serial
  Serial.begin(115200);

  // Initialize the LED and switch pins
  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP); // Assuming a pull-up resistor is used

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");

  // Start the server
  server.begin();
  Serial.println("Server started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Handle incoming client requests
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    String request = client.readStringUntil('\r');
    Serial.println("Request: " + request);
    client.flush();

    // Handle switch status updates
    if (request.indexOf("/switch-status") != -1) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: application/json");
      client.println("Connection: close");
      client.println();
      client.println("{\"status\": \"Switch status received\"}");
    }
    // Default response for unknown routes
    else {
      client.println("HTTP/1.1 404 Not Found");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println();
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.println("<h1>404 - Page Not Found</h1>");
      client.println("<p><a href='/'>Back to Home</a></p>");
      client.println("</html>");
    }

    delay(1);
    Serial.println("Client disconnected");
  }

  // Run the client-side task to send the switch status
  sendSwitchStatus();
}





