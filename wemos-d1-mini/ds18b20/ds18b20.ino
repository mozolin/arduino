/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com  
*********/

#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = A0;
int idx = 0;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  // Start the DS18B20 sensor
  sensors.begin();

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, 1);
  delay(300);
  digitalWrite(LED_BUILTIN, 0);
  delay(300);
  
  idx++;
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(idx);
  Serial.print(") Ds18b20 (esp32): ");
  Serial.print(temperatureC);
  Serial.print("ºC, ");
  Serial.print(temperatureF);
  Serial.println("ºF");
  delay(3000);
}
