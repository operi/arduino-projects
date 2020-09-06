long halfHour = 1800000;

bool getPumpState() {
  return digitalRead(pinPump);
}

void handlePump() {
  digitalWrite(pinPump, !getPumpState());
  if (getPumpState() == ON) {
    stopPumpAt = millis() + halfHour;
  } else {
    stopPumpAt = 0;
  }
}

bool shouldTurnPumpOff() {
  return stopPumpAt > 0 and stopPumpAt < millis();
}
