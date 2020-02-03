// Entradas e saídas digitais - push-button + led
// Apostila Eletrogate - KIT INTERMEDIATE

#define PinButton 8
#define ledPin 7

bool mystate = false;

void setup()
{
  pinMode(PinButton, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  delay(100);
}

void loop()
{
  if (digitalRead(PinButton) == HIGH) {
    delay(200);
    mystate = !mystate;

    if (mystate) {
    digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
    }
   
  }
 
  
}
