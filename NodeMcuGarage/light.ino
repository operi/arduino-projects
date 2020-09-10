bool getLightState() {
  return digitalRead(pinLight);
}

void handleLight() {
  digitalWrite(pinLight, !getLightState());
  sendStatus();
}
