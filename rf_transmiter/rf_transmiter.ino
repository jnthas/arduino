
#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif


RH_ASK driver(2000, D2, D4, D5); //2 = Rx 4 = Tx

 
const int pinoBotao = 2; //PINO DIGITAL UTILIZADO PELO PUSH BUTTON

void setup()
{
  #ifdef RH_HAVE_SERIAL
      Serial.begin(9600);    // Debugging only
  #endif
      if (!driver.init())
  #ifdef RH_HAVE_SERIAL
           Serial.println("init failed");
  #else
    ;
  #endif


  pinMode(D3, INPUT_PULLUP);
}

void loop()
{
  if (digitalRead(D3) == LOW){ 
      send();
  }
   
}



void send() 
{
    const char *msg = "012345678912";
    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    delay(200);  
}
