/// Llibreries necessaries
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>                                                      // necessari pel servidor, per arrancar un servidor necessitem un servei i una característica amb els seus propis UUIDs
#include <BLE2902.h>                                                        // necessari si té la propietat de NOTIFY, et diu si les notificacions estan o no habilitades

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

// Definició pel servei i les característiques                              // UUID generats aleatoriament https://www.uuidgenerator.net/
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"          // són les dades dels sensors
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"          // subcategoria per sota dels sensors que engloba les propietats, valors i descriptors.
                                                                            // Tot servei, característica i descriptor té un UUID únic. 
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");                                     // funció  Serial.println("") permet escriure en  el monitor

  BLEDevice::init("ESP32-EVB");                                             // creació del dispositiu BLE i assignació del nom del dispositiu, es veu des de l'app del mòbil (nRF Connect for Mobile)
  BLEServer *pServer = BLEDevice::createServer(); 
  pServer->setCallbacks(new MyServerCallbacks());
  //pServer = BLEDevice::createServer();                                      // creació del servidor BLE
  
  BLEService *pService = pServer->createService(SERVICE_UUID);              // creació del servei llegint el UUID introduit anteriorment
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(      // creació de les característiques del BLE (servei associat al pService acabat de definir)
                                         CHARACTERISTIC_UUID,               // associem al pService una característica (amb l'UUID corresponent) i dos propietats (READ i WIRTE per poder modificar els valors)
                                         BLECharacteristic::PROPERTY_READ | // READ el client li demana el servidor que li entregui un paquet de dades (server to client)
                                         BLECharacteristic::PROPERTY_WRITE | //WRITE el client li envia informació al servidor (client to server9
                                         BLECharacteristic::PROPERTY_NOTIFY |// NOTIFY el server notifica al client quan rep nous valor, actualitza cst
                                         BLECharacteristic::PROPERTY_INDICATE //INDICATE el client li fa saber al servidor que ha rebut la informació
                                       );

  pCharacteristic->addDescriptor(new BLE2902());                            // creació de la descripció del BLE (no entenc que fa)
  pCharacteristic->setValue("Hello World says Quim");                       // assignació del valor de la característica
  pService->start();                                                        // arranquem el servei
  
BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();                 // el dispositiu es fa visible perquè pugui ser reconegut per la resta  
  pAdvertising->addServiceUUID(SERVICE_UUID);                               
  
pAdvertising->setScanResponse(false);                                       // busca respostes de dispositius interessats 
  pAdvertising->setMinPreferred(0x06);                                      // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
  // Notificació del valor canviat
  if (deviceConnected) {
        pCharacteristic->setValue((uint8_t*)&value, 4);                     // el 4 és el número de bytes que estem enviant, uint8_t fa referència al valor màxim que un byte pot emmagatzemar 255    
        pCharacteristic->notify();
        value++;
        delay(300); 
  }
  
  // Connexió
  if (deviceConnected && !oldDeviceConnected) {
          // do stuff here on connecting
          oldDeviceConnected = deviceConnected;
      }
  
  // Desconnexió 
  if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
      }
    }

//NOTES EXTRES
// Tot i canviar els dos UUID em continuen sortint els anteriors en el mòbil(SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b" i CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8")
//#include <stdio.h>                                                          // per poder publicar un text més un valor de les característiques
//char buffer[32];                                                            // un buffer és un emmagatzament temporal de informaicó
