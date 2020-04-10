#include <RCSwitch.h>
#include <Servo.h>

RCSwitch mySwitch = RCSwitch();

Servo motorA;                           
Servo motorB;                           

void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
  motorA.attach(5); 
  motorB.attach(6); 
}

void loop() {
  if (mySwitch.available()) {

    Serial.println(mySwitch.getReceivedValue());
    
    if (mySwitch.getReceivedValue() == 7897281) {   // Botao A      
      Serial.println("Motor A");
      motorA.write(45); 
      delay(1000); 
      motorA.write(125); 
      
    } else if (mySwitch.getReceivedValue() == 7897282) {    // Botao B
      Serial.println("Motor B");
      motorB.write(45); 
      delay(1000);     
    }
    
    mySwitch.resetAvailable();
  }
}
