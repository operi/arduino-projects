void setupWS() {
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("WebSocket server started.");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        sendStatus(num);
      }
      break;
    case WStype_TEXT:
    String payload_str = String((char*) payload);
      if (payload_str == "pump") {
        handlePump();
      } else if (payload_str == "light") {
        handleLight();
      }
      sendStatus(num);
      break;
  }
}

void sendStatus(uint8_t num) {
  DynamicJsonDocument doc(capacity);
  // See https://github.com/nodemcu/nodemcu-devkit-v1.0/issues/16#issuecomment-244625860
  JsonObject light = doc.createNestedObject("light");
  JsonObject pump = doc.createNestedObject("pump");
  light["state"] = !getLightState();
  pump["state"] = !getPumpState();
  pump["stopPumpAt"] = stopPumpAt;
  char output[128];
  int size = serializeJson(doc, output);
  webSocket.sendTXT(num, output, size);
}
