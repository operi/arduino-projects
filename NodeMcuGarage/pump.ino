bool getPumpState() {
  return digitalRead(pinPump);
}

void handlePump() {
  digitalWrite(pinPump, !getPumpState());
}
