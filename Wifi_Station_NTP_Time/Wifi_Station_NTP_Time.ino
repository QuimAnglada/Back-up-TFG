// Station 
// Conexió al router de casa

#include <WiFi.h>
#include "time.h"

// Replace with your network credentials (STATION)
const char* ssid = "Enric";
const char* password = "5a303030324346433132393633";

// Direcció del servidor NTP que farem servir
const char* ntpServer = "europe.pool.ntp.org";
// Ajustem la nostra zona horàri, UTC+1.00 es correspon amb 3600 ms 
const long  gmtOffset_sec = 3600;
// Ajustem si compensem l'horari, és a dir, avancem i endarrerim el rellotge dos cops l'any. 3600 si ho fem, 0 si no.
const int   daylightOffset_sec = 0;

void setup() {
  Serial.begin(115200);
  initWiFi();
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
  
}

void loop() {
  delay(1000);
  printLocalTime();
}

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

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

// Desconnectar i reconnectar a la wifi, possibles opcions a tenir en compte: 
// https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/#8
