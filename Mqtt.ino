
void mqttconnect() {
  /* Loop until reconnected */
  while (!client.connected()) {
    /* client ID */
    String clientId = "ESP8266Client";
    /* connect now */
    if (client.connect(clientId.c_str(), mqttUser, mqttPassword)) {
      /* subscribe topic with default QoS 0*/
      client.subscribe(topic1);
      client.subscribe(topic2);
      pisca(4000);
    } else {
      /* Wait 5 seconds before retrying */
      delay(5000);
    }
  }
}

void clearMessage() {
  for (uint8_t i = 0; i < 32; i++) {
    msg[i] = 0;
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  //Serial.print("Message arrived [");-
  if (topic == topic3){
    for (int i = 0; i < length; i++) {
      msg[i] = (char)payload[i];
    }
    if (strncmp("chrono", msg, 6) == 0) {
        acende(0x00, 0x00, 0xFF);
        client.publish(topic1, "start");
        lerSensor2(); 
    }
  clearMessage();
  }
  if (topic == topic2) {
    Serial.print(topic);
    for (int i = 0; i < length; i++) {
      msg[i] = (char)payload[i];
    }
    if (strncmp(deviceName, msg, 3) != 0) {
    client.publish(topic2, deviceName);
    }
    clearMessage();
  } else {
    for (int i = 0; i < length; i++) {
      msg[i] = (char)payload[i];
      if( i>=6 && i<8){
         timeout[i-6] = (char)payload[i];
      }
      if (i>8){
         cor[i-9] = (char)payload[i];
      }
    }
    Serial.println();
    Serial.println(msg);
    ttimeout = 1000*atoi(timeout);
    if (strncmp(deviceName, msg, 5) == 0) {
      for (int i = 0; i < 5; i++) {
        nome[i] = msg[i];
      }
      Serial.print("Meu nome é ");
      Serial.print(nome);
      Serial.println();
      Serial.println(cor);
      if (strncmp("Azul", cor, 4) == 0) {
        acende(0x00, 0x00, 0xFF);
        client.publish(topic1, "start");
        lerSensor1(ttimeout);
      } else {
        if (strncmp("Vermelho", cor, 8) == 0) {
          acende(0xFF, 0x00, 0x00);
          client.publish(topic1, "start");
          lerSensor2();
        } else {
          if (strncmp("Branca", cor, 6) == 0) {
            acende(0xFF, 0xFF, 0xFF);
            client.publish(topic1, "start");
            lerSensor2();
          } else {
            if (strncmp("Verde", cor, 5) == 0) {
              acende(0x00, 0xFF, 0x00);
              client.publish(topic1, "start");
              lerSensor2();

            } else {
              if (strncmp("Amarelo", cor, 7) == 0) {
                acende(0xFF, 0xFF, 0x00);
                client.publish(topic1, "start");
                lerSensor2();
              } else {
                if (strncmp("Roxo", cor, 4) == 0) {
                  acende(0x55, 0x00, 0xAB);
                  client.publish(topic1, "start");
                  lerSensor2();
                } else {
                  if (strncmp("Rosa", cor, 4) == 0) {
                    acende(0x99, 0x00, 0x99);
                    client.publish(topic1, "start");
                    lerSensor2();
                  } else {
                    if (strncmp("Laranja", cor, 7) == 0) {
                      acende(0x8e, 0x46, 0x00);
                      client.publish(topic1, "start");
                      lerSensor2();
                    } else {
                      acende(0x00, 0x00, 0x00);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    clearMessage();
  }
}
