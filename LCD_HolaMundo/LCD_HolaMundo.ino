//Llibreries
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_DC 7    
#define TFT_CS 10   //Chip select

// Objecte amb nom tft, tipus Adafruit_ILI9341 i coma arguments passem els pins
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);


void setup() {
  // Inicialitzem la pantalla
  tft.begin();
  // Orientació de la pantalla, els valors van de 0 a 3
  tft.setRotation(0);
  // Color de fons de tota la pantalla, ILI9341_seguit de color que ens interessi
  tft.fillScreen(ILI9341_RED);
  // Color del text
  tft.setTextColor(ILI9341_WHITE);
  // Mida del text, 6 px de base per 8 px, en el cas de dos 12 x 16 pxs.
  tft.setTextSize(2);
  // On situem el text dins de la pantalla, (x,-y)
  tft.setCursor(0,0);
  // Text
  tft.print("Hola, han passat: ");
  tft.setCursor(75,250);
  tft.print("segons");
}

void loop() {
  tft.setTextColor(ILI9341_BLUE,ILI9341_RED);
  tft.setTextSize(8);
  tft.setCursor(30,100);
  tft.print(millis()/1000);
  delay(10);
}

// Tot el que sigui permanent ho deixem al set up mentre que les dades dinàmiques estarant ubicades al loop.
// Això és degut a que la pantalla no és prou ràpida per canviar les dades. 
