long pumpRunTime = 30 * 60;

bool getPumpState() {
  return digitalRead(pinPump);
}

void handlePump() {
  digitalWrite(pinPump, !getPumpState());
  if (getPumpState() == ON) {
    stopPumpAt = timeClient.getEpochTime() + pumpRunTime;
  } else {
    stopPumpAt = 0;
  }
  sendStatus();
}

bool shouldTurnPumpOff() {
  return stopPumpAt > 0 and stopPumpAt < timeClient.getEpochTime();
}
