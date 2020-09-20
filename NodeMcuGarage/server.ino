void setupServer() {
  server.on("/pump", HTTP_POST, handlePump);
  server.on("/light", HTTP_POST, handleLight);
  
  server.onNotFound([]() {
    if (!handleFileRead(server.uri()))
      server.send(404, "text/plain", "404: Not Found");
  });

  server.begin();
  Serial.println("HTTP server started");
}
