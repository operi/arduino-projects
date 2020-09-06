void setupServer() {
  server.on("/pump", HTTP_POST, handlePump);
  server.on("/light", HTTP_POST, handleLight);
  server.on("/upload", HTTP_GET, []() {
    if (!handleFileRead("/upload.html"))
      server.send(404, "text/plain", "404: Not Found");
  });

  server.on("/upload", HTTP_POST,
  []() {
    server.send(200);
  },
  handleFileUpload);
  
  server.onNotFound([]() {
    if (!handleFileRead(server.uri()))
      server.send(404, "text/plain", "404: Not Found");
  });

  server.begin();
  Serial.println("HTTP server started");
}
