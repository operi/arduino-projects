#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include "WifiKeys.h"

ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80);

const int pinPump = 5;
const int pinLight = 4;

String getContentType(String filename);
bool handleFileRead(String path);

void setup(void) {
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');

  pinMode(pinPump, OUTPUT);
  pinMode(pinLight, OUTPUT);

  wifiMulti.addAP(ssid, password);

  Serial.println("Connecting ...");
  // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  // Start the mDNS responder
  if (MDNS.begin("arduino")) {
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }
  
  SPIFFS.begin();

  server.on("/pump", HTTP_POST, handlePump);
  server.on("/light", HTTP_POST, handleLight);
  server.onNotFound([]() {
    if (!handleFileRead(server.uri()))
      server.send(404, "text/plain", "404: Not Found");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}

String getContentType(String filename){
  if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path){
  Serial.println("handleFileRead: " + path);
  if(path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){
    if(SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    Serial.println(String("\tSent file: ") + path);
    return true;
  }
  Serial.println(String("\tFile Not Found: ") + path);
  return false;
}

void handlePump() {
  digitalWrite(pinPump, !digitalRead(pinPump));
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleLight() {
  digitalWrite(pinLight, !digitalRead(pinLight));
  server.sendHeader("Location", "/");
  server.send(303);
}
