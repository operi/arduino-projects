#include <ESP8266WiFi.h>
#include "WifiKeys.h"

int relayMotor = 5; // D1
int relayLight = 4; // D2
WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  delay(10);

  pinMode(relayMotor, OUTPUT);
  digitalWrite(relayMotor, LOW);
  pinMode(relayLight, OUTPUT);
  digitalWrite(relayLight, LOW);

  setupWifi();
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return ;
  }

  // Wait until the client sends some data
  while (!client.available()) {
    delay(1);
  }

  String request = readRequest(client);
  actOnRequest(request);
  printHTML(client);
  
  delay(1);
}

void actOnRequest(String request){
  if (request.indexOf("flip-motor") != -1)  {
    int current = digitalRead(relayMotor);
    digitalWrite(relayMotor, !current);
  }
  if (request.indexOf("flip-light") != -1) {
    int current = digitalRead(relayLight);
    digitalWrite(relayLight, !current);
  }
}

void printHTML(WiFiClient client) {
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.println("<br><br>");
  client.print("<a href=\"/flip-motor\"\"><button>Flip MOTOR </button></a> <span>Current status: ");
  client.print(!digitalRead(relayMotor));
  client.println("</span>");
  client.println("<br><br>");
  client.println("<a href=\"/flip-light\"\"><button>Flip LIGHT </button></a> <span>Current status: ");
  client.print(!digitalRead(relayLight));
  client.println("</span>");
  client.println("</html>");

}

void setupWifi(){
  IPAddress ip(192, 168, 0, 157);
  IPAddress gateway(192, 168, 0, 1);
  IPAddress subnet(255, 255, 255, 0);
  // if need internet access
  //IPAddress dns(192, 168, 1, 249);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);


  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Start the server
  server.begin();

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

String readRequest(WiFiClient client){
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  client.flush();
  return request;  
}
