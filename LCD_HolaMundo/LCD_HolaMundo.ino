//Llibreries
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Board_Pinout.h"
#include "Wire.h"
#include "Adafruit_STMPE610.h"

// Objecte amb nom tft, tipus Adafruit_ILI9341 i coma arguments passem els pins
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

void setup() {
  Serial.print ("1");
  delay(1000);
  Serial.begin(115200);
  Serial.print ("1aaa");
  // Inicialitzem la pantalla
  tft.begin();
  Serial.print ("2");
  Wire.begin();
  pinMode(TFT_DC, OUTPUT);
  delay(1000);
 }

void loop() {
   // Orientació de la pantalla, els valors van de 0 a 3
  tft.setRotation(0);
  Serial.print ("3");
  // Color de fons de tota la pantalla, ILI9341_seguit de color que ens interessi
   tft.fillScreen(ILI9341_BLACK);
   Serial.print ("4");
  // Color del text
  tft.setTextColor(ILI9341_WHITE);
  Serial.print ("5");
  // Mida del text, 6 px de base per 8 px, en el cas de dos 12 x 16 pxs.
  tft.setTextSize(2);
  Serial.print ("6");
  // On situem el text dins de la pantalla, (x,-y)
  tft.setCursor(0,0);
  Serial.print ("7");
  // Text
  tft.print("Hola, han passat: ");
  Serial.print ("8");
  tft.setCursor(75,250);
  tft.print("segons");
  tft.setTextColor(ILI9341_WHITE,ILI9341_BLACK);
  tft.setTextSize(8);
  tft.setCursor(30,100);
  tft.print(millis()/1000);
  delay(10);
}

// Tot el que sigui permanent ho deixem al set up mentre que les dades dinàmiques estarant ubicades al loop.
// Això és degut a que la pantalla no és prou ràpida per canviar les dades. 
