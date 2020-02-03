#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Morse.h>

#ifndef STASSID
#define STASSID "2G_NETVIRTUA_102"
#define STAPSK  "3313906800"
#endif


ESP8266WebServer server(80);

Device devices[] = {
  { .name = "Fan", .port = 2, .state = false }
};



void setup()
{
  
  Morse morse(2, &server, devices);
  
  Serial.begin(9600);
  //morse.addServer();

  morse.listDevices();

  //server.begin();

  
//  Serial.println("Size");
//  Serial.println(sizeof(devices));
}

void loop()
{
//  //morse.hello("jonathas");
//  //morse.listDevices(devices);
//
//  morse.dot(); morse.dot(); morse.dot();
//  morse.dash(); morse.dash(); morse.dash();
//  morse.dot(); morse.dot(); morse.dot();
//  delay(3000);
}
