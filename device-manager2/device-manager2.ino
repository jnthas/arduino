#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DeviceManager.h>
//#include <Morse.h>

#ifndef STASSID
#define STASSID "2G_NETVIRTUA_102"
#define STAPSK  "3313906800"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

Device devices[1] = {
  { .name = "Fan", .port = D1, .state = false }
};



void setup() {
  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  DeviceManager manager(&server, devices);

  manager.initialize();


  server.begin();


  manager.showAllDevices2();
  
  Serial.println("HTTP server started");
  MDNS.addService("http", "tcp", 80); 


}

void loop() {
  server.handleClient();

}
