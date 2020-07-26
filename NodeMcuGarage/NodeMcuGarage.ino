#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include "WifiKeys.h"

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

const int pinPump = 4;
const int pinLight = 5;

void handleRoot();
void handlePump();
void handleLight();
void handleNotFound();

String styles = "<style>form{text-align:center;height:200px;width:100%} input{height:50%;width:50%;font-size:50px}</style>";
String lineBreak = "<br><br>";
String formPump = "<form action=\"/pump\" method=\"POST\"><input type=\"submit\" value=\"Toggle Pump\"></form>";
String formLight = "<form action=\"/light\" method=\"POST\"><input type=\"submit\" value=\"Toggle Light\"></form>";

void setup(void) {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  pinMode(pinPump, OUTPUT);
  pinMode(pinLight, OUTPUT);

  wifiMulti.addAP(ssid, password);

  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer

  if (MDNS.begin("esp8266")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  server.on("/", HTTP_GET, handleRoot);     // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/pump", HTTP_POST, handlePump);  // Call the 'handleLED' function when a POST request is made to URI "/LED"
  server.on("/light", HTTP_POST, handleLight);
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();                    // Listen for HTTP requests from clients
}

void handleRoot() {                         // When URI / is requested, send a web page with a button to toggle the LED
  server.send(200, "text/html", getHTML());
}

String getHTML() {
  return styles + lineBreak + formPump + formLight;
}

void handlePump() {                          // If a POST request is made to URI /LED
  digitalWrite(pinPump, !digitalRead(pinPump));     // Change the state of the LED
  server.sendHeader("Location", "/");       // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}

void handleLight() {
  digitalWrite(pinLight, !digitalRead(pinLight));
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
