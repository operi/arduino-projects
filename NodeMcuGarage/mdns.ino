void setupMDNS() {
  if (MDNS.begin("garage")) {
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }
}
