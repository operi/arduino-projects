#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include "WifiKeys.h"

ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80);

const int pinPump = 5;
const int pinLight = 4;

void handleRoot();
void handlePump();
void handleLight();
void handleNotFound();

String styles = "<style>body{padding-top:3rem;display:grid;place-items:center;grid-gap:3rem;}form{height:8rem;width:50%;}button{width:100%;height:100%;font-size:3rem;border-radius:12px;}</style>";
String formPump = "<form action=\"/pump\"><button type=\"submit\" formmethod=\"post\">Toggle Pump</button></form>";
String formLight = "<form action=\"/light\"><button type=\"submit\" formmethod=\"post\">Toggle Light</button></form>";

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

  server.on("/", HTTP_GET, handleRoot);
  server.on("/pump", HTTP_POST, handlePump);
  server.on("/light", HTTP_POST, handleLight);
  server.onNotFound([](){
    server.send(404, "text/plain", "404: Not found");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}

void handleRoot() {
  server.send(200, "text/html", getHTML());
}

String getHTML() {
  return styles + "<body>" + formPump + formLight + "</body>";
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
