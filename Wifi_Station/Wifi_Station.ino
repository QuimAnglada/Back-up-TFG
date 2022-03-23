// Station 
// Conexió al router de casa

#include <WiFi.h>

// Replace with your network credentials (STATION)
const char* ssid = "Enric";
const char* password = "5a303030324346433132393633";

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  initWiFi();
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());
}

void loop() {
  // put your main code here, to run repeatedly:
}

// Desconnectar i reconnectar a la wifi, possibles opcions a tenir en compte: 
// https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/#8
