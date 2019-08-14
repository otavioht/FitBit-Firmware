
void lerSensor1(int timeout) {
  int Sensor1 = 0;
  long ltime = 0;
  long ftime = millis();
  while (ltime - ftime < timeout) {
    ltime = millis();
    Sensor1 = digitalRead(S1);
    if (Sensor1 == 1) {
      break;
    }
    delay(20);
  }
  acende(0x00, 0x00, 0x00);
  client.publish(topic1, "stop");
}

void lerSensor2() {
  int Sensor1 = 0;
  while (true) {
    Sensor1 = digitalRead(S1);
    if (Sensor1 == 1) {
      break;
    }
    delay(5);
  }
}

