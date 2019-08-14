#include <FS.h>                   //this needs to be first, or it all crashes and burns...

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<String.h>
#include "FastLED.h"
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager 

#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

#define RGBPIN            14              // 14
#define NUMPIXELS            24
#define G13       2               //4
#define S1      4   //5
CRGB leds[NUMPIXELS];

/* Create an instance of PubSubClient client */
WiFiClient espClient;
PubSubClient client(espClient);

/* Device name*/
const char* deviceName = "ESP02";

/* MQTT Broker configs*/
char mqtt_server[40];
char mqtt_port[8] = "PORT";

const char* mqttUser = "MQTT_USER";
const char* mqttPassword = "MQTT_PASSWORD";

/* Topics names on MQTT Broker */
const char* topic1 = "test";
const char* topic2 = "names";
const char* topic3 = "time";

/* Message, color, name and time variables*/
int ttimeout;
char timeout[4];
char delay2[4];
char nome[8];
char cor[12];
char msg[32];
char msg2[27];

bool shouldSaveConfig = false;

/* Callback notifying us of the need to save config */
void saveConfigCallback () {
  //Serial.println("Should save config");
  shouldSaveConfig = true;
}

// The extra parameters to be configured (can be either global or just in the setup)
// After connecting, parameter.getValue() will get you the configured value
// id/name placeholder/prompt default length
WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 6);

/*WiFiManager
Local intialization. Once its business is done, there is no need to keep it around*/
WiFiManager wifiManager;

 /* Reset save configurations button */
void resetESP(){
  if (digitalRead(G13) == LOW) {
    wifiManager.resetSettings();
    Serial.println("resetou");
    ESP.reset();
  }
}

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<NEOPIXEL, RGBPIN>(leds, NUMPIXELS);
  FastLED.show();
  pinMode(G13, INPUT);
  
  if (SPIFFS.begin()) {
    //Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        //Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          strcpy(mqtt_server, json["mqtt_server"]);
          strcpy(mqtt_port, json["mqtt_port"]);

        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read
  
  /* Wifi Manager Settings */
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  
  wifiManager.addParameter(&custom_mqtt_server);
  
  wifiManager.addParameter(&custom_mqtt_port);
  
  wifiManager.setMinimumSignalQuality(40);
  
  //wifiManager.setTimeout(240);
  
  /* Attempt to connect to "Fitbit" Network, change the name of you network here */
  if (!wifiManager.autoConnect("FitBit")) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    analogWriteFreq(48000);
    delay(5000);
  }

  /* Save network and broker settings */
  if (shouldSaveConfig) {
    Serial.println("saving config");
    strcpy(mqtt_server, custom_mqtt_server.getValue());
    strcpy(mqtt_port, custom_mqtt_port.getValue());
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["mqtt_server"] = mqtt_server;
    json["mqtt_port"] = mqtt_port;

    File configFile = SPIFFS.open("/config.json", "w");

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
  }

  /*Print MQTT server and port */
  Serial.println(mqtt_server);
  Serial.println(mqtt_port);
  
  /* Configure the MQTT server with IPaddress and port */
  client.setServer(mqtt_server, atoi(mqtt_port));
  client.setCallback(callback);
  delay(1000);

  resetESP();

 /* Connect to MQTT Broker */
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {

      Serial.println("Connected");
      pisca(4000);
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }

  /* Subscribe to topics and publish your device name */
  client.subscribe(topic1);
  client.subscribe(topic2);
  client.publish(topic2, deviceName);
}

void loop() {
  /* Try to reconnect again */
  if (!client.connected()) {
    mqttconnect();
  }
  // Function to listen for incomming
  client.loop();
  resetESP();
  long lastMsg;
  long now = millis();
  if (now - lastMsg > 8000) {
   lastMsg = now;
   client.publish(topic1, "Azul");
   Serial.println("Keep Alive-");
  }
  
}
