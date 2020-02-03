#include "LedControl.h"
#include "binary.h"

#define humidity_sensor A0
int analog_humidity;
int humidity;

LedControl lc = LedControl(12,11,10,1);

// delay time between faces
unsigned long delaytime=1000;

const uint64_t IMAGES[] = {
  0x003c420000660000, //happy
  0x003c420000996600, //happy1
  0x003c420000e90600, //happy2
  0x00007e0000664400, //sad1
  0x00423c0000664400, //sad2
  0x003c243800363600  //surprise
};


void setup() {

  Serial.begin(9600);
  pinMode(humidity_sensor, INPUT);
  Serial.println("Inicializando sensor...");
  
  lc.shutdown(0,false);
  // Set brightness to a medium value
  lc.setIntensity(0,0);
  // Clear the display
  lc.clearDisplay(0);  

  delay(2000);
}

void displayImage(uint64_t image) {
  for (int i = 0; i < 8; i++) {
    byte row = (image >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
      lc.setLed(0, i, j, bitRead(row, j));
    }
  }
}


void surprise() {
  displayImage(IMAGES[4]);
  delay(1500);
}

void happy() {
  displayImage(IMAGES[1]);
  delay(delaytime);
  displayImage(IMAGES[2]);
  delay(delaytime);
}

void sad() {
  displayImage(IMAGES[3]);
  delay(delaytime);
  
  displayImage(IMAGES[4]);
  delay(delaytime);
}


void loop() {

  analog_humidity = analogRead(humidity_sensor);
  humidity = map(analog_humidity,450,380,0,100);


  //384 (wet) - 442 (dry)
  //391 (wet) - 443 (dry) - output led
  //Mostra o valor da porta analogica no serial monitor
  
  Serial.print("Umidade : ");
  Serial.println(humidity);


  if (humidity < 60) {
    sad();
  } else {
    happy();
  }

  
}
