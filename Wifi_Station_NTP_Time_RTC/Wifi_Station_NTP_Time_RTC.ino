// Station -> Conexió al router de casa
// NTP time -> mitjantçant la conexió wifi s'obté el temps real de qualsevol zona del món
// RTC -> Real time clock, té com a objectiu mantenir el temps després de la configuració 
//        inicial amb NTP (també funciona en cas de que no hi hagi conexió wifi)

#include <WiFi.h>
#include "time.h"
#include <ESP32Time.h>

// Replace with your network credentials (STATION)
const char* ssid = "Enric";
const char* password = "5a303030324346433132393633";

// Direcció del servidor NTP que farem servir
const char* ntpServer = "europe.pool.ntp.org";
// Ajustem la nostra zona horàri, UTC+1.00 es correspon amb 3600 ms 
const long  gmtOffset_sec = 3600;
// Ajustem si compensem l'horari, és a dir, avancem i endarrerim el rellotge dos cops l'any. 3600 si ho fem, 0 si no.
const int   daylightOffset_sec = 0;

// Creació objecte RTC, podrem configurar i consultar diferents dades
ESP32Time rtc;

void setup() {
  Serial.begin(115200);
  initWiFi();
  //  RRSI Received signal strength indication
  //  L'escala va del 0 al -100, el 0 és la recepció més forta mentre que el 100 la més dèbil
  //  https://teamdynamix.umich.edu/TDClient/47/LSAPortal/KB/ArticleDet?ID=1644
  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); //Actualització RTC intern
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)){  // Passem la informació del RTC intern a l'estructura tm timeinfo
    rtc.setTimeStruct(timeinfo); // Actualització de la variable rtc passan com a paràmetre tm timeinfo
  }
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
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
  Serial.println(" Connected");
  Serial.print("Local IP: ");
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
