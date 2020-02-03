#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"

/* Network credentials */
#define WIFI_SSID "2G_NETVIRTUA_102"
#define WIFI_PASS "3313906800"    

#define SERIAL_BAUDRATE 115200

/* Belkin WeMo emulation */
fauxmoESP fauxmo;

/* Set Relay Pins */
#define LIGHT_1 D1
#define LIGHT_2 D2
#define LIGHT_3 D3

void setup() 
{
   Serial.begin(SERIAL_BAUDRATE);

   //Set relay pins to outputs
   pinMode(LIGHT_1, OUTPUT);
   digitalWrite(LIGHT_1, LOW);
   pinMode(LIGHT_2, OUTPUT);
   digitalWrite(LIGHT_2, LOW);   
   pinMode(LIGHT_3, OUTPUT);
   digitalWrite(LIGHT_3, LOW);   
   delay(500);


   
   //setup and wifi connection
   wifiSetup();
 
   
   
   
   fauxmo.createServer(true); 
   fauxmo.setPort(80); 
   fauxmo.enable(true);

   fauxmo.addDevice("Green Light");
   fauxmo.addDevice("Red Light");
   fauxmo.addDevice("Orange Light");
   fauxmo.onSetState(callback); 
}

void loop() 
{
  fauxmo.handle();
}

/* ---------------------------------------------------------------------------
 Device Callback
 ----------------------------------------------------------------------------*/
void callback(unsigned char device_id, const char * device_name, bool state, unsigned char value) 
{

  Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);       
  
  //Switching action on detection of device name
  
  if ( (strcmp(device_name, "Red Light") == 0) ) 
  {
    if (state) 
    {
      digitalWrite(LIGHT_1, HIGH);
    } 
    else 
    {
      digitalWrite(LIGHT_1, LOW);
    }
  }

  if ( (strcmp(device_name, "Orange Light") == 0) ) 
  {
    if (state) 
    {
      digitalWrite(LIGHT_2, HIGH);
    } 
    else 
    {
      digitalWrite(LIGHT_2, LOW);
    }
  }

  if ( (strcmp(device_name, "Green Light") == 0) ) 
  {
    if (state) 
    {
      digitalWrite(LIGHT_3, HIGH);
    } 
    else 
    {
      digitalWrite(LIGHT_3, LOW);
    }
  }

}
    
/* -----------------------------------------------------------------------------
 Wifi Setup
 -----------------------------------------------------------------------------*/
void wifiSetup() 
{
   // Set WIFI module to STA mode
   WiFi.mode(WIFI_STA);

   // Connect
   Serial.println ();
   Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
   Serial.println();
   WiFi.begin(WIFI_SSID, WIFI_PASS);

   // Wait
   while (WiFi.status() != WL_CONNECTED) 
   {
      Serial.print(".");
      delay(100);
   }
   Serial.print(" ==> CONNECTED!" );
   Serial.println();

   // Connected!
   Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
   Serial.println();
}
