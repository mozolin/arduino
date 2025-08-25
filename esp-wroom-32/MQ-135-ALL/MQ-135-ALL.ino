
#include <MQUnifiedsensor.h>

#define CHIP_NAME "ESP-WROOM-32"
#define GAS_VOLTAGE_RESOLUTION 5
//-- MQ135: pin
#define MQ135_SENSOR_GPIO 2
#define GAS_SENSOR_TYPE "MQ-135"
#define GAS_ADC_BIT_RESOLUTION 10
//-- RS / R0 = 3.6 ppm  
#define MQ135_RATIO_CLEANAIR 3.6
//-- ratio: 1023/4096 (analog output Arduino / analog output ESP32)
#define MQ135_ESP32_ADC 0.249755859375

MQUnifiedsensor MQ135(
	CHIP_NAME,
	GAS_VOLTAGE_RESOLUTION,
	GAS_ADC_BIT_RESOLUTION,
	MQ135_SENSOR_GPIO,
	GAS_SENSOR_TYPE
);

float
	mq135Analog,
	mq135GasCO,
	mq135GasAlcohol,
	mq135GasCO2,
	mq135GasToluen,
	mq135GasNH4,
	mq135GasAceton;


void setup() {
  //Init the serial port communication - to debug the library
  //Serial.begin(9600); //Init serial port
  Serial.begin(115200); //Init serial port

  //Set math model to calculate the PPM concentration and the value of constants
  MQ135.setRegressionMethod(1); //_PPM =  a*ratio^b
  
  /*****************************  MQ Init ********************************************/ 
  //Remarks: Configure the pin of arduino as input.
  /************************************************************************************/ 
  MQ135.init();
  /* 
    //If the RL value is different from 10K please assign your RL value with the following method:
    MQ135.setRL(10);
  */
  /*****************************  MQ CAlibration ********************************************/ 
  // Explanation: 
  // In this routine the sensor will measure the resistance of the sensor supposedly before being pre-heated
  // and on clean air (Calibration conditions), setting up R0 value.
  // We recomend executing this routine only on setup in laboratory conditions.
  // This routine does not need to be executed on each restart, you can load your R0 value from eeprom.
  // Acknowledgements: https://jayconsystems.com/blog/understanding-a-gas-sensor
  Serial.print("Calibrating A0 - please wait.");
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ135.update(); // Update data, the arduino will read the voltage from the analog pin
    calcR0 += MQ135.calibrate(MQ135_RATIO_CLEANAIR);
    Serial.print(".");
  }
  MQ135.setR0(calcR0/10);
  Serial.println("  done!.");
  
  if(isinf(calcR0)) {Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0 == 0){Serial.println("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply"); while(1);}

}

void loop() {
  MQ135.update(); // Update data, the arduino will read the voltage from the analog pin

  //mq135Analog = analogRead(MQ135_SENSOR_GPIO) * MQ135_ESP32_ADC;
  mq135Analog = analogRead(MQ135_SENSOR_GPIO);
  
  MQ135.setA(605.18); MQ135.setB(-3.937); // Configure the equation to calculate CO concentration value
  mq135GasCO = MQ135.readSensor() * MQ135_ESP32_ADC; // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

  MQ135.setA(77.255); MQ135.setB(-3.18); //Configure the equation to calculate Alcohol concentration value
  mq135GasAlcohol = MQ135.readSensor() * MQ135_ESP32_ADC; // SSensor will read PPM concentration using the model, a and b values set previously or from the setup

  MQ135.setA(110.47); MQ135.setB(-2.862); // Configure the equation to calculate CO2 concentration value
  mq135GasCO2 = MQ135.readSensor() * MQ135_ESP32_ADC + 400; // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

  MQ135.setA(44.947); MQ135.setB(-3.445); // Configure the equation to calculate Toluen concentration value
  mq135GasToluen = MQ135.readSensor() * MQ135_ESP32_ADC; // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
  
  MQ135.setA(102.2 ); MQ135.setB(-2.473); // Configure the equation to calculate NH4 concentration value
  mq135GasNH4 = MQ135.readSensor() * MQ135_ESP32_ADC; // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

  MQ135.setA(34.668); MQ135.setB(-3.369); // Configure the equation to calculate Aceton concentration value
  mq135GasAceton = MQ135.readSensor() * MQ135_ESP32_ADC; // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
  // Note: 400 Offset for CO2 source: https://github.com/miguel5612/MQSensorsLib/issues/29
  /*
  Motivation:
  We have added 400 PPM because when the library is calibrated it assumes the current state of the
  air as 0 PPM, and it is considered today that the CO2 present in the atmosphere is around 400 PPM.
  https://www.lavanguardia.com/natural/20190514/462242832581/concentracion-dioxido-cabono-co2-atmosfera-bate-record-historia-humanidad.html
  */

  Serial.print("MQ135 (GPIO=");
  Serial.print(MQ135_SENSOR_GPIO);
  Serial.print("): Analog = ");
  Serial.print(mq135Analog);

  Serial.print(", CO = ");
  Serial.print(mq135GasCO);
  Serial.print(", Alcohol = ");
  Serial.print(mq135GasAlcohol);
  Serial.print(", CO2 = ");
  Serial.print(mq135GasCO2);
  Serial.print(" ppm, Toluen = ");
  Serial.print(mq135GasToluen);
  Serial.print(", NH4 = ");
  Serial.print(mq135GasNH4);
  Serial.print(", Aceton = ");
  Serial.print(mq135GasAceton);
    
  Serial.println("");

  

  /*
    Exponential regression:
  GAS      | a      | b
  CO       | 605.18 | -3.937  
  Alcohol  | 77.255 | -3.18 
  CO2      | 110.47 | -2.862
  Toluen  | 44.947 | -3.445
  NH4      | 102.2  | -2.473
  Aceton  | 34.668 | -3.369
  */

  delay(3000); //Sampling frequency
}
