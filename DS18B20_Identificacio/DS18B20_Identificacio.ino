//Needed libraries
#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 13;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  // Start the DS18B20 sensor
  sensors.begin();

    // Buscamos los sensores conectados
    Serial.println("Buscando dispositivos...");
    Serial.println("Encontrados: ");
    int numeroSensoresConectados = sensors.getDeviceCount();
    Serial.print(numeroSensoresConectados);
    Serial.println(" sensores");
 
    // Si hemos encontrado uno mostramos su dirección
    if(numeroSensoresConectados==1){
        
        // Tipo definido como una array de 8 bytes (uint8_t)
        DeviceAddress sensorTemperatura;
        // Obtenemos dirección
        sensors.getAddress(sensorTemperatura, 1);
 
        // Mostamos por el monitor serie
        Serial.print("Sensor encontrado: ");
 
        // Recorremos los 8 bytes del identificador único
        for (uint8_t i = 0; i < 8; i++)
        {
          // Si solo tiene un dígito rellenamos con un cero a la izquierda
          if (sensorTemperatura[i] < 16) Serial.print("0");
 
          // Mostramos los datos que van en HEXADECIMAL
          Serial.print(sensorTemperatura[i], HEX);
        }
    }    
}

void loop() {
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  Serial.print(temperatureF);
  Serial.println("ºF");
  delay(5000);
}
