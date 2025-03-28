#include <SPI.h>
#include <Ethernet.h>

// MAC address (this should be a unique address, you can generate one or use any random address)
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Create an EthernetClient object
EthernetClient client;

void setup() {
  // Start the serial communication
  Serial.begin(9600);

  // Start Ethernet connection
  Serial.println("Starting Ethernet...");

  // Initialize Ethernet shield with DHCP
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // If DHCP fails, assign a static IP address
    Ethernet.begin(mac, IPAddress(192, 168, 1, 177));
  }

  // Wait for the Ethernet shield to initialize
  delay(1000);

  // Print the IP address assigned by DHCP or static IP if DHCP fails
  Serial.print("IP Address: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // Check if there is any input from Serial Monitor
  if (Serial.available() > 0) {
    int value = Serial.parseInt(); // Read the integer from Serial input

    if (value > 0) {
      Serial.print("Sending value: ");
      Serial.println(value);

      // Create the JSON data to send
      String jsonData = "{\"value\": " + String(value) + "}";

      // Replace with your FastAPI server's IP address and port
      String serverIP = "192.168.31.107";  // FastAPI server IP
      int serverPort = 8000;  // FastAPI server port

      // Try to connect to the FastAPI server
      if (client.connect(serverIP.c_str(), serverPort)) {
        Serial.println("Connected to API!");

        // Send the HTTP POST request
        client.println("POST /data HTTP/1.1");
        client.print("Host:192.168.31.107:8080 ");
        client.println(serverIP);
        client.println("Content-Type: application/json");
        client.println();  // Blank line before the data
        client.println(jsonData);  // Send the JSON data

        Serial.println("Data sent.");
      } else {
        Serial.println("Connection failed.");
      }

      // Wait for and print any response from the server
      while (client.available()) {
        char c = client.read();
        Serial.print(c);
      }

      // Close the connection after the request is sent
      client.stop();
    }
  }
}