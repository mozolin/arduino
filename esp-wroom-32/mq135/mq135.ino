#include "MQ135.h"

#define ANALOG_GPIO    2

// The load resistance on the board
#define RLOAD 10.0
// Calibration resistance at atmospheric CO2 level
#define RZERO 76.63

// The load resistance on the board
//#define RLOAD 22.0
// Calibration resistance at atmospheric CO2 level
//#define RZERO 879.13


//MQ135 gasSensor = MQ135(ANALOG_GPIO, RZERO, RLOAD);
MQ135 gasSensor = MQ135(ANALOG_GPIO);

int sensorValue;
int digitalValue;

void setup()
{
  //Serial.begin(9600);
  Serial.begin(115200);
  pinMode(2, INPUT);
}

void loop()
{
  float t = 25.2, h = 26.0;
  
  sensorValue = analogRead(ANALOG_GPIO); // read analog input pin 0
  Serial.print("Analog value = ");
  Serial.print(sensorValue, DEC);
  float ppm = gasSensor.getPPM();
  Serial.print(", PPM = ");
  Serial.print(ppm);
  float correctedPPM = gasSensor.getCorrectedPPM(t, h);
  Serial.print(", CorrectedPPM = ");
  Serial.print(correctedPPM);
  float rZero = gasSensor.getRZero();
  Serial.print(", rZero = ");
  Serial.println(rZero);
  delay(3000); // wait 100ms for next reading
}
