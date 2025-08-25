#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
 
int Vpin = A0; // «+» подсоединяем к аналоговому выводу А0
float voltage;
float volts;

void setup() // процедура setup
{
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("\nSSD1306 allocation failed!\n"));
    for(;;); // Don't proceed, loop forever
  } else {
    Serial.println(F("\nSSD1306 successfully initialized!\n"));
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  //display.display();
  //delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
}

void measure()
{
	voltage = analogRead(Vpin); 
  // получаем измеряемое напряжение
  volts = voltage / 1023 * 5.0; 
  
  display.clearDisplay();

  Serial.print(F("voltage = "));
  Serial.println(volts);

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(32,0);
  display.print(volts);
  display.print(F("V"));
  display.display();
}
 
void loop() // процедура loop
{ 
  measure();
  delay(2000);
}
