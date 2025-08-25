
int redPin = 0;
int bluePin = 3;

void setup()
{
  Serial.begin(115200);
  Serial.println("ПРИВЕТ!");
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}


void loop()
{
  digitalWrite (redPin,HIGH);
  digitalWrite (bluePin,LOW);
  delay (1000);
  digitalWrite (redPin,LOW);
  digitalWrite (bluePin,HIGH);
  delay (1000);
}
