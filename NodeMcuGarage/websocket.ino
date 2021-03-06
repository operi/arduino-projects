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
      break;
  }
}

void sendStatus(uint8_t num) {
  const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3);
  DynamicJsonDocument doc(capacity);
  // See https://github.com/nodemcu/nodemcu-devkit-v1.0/issues/16#issuecomment-244625860
  JsonObject light = doc.createNestedObject("light");
  light["state"] = !getLightState();
  JsonObject pump = doc.createNestedObject("pump");
  pump["state"] = !getPumpState();
  pump["stopPumpAt"] = stopPumpAt;
  char output[128];
  int size = serializeJson(doc, output);
  webSocket.sendTXT(num, output, size);
}

void broadcastStatus() {
    const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3);
  DynamicJsonDocument doc(capacity);
  // See https://github.com/nodemcu/nodemcu-devkit-v1.0/issues/16#issuecomment-244625860
  JsonObject light = doc.createNestedObject("light");
  light["state"] = !getLightState();
  JsonObject pump = doc.createNestedObject("pump");
  pump["state"] = !getPumpState();
  pump["stopPumpAt"] = stopPumpAt;
  char output[128];
  int size = serializeJson(doc, output);
  webSocket.broadcastTXT(output, size);
}
