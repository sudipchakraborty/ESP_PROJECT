#include <WiFi.h>

// Wi-Fi credentials
const char* ssid = "AirFiber-Sudip";
const char* password = "Sudip@07011977";

WiFiServer server(80);                   // Web server on port 80

HardwareSerial SerialMega(2); // Use UART2 on ESP32 (RX2: GPIO16, TX2: GPIO17)

void setup() {
  // Initialize Serial connections
  SerialMega.begin(9600, SERIAL_8N1, 16, 17); // UART2: RX2=GPIO16, TX2=GPIO17
  Serial.begin(9600);                         // Debug monitor

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
}

void loop() {
  WiFiClient client = server.available(); // Check for incoming client connections

  if (client) {
    Serial.println("Client connected");
    String clientRequest = "";

    while (client.connected()) {
      if (client.available()) {
        // Read client data
        char c = client.read();
        clientRequest += c;
        Serial.write(c); // Print the received character to the Serial Monitor
        client.write(c);
      }

      // Check if there's data from Arduino Mega
      if (SerialMega.available()) {
        String megaData = SerialMega.readStringUntil('\n');
        Serial.print("Data from Mega: ");
        Serial.println(megaData);

        // Send Mega data as a response to the client
        client.println("Data from Mega: " + megaData);
      }
    }

    // Print the complete client request
    Serial.println("\nClient Request:");
    Serial.println(clientRequest);

    // Send a response to the client
    String response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
    response += "ESP32 received your request: \n";
    response += clientRequest;
    response += "\nThis is a response from ESP32.";
    client.print(response);

    // Close the client connection
    client.stop();
    Serial.println("Client disconnected");
  }
}
