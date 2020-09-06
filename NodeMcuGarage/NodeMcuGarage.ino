#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <FS.h>
#include "Keys.h"

File fsUploadFile;
ESP8266WebServer server(80);
WebSocketsServer webSocket(81);
IPAddress ip(192, 168, 0, 200);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
const size_t capacity = JSON_OBJECT_SIZE(2);
DynamicJsonDocument doc(capacity);
int stopPumpAt = 0;


// use custom values. See https://github.com/nodemcu/nodemcu-devkit-v1.0/issues/16#issuecomment-244625860
const int ON = !HIGH;
const int OFF = !LOW;

const int pinPump = 5;
const int pinLight = 4;

void setup(void) {
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');

  pinMode(pinPump, OUTPUT);
  pinMode(pinLight, OUTPUT);
  digitalWrite(pinPump, OFF);
  digitalWrite(pinLight, OFF);

  setupWIFI();

  setupMDNS();

  setupOTA();

  SPIFFS.begin();

  setupServer();

  setupWS();
}

void loop(void) {
  webSocket.loop();
  ArduinoOTA.handle();
  server.handleClient();
  MDNS.update();
  if (shouldTurnPumpOff()) {
    handlePump();
  }
}
