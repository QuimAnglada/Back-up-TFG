// Incloure les llibreires necessaries
  #include "BluetoothSerial.h"

// Comprobació si el Bluetooth està activat correctament
  #if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
  #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
  #endif

// Es realitza una instància (és una relació específica de qualsevol objecta, no em queda clar que és)
  BluetoothSerial SerialBT;

// Creació variable relés (a quin GPIO estan conectats)
const int relePin1 = 32;
const int relePin2 = 33;

// Creació variable per emmagatzemar els missatges rebuts
String message = "";

// Creació variable per emmagatzemar els missatges provinents del mòbil (bluetooth serial)
char incomingChar;

void setup() {
// baud rate (número d'unitats de senyal per segon, velocitat de transmissió)
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");  

// Inicialitzem el relé com una sortida
  pinMode(relePin1, OUTPUT);
  pinMode(relePin2, OUTPUT);
}

void loop() {
// Zona del codi des d'on s'envia i és rep la informació. 
// S'hi ha informació disponible s'envia de l'ESP32-EVB al dispositiu connectat, en aquest cas el mòbil.
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }

// S'hi ha bytes per llegir en el mòbil, aquest són escrits en el monitor. 
  if (SerialBT.available()) {
    char incomingChar = SerialBT.read();
    if (incomingChar != '\n'){            // != diferent
      message += String(incomingChar);    // += concatenar (no entenc perquè concatenem)
    }
    else{
      message = "";
    }
    Serial.write(SerialBT.read());
  }

// Check received message and control output accordingly
  if (message =="rele1_on"){
    digitalWrite(relePin1, HIGH);
  }
  else if (message =="rele1_off"){
    digitalWrite(relePin1, LOW);
  }
  else if (message =="rele2_on"){
    digitalWrite(relePin2, HIGH);
  }
  else if (message =="rele2_off"){
    digitalWrite(relePin2, LOW);
  }
}
