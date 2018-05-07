/*
 Basic ESP8266 MQTT json example

 Demonstrates sending and receiving Protocol Buffers over MQTT to the ESP8266.
 In this case, we will use React.js to control the brightness of
 the ESP8266 onboard led.

*/

#include "config.h"

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "Message.h"

byte brightness = 0;

WiFiClient espClient;
PubSubClient client(espClient);

Brightness_t data = Potentiometer_Brightness_init_default;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);

  while (!Serial && millis() < 10000)
    ;
    
  analogWriteRange(255);

  setup_wifi();
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(callback);
}

void setup_wifi() {
  delay(10);
   
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf(" ESP8266 Chip id = %08X\n", ESP.getChipId());
}

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';
  Message::decode(&data, payload, length);

  //  Serial.println("------");
  //  Serial.println(Brightness.id);
  //  Serial.println(Brightness.level);
  //  Serial.println("------");
  
  // 1   -> set Brightness
  // 100 -> getState req
  // 101 -> getState ack
  
  switch(data.id) {
    case 100: // getState req
      if (!sendSettings()) {
        break;
      }
    case 1: // set Brightness
      if (data.level) {
          brightness = constrain(data.level, 0, 255);
          analogWrite(LED_PIN, (255 - brightness));
          Serial.print("Brightness: ");
          Serial.println(brightness);
      }
      break;
    default:
//      Serial.println(Brightness.level);
      break;
  }
}

bool sendSettings() {
  data.id = 101; // Code for getState ack
  data.level = brightness;
  
  uint8_t *buffer;
  size_t length;

  Message::encode(&data, &buffer, &length);
  
  if(client.publish(MQTT_TOPIC_OUT, buffer, length)) {
    return true;
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(MQTT_CLIENT_ID)) {
      Serial.println("connected");
      sendSettings();
      client.subscribe(MQTT_TOPIC_OUT);
      client.subscribe(MQTT_TOPIC_IN);
    } else {
      Serial.print("failed, rc=");
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();
}
