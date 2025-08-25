/*
  MQUnifiedsensor Library - reading an MQ135

  Demonstrates the use a MQ135 sensor.
  Library originally added 01 may 2019
  by Miguel A Califa, Yersson Carrillo, Ghiordy Contreras, Mario Rodriguez
 
  Added example
  modified 23 May 2019
  by Miguel Califa 

  Updated library usage
  modified 26 March 2020
  by Miguel Califa 

  Wiring:
  https://github.com/miguel5612/MQSensorsLib_Docs/blob/master/static/img/MQ_Arduino.PNG
  Please make sure arduino A0 pin represents the analog input configured on #define pin

 This example code is in the public domain.

*/

//Include the library
#include <MQUnifiedsensor.h>

//Definitions
#define placa "Arduino UNO"
#define Voltage_Resolution 5
#define pin0 A0 //Analog input 0 of your arduino
#define pin1 A1 //Analog input 0 of your arduino
#define type "MQ-135" //MQ135
#define ADC_Bit_Resolution 10 // For arduino UNO/MEGA/NANO
#define RatioMQ135CleanAir 3.6//RS / R0 = 3.6 ppm  
//#define calibration_button 13 //Pin to calibrate your sensor

//Declare Sensor
MQUnifiedsensor MQ135_0(placa, Voltage_Resolution, ADC_Bit_Resolution, pin0, type);
MQUnifiedsensor MQ135_1(placa, Voltage_Resolution, ADC_Bit_Resolution, pin1, type);

void setup() {
  //Init the serial port communication - to debug the library
  //Serial.begin(9600); //Init serial port
  Serial.begin(115200); //Init serial port

  //Set math model to calculate the PPM concentration and the value of constants
  MQ135_0.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ135_1.setRegressionMethod(1); //_PPM =  a*ratio^b
  
  /*****************************  MQ Init ********************************************/ 
  //Remarks: Configure the pin of arduino as input.
  /************************************************************************************/ 
  MQ135_0.init();
  MQ135_1.init();
  /* 
    //If the RL value is different from 10K please assign your RL value with the following method:
    MQ135_0.setRL(10);
    MQ135_1.setRL(10);
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
    MQ135_0.update(); // Update data, the arduino will read the voltage from the analog pin
    calcR0 += MQ135_0.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }
  MQ135_0.setR0(calcR0/10);
  Serial.println("  done!.");
  
  if(isinf(calcR0)) {Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0 == 0){Serial.println("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply"); while(1);}



  Serial.print("Calibrating A1 - please wait.");
  float calcR1 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ135_1.update(); // Update data, the arduino will read the voltage from the analog pin
    calcR1 += MQ135_0.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }
  MQ135_1.setR0(calcR1/10);
  Serial.println("  done!.");
  
  if(isinf(calcR1)) {Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR1 == 0){Serial.println("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply"); while(1);}
  /*****************************  MQ CAlibration ********************************************/ 
  //Serial.println("** Values from MQ-135 ****");
  //Serial.println("|    CO   |  Alcohol |   CO2  |  Toluen  |  NH4  |  Aceton  |");  
}

void loop() {
  MQ135_0.update(); // Update data, the arduino will read the voltage from the analog pin

  MQ135_0.setA(605.18); MQ135_0.setB(-3.937); // Configure the equation to calculate CO concentration value
  float CO_0 = MQ135_0.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

  MQ135_0.setA(77.255); MQ135_0.setB(-3.18); //Configure the equation to calculate Alcohol concentration value
  float Alcohol_0 = MQ135_0.readSensor(); // SSensor will read PPM concentration using the model, a and b values set previously or from the setup

  MQ135_0.setA(110.47); MQ135_0.setB(-2.862); // Configure the equation to calculate CO2 concentration value
  float CO2_0 = MQ135_0.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

  MQ135_0.setA(44.947); MQ135_0.setB(-3.445); // Configure the equation to calculate Toluen concentration value
  float Toluen_0 = MQ135_0.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
  
  MQ135_0.setA(102.2 ); MQ135_0.setB(-2.473); // Configure the equation to calculate NH4 concentration value
  float NH4_0 = MQ135_0.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

  MQ135_0.setA(34.668); MQ135_0.setB(-3.369); // Configure the equation to calculate Aceton concentration value
  float Aceton_0 = MQ135_0.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
  //Serial.print("|   "); Serial.print(CO); 
  //Serial.print("   |   "); Serial.print(Alcohol);
  // Note: 400 Offset for CO2 source: https://github.com/miguel5612/MQSensorsLib/issues/29
  /*
  Motivation:
  We have added 400 PPM because when the library is calibrated it assumes the current state of the
  air as 0 PPM, and it is considered today that the CO2 present in the atmosphere is around 400 PPM.
  https://www.lavanguardia.com/natural/20190514/462242832581/concentracion-dioxido-cabono-co2-atmosfera-bate-record-historia-humanidad.html
  */
  //Serial.print("   |   "); Serial.print(CO2 + 400); 
  //Serial.print("   |   "); Serial.print(Toluen); 
  //Serial.print("   |   "); Serial.print(NH4); 
  //Serial.print("   |   "); Serial.print(Aceton);
  //Serial.println("   |");

  int mq135Analog_0 = analogRead(pin0);
  Serial.print("Analog_0 = ");
  Serial.print(mq135Analog_0);
  Serial.print(" ppm");

  Serial.print(", CO_0 = ");
  Serial.print(CO_0);
  Serial.print(", Alcohol_0 = ");
  Serial.print(Alcohol_0);
  Serial.print(", CO2_0 = ");
  Serial.print(CO2_0 + 400);
  Serial.print(", Toluen_0 = ");
  Serial.print(Toluen_0);
  Serial.print(", NH4_0 = ");
  Serial.print(NH4_0);
  Serial.print(", Aceton_0 = ");
  Serial.print(Aceton_0);
    
  Serial.println("");

  
  MQ135_1.update(); // Update data, the arduino will read the voltage from the analog pin

  MQ135_1.setA(605.18); MQ135_1.setB(-3.937); // Configure the equation to calculate CO concentration value
  float CO_1 = MQ135_1.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

  MQ135_1.setA(77.255); MQ135_1.setB(-3.18); //Configure the equation to calculate Alcohol concentration value
  float Alcohol_1 = MQ135_1.readSensor(); // SSensor will read PPM concentration using the model, a and b values set previously or from the setup

  MQ135_1.setA(110.47); MQ135_1.setB(-2.862); // Configure the equation to calculate CO2 concentration value
  float CO2_1 = MQ135_1.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

  MQ135_1.setA(44.947); MQ135_1.setB(-3.445); // Configure the equation to calculate Toluen concentration value
  float Toluen_1 = MQ135_1.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
  
  MQ135_1.setA(102.2 ); MQ135_1.setB(-2.473); // Configure the equation to calculate NH4 concentration value
  float NH4_1 = MQ135_1.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

  MQ135_1.setA(34.668); MQ135_1.setB(-3.369); // Configure the equation to calculate Aceton concentration value
  float Aceton_1 = MQ135_1.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
  //Serial.print("|   "); Serial.print(CO); 
  //Serial.print("   |   "); Serial.print(Alcohol);
  // Note: 400 Offset for CO2 source: https://github.com/miguel5612/MQSensorsLib/issues/29
  /*
  Motivation:
  We have added 400 PPM because when the library is calibrated it assumes the current state of the
  air as 0 PPM, and it is considered today that the CO2 present in the atmosphere is around 400 PPM.
  https://www.lavanguardia.com/natural/20190514/462242832581/concentracion-dioxido-cabono-co2-atmosfera-bate-record-historia-humanidad.html
  */
  //Serial.print("   |   "); Serial.print(CO2 + 400); 
  //Serial.print("   |   "); Serial.print(Toluen); 
  //Serial.print("   |   "); Serial.print(NH4); 
  //Serial.print("   |   "); Serial.print(Aceton);
  //Serial.println("   |");

  int mq135Analog_1 = analogRead(pin1);
  Serial.print("Analog_1 = ");
  Serial.print(mq135Analog_1);
  Serial.print(" ppm");

  Serial.print(", CO_1 = ");
  Serial.print(CO_1);
  Serial.print(", Alcohol_1 = ");
  Serial.print(Alcohol_1);
  Serial.print(", CO2_1 = ");
  Serial.print(CO2_1 + 400);
  Serial.print(", Toluen_1 = ");
  Serial.print(Toluen_1);
  Serial.print(", NH4_1 = ");
  Serial.print(NH4_1);
  Serial.print(", Aceton_1 = ");
  Serial.print(Aceton_1);
    
  Serial.println("");
  Serial.println("********************");


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
