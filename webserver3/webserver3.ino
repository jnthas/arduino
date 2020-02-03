#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "2G_NETVIRTUA_102"
#define STAPSK  "3313906800"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

const int led = 13;

struct Device
{
    String name;
    uint8_t port;
    bool state;
};

Device devices[] = {
  { .name = "Fan", .port = D1, .state = false },
  { .name = "Device 2", .port = D2, .state = false },
  { .name = "Device 3", .port = D3, .state = false },
  { .name = "Device 4", .port = D4, .state = false }
};


String MAIN_PAGE = "<!DOCTYPE html>"\
                  "<html>"\
                      "<head>"\
                          "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"\
                          "<link rel=\"icon\" href=\"data:,\">"\
                          "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}</style>"\
                      "</head>"\
                      "<body>"\
                          "<h1>Device Manager</h1>"\
                          "{DEVICE_ELEM}"\
                      "</body>"\
                  "</html>";

String DEVICE_ELEM = "<p>{DEVICE_NAME} - <a href=\"/turn/{DEV_STATE}?port={DEV_PORT}\">{DEV_STATE_LABEL}</a></p>";


void showAllDevices() {

  String message = "Devices:\n\n";
  for (uint8_t i = 0; i < sizeof(devices)/16; i++) {
    message += " " + devices[i].name + ": " + devices[i].state + "\n";
  }
  server.send(200, "text/plain", message);
  
}

void controlPort(int port, bool state) {
  digitalWrite(port, state);
}


void handleRoot() {
  String all_dev_elems = "";
  for (uint8_t i = 0; i < sizeof(devices)/16; i++) {
    String dev = DEVICE_ELEM;
    String state = getState(devices[i].state);
    String stateLabel = getState(!devices[i].state);
    dev.replace("{DEVICE_NAME}", devices[i].name);
    dev.replace("{DEV_STATE}", state);
    dev.replace("{DEV_PORT}", String(devices[i].port, DEC));
    stateLabel.toUpperCase();
    dev.replace("{DEV_STATE_LABEL}", stateLabel);
    
    all_dev_elems += dev;
  }
  String page = MAIN_PAGE;
  page.replace("{DEVICE_ELEM}", all_dev_elems);
  server.send(200, "text/html", page);
  
}

String getState(bool state) {
  if (state) {
    return String("on");
  }

  return String("off");
}


void handleTurnDevices(bool state) {
  
  if (server.hasArg("port")) {
    uint8_t index = findDeviceIndexByPort(server.arg("port").toInt());
    devices[index].state = !devices[index].state;
    controlPort(devices[index].port, devices[index].state);
    handleRoot();
  } else {
    server.send(200, "text/plain", "no port found");
  }
  
}


uint8_t findDeviceIndexByPort(int port) {
  for (uint8_t i = 0; i < sizeof(devices)/16; i++) {
    if (devices[i].port == port) {
      return i;
    }
  }
}


void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void initializePorts() {
  for (uint8_t i = 0; i < sizeof(devices)/16; i++) {
    pinMode(devices[i].port, OUTPUT);
    digitalWrite(devices[i].port, devices[i].state);
  }
}


void setup(void) {
  
  initializePorts();
  
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

  server.on("/", handleRoot);

  server.on("/turn/on", []() {
    handleTurnDevices(true);
  });

  server.on("/turn/off", []() {
    handleTurnDevices(false);
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  MDNS.addService("http", "tcp", 80); 
}

void loop(void) {
  server.handleClient();
 
}
