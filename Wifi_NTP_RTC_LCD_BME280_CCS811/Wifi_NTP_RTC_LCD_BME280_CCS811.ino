//-------------------Llibreries---------------------------------------------------------
//Wi-Fi
#include <WiFi.h>

//NTP
#include "time.h"

//RTC
#include "ESP32Time.h"

//MOD-LCD2.8RTP
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Board_Pinout.h"
#include "Wire.h"
#include "Adafruit_STMPE610.h"

//SEN0335
//BME280
#include "DFRobot_BME280.h"

//CCS811
#include "DFRobot_CCS811.h"

//-------------------Connexió a la WiFi (STATION)--------------------------------------
const char* ssid = "Enric";
const char* password = "5a303030324346433132393633";

//-------------------Connexió al servidor NTP------------------------------------------
// Direcció del servidor NTP que farem servir
const char* ntpServer = "europe.pool.ntp.org";
// Ajustem la nostra zona horàri, UTC+1.00 es correspon amb 3600 ms 
const long  gmtOffset_sec = 3600;
// Ajustem si compensem l'horari, és a dir, avancem i endarrerim el rellotge un cop l'any. 3600 si ho fem, 0 si no.
const int   daylightOffset_sec = 3600;

//-----------------------------RTC----------------------------------------------------
// Creació objecte RTC, podrem configurar i consultar diferents dades
ESP32Time rtc;

//-----------------------------MOD-LCD2.8RTP------------------------------------------
// Objecte amb nom tft, tipus Adafruit_ILI9341 i coma arguments passem els pins
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

//---------------------------CCS811---------------------------------------------------
//DFRobot_CCS811 CCS811;
DFRobot_CCS811 CCS811(&Wire, /*IIC_ADDRESS=*/0x5A);

//---------------------------BME280---------------------------------------------------
typedef DFRobot_BME280_IIC    BME;    
BME   bme(&Wire, 0x76);   // Direcció del sensor
#define SEA_LEVEL_PRESSURE    1015.0f

//--------------------------------SET UP-----------------------------------------------
void setup() {
  Serial.begin(115200);
  
  //-----------------------Connexió a la WiFi (STATION) 2------------------------------
  initWiFi();
  //  RRSI Received signal strength indication
  //  L'escala va del 0 al -100, el 0 és la recepció més forta mentre que el 100 la més dèbil
  //  https://teamdynamix.umich.edu/TDClient/47/LSAPortal/KB/ArticleDet?ID=1644
  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());

  //---------------------------NTP + RTC-----------------------------------------------
  //Inicialització i obtenció del temps
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); //Actualització RTC intern
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)){  // Passem la informació del RTC intern a l'estructura tm timeinfo
    rtc.setTimeStruct(timeinfo); // Actualització de la variable rtc passan com a paràmetre tm timeinfo
  }

  //-------------------Desconnexió de la WiFi (STATION)-----------------------------------
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  //-----------------------------MOD-LCD2.8RTP------------------------------------------
  // Inicialitzem la pantalla
  Wire.begin();
  tft.begin();
  pinMode(TFT_DC, OUTPUT);

  //---------------------------CCS811------------------------------------------------------
   while(CCS811.begin() != 0){
        Serial.println("failed to init chip, please check if the chip connection is fine");
    }
  
  //----------------------------BME280-----------------------------------------------------
  bme.reset();
  Serial.println("bme read data test");
  while(bme.begin() != BME::eStatusOK) {
    Serial.println("bme begin faild");
    printLastOperateStatus(bme.lastOperateStatus);
    delay(2000);
  }
  Serial.println("bme begin success");
  Serial.println("");
  
  delay(1000);
}

//---------------------------LOOP---------------------------------------------------------
void loop() {
  delay(1000);
  printLocalTime(); //monitor

  //-----------------------------Definició de MOD-LCD2.8RTP----------------------------------
  // Orientació de la pantalla, els valors van de 0 a 3
  tft.setRotation(1);
  // Color de fons de tota la pantalla, ILI9341_seguit de color que ens interessi
   tft.fillScreen(ILI9341_BLACK);
   // Color del text
  tft.setTextColor(ILI9341_WHITE);
   // Mida del text, 6 px de base per 8 px, en el cas de dos 12 x 16 pxs.
   tft.setTextSize(1);
  // On situem el text dins de la pantalla, (x,-y)
  tft.setCursor(0,0);
  // Text
  tft.print(rtc.getTime("%D, %H:%M:%S"));
  
  tft.setTextSize(2);
  // On situem el text dins de la pantalla, (x,-y)
  tft.setCursor(0,20);
  // Text
  tft.print("TFG Joaquim Anglada");

  //---------------------------CCS811------------------------------------------------------
  float co2 = CCS811.getCO2PPM();
  float tvoc = CCS811.getTVOCPPB();

  // Baseline, valor inicial al partir delqual es compara la resta
  //CCS811.writeBaseLine(0x447B); //Baseline per defecte
  CCS811.writeBaseLine(0xA46A); //calculada ""
    
  //monitor
  if(CCS811.checkDataReady() == true){
    Serial.println("======== start print ========");
    Serial.print("CO2: ");
    Serial.print(co2);
    Serial.print("ppm, TVOC: ");
    Serial.print(tvoc);
    Serial.println("ppb");

    } else {
       Serial.println("Data is not ready!");
    }

  //pantalla lcd
  tft.setTextSize(1);
  tft.setCursor(0,140);
  tft.print("CO2 (ppm): "); tft.setCursor(120,140);  tft.print(co2);
  tft.setCursor(0,160);
  tft.print("TVOC (ppb): ");  tft.setCursor(120,160);  tft.print(tvoc);
 
  //--------------------------------BME280-------------------------------------------------
  float   temp = bme.getTemperature();
  uint32_t    press = bme.getPressure();
  float   alti = bme.calAltitude(SEA_LEVEL_PRESSURE, press);
  float   humi = bme.getHumidity();

  //monitor  
  Serial.print("temperature (unit Celsius): "); Serial.println(temp);
  Serial.print("pressure (unit pa):         "); Serial.println(press);
  Serial.print("altitude (unit meter):      "); Serial.println(alti);
  Serial.print("humidity (unit percent):    "); Serial.println(humi);
  Serial.println("========  end print  ========");
  Serial.println("");

  //pantalla lcd
  tft.setTextSize(2);
  tft.setCursor(0,40);
  tft.print("=== Parametres ===");
  tft.setTextSize(1);
  tft.setCursor(0,60);
  tft.print("Temperatura (C): "); tft.setCursor(120,60); tft.print(temp);
  tft.setCursor(0,80);
  tft.print("Pressio (Pa):         ");  tft.setCursor(120,80); tft.print(press);
  tft.setCursor(0,100);
  tft.print("Altitud (m):      "); tft.setCursor(120,100); tft.print(alti);
  tft.setCursor(0,120);
  tft.print("Humitat rel. (%):    "); tft.setCursor(120,120);  tft.print(humi);
}


//--------------Funció: Connexió i comprovació de l'accés a la WiFi (STATION)------------------
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(" Connected");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
}

//---------------Funció: Mostrar el dia i l'hora pel monitor-------------------------------
void printLocalTime() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

//-----------------Funció: Mostrar l'estat operacional del sensor pel monitor--------------
void printLastOperateStatus(BME::eStatus_t eStatus) {
  switch(eStatus) {
  case BME::eStatusOK:    Serial.println("everything ok"); break;
  case BME::eStatusErr:   Serial.println("unknow error"); break;
  case BME::eStatusErrDeviceNotDetected:    Serial.println("device not detected"); break;
  case BME::eStatusErrParameter:    Serial.println("parameter error"); break;
  default: Serial.println("unknow status"); break;
  }
}


//-----------------------------INFORMACIÓ ADDICIONAL----------------------------------------
// Station -> Conexió al router de casa
// NTP time -> mitjantçant la conexió wifi s'obté el temps real de qualsevol zona del món
// RTC -> Real time clock, té com a objectiu mantenir el temps després de la configuració 
//        inicial amb NTP (també funciona en cas de que no hi hagi conexió wifi

// Desconnectar i reconnectar a la wifi, possibles opcions a tenir en compte: 
// https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/#8
