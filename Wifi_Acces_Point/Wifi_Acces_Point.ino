// Access point
// Els dispositius (estacions) del voltant es poden connectar a la red Wi-Fi que crea la placa ESP32-EVB

// Import required libraries
#include <WiFi.h>
#include <Wire.h>

// Set your access point network credentials
const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();
  
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
}

void loop(){
  }

// AP IP address: 192.168.4.1 obtinguda
