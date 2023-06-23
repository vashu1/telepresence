// board - NodeMCU 1.0    serial 115200
// note to myself - use left USB port, reconnect usb on error
// https://github.com/esp8266/Arduino/issues/1330
// the issue is mentioned about support for I2C slave mode in esp8266.

// pinouts https://esp8266-shop.com/esp8266-guide/esp8266-nodemcu-pinout/  https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/

// AWS IoT - based on https://how2electronics.com/connecting-esp8266-to-amazon-aws-iot-core-using-mqtt/

// another IoT example https://medium.com/accenture-the-dock/esp8266-aws-iot-core-guide-c640f2622a51
// checkout esphome.io

// Load Wi-Fi library
#include <ESP8266WiFi.h>

#include <WiFiClientSecure.h>
// NodeMCU 1.0
#include <PubSubClient.h>  // check out https://github.com/dersimn/ArduinoPubSubClientTools
#include <ArduinoJson.h>  //https://arduinojson.org/
#include <time.h>
#include "secrets.h"
#include "common.h"

#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>

#include <AccelStepper.h>
//TODO AccelStepper::HALF4WIRE
#define MotorInterfaceType 4
#define MOTOR_PIN_1 14
#define MOTOR_PIN_2 12
#define MOTOR_PIN_3 13
#define MOTOR_PIN_4 15
AccelStepper phoneStepper(MotorInterfaceType, MOTOR_PIN_1, MOTOR_PIN_3, MOTOR_PIN_2, MOTOR_PIN_4);

#define OTA_HOSTNAME "telerobot"
//in secrets #define OTA_PASSWORD "" // uncomment    TODO add empty str check

unsigned long lastCmdTime = 0;

/*#include <AccelStepper.h>
const int stepsPerRevolution = 2038;
// D5-D8
#define IN1 14
#define IN2 12
#define IN3 13
#define IN4 15
AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);
*/
WiFiClientSecure net;

BearSSL::X509List cert(cacert);
BearSSL::X509List client_crt(client_cert);
BearSSL::PrivateKey key(privkey);

PubSubClient client(net);

time_t now;
time_t nowish = 1510592825;

void NTPConnect(void) {
    //Serial.print("Setting time using SNTP");
    configTime(TIME_ZONE * 3600, 0 * 3600, NTP_SERVERS);
    now = time(nullptr);
    while (now < nowish) {
        delay(500);
        //Serial.print(".");
        now = time(nullptr);
    }
    //Serial.println("done!");
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    //Serial.print("Current time: ");
    //Serial.print(asctime(&timeinfo));
}


int getCmdCode(const char* cmd) {
  if (strcmp(CMD_STR_MOVE,   cmd) == 0) return CMD_INT_MOVE;
  if (strcmp(CMD_STR_CAMERA, cmd) == 0) return CMD_INT_CAMERA;
  if (strcmp(CMD_STR_PING,   cmd) == 0) return CMD_INT_PING;
  return -1;
}


void iotMessageReceived(char *topic, byte *payload, unsigned int length) {
    //Serial.println(topic);
    //Serial.println((char*)payload);
    
    DynamicJsonDocument doc(JSON_MAX_SIZE);
    deserializeJson(doc, payload);

    const char* cmd = doc["cmd"];
    int cmdCode = getCmdCode(cmd);
    // {"cmd":"camera", "speed":200,"steps":2000,"acceleration":100}
    if (cmdCode == CMD_INT_PING) {
      client.publish(AWS_IOT_PUBLISH_TOPIC, "pong");
      return;
    };
    if (cmdCode == CMD_INT_CAMERA) {
      phoneStepper.setAcceleration(doc.containsKey("acceleration") ? doc["acceleration"] : 100);
      phoneStepper.setMaxSpeed(doc.containsKey("speed") ? doc["speed"] : 100);
      phoneStepper.setSpeed(doc.containsKey("speed") ? doc["speed"] : 100);
      phoneStepper.moveTo(phoneStepper.currentPosition() + (int)doc["steps"]);
      return;
    }
    // {"cmd": "move", "direction": "f", "levels": [10,20]}
    Serial.write((uint8_t)SERIAL_PACKET_START);
    String direction = doc["direction"];
    Serial.write(direction.charAt(0));
    JsonArray cmds = doc["levels"].as<JsonArray>();
    Serial.write((uint8_t)cmds.size());
    uint8_t sum = 0;
    for (int i=0 ; i < cmds.size() ; i++) {
      uint8_t level = cmds[i];
      sum = sum ^ level;
      Serial.write(level);
    }
    Serial.write(sum);
    
    lastCmdTime = millis();
}


void connectAWS() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  //Serial.println(String("Attempting to connect to SSID: ") + String(WIFI_SSID));

  while (WiFi.status() != WL_CONNECTED) {
      //Serial.print(".");
      delay(1000);
  }

  NTPConnect();

  net.setTrustAnchors(&cert);
  net.setClientRSACert(&client_crt, &key);

  client.setServer(MQTT_HOST, 8883);
  client.setCallback(iotMessageReceived);

  //Serial.println("Connecting to AWS IOT");

  while (!client.connect(THINGNAME)) {
      //Serial.print(".");
      delay(1000);
  }

  if (!client.connected()) {
      //Serial.println("AWS IoT Timeout!");
      return;
  }
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  //Serial.println("AWS IoT Connected!");
  client.publish(AWS_IOT_PUBLISH_TOPIC, "\"esp8266_connected\"");
}


void setup() {
    Serial.begin(SERIAL_SPEED);
  
  //TODO undo comment password
    ArduinoOTA.setHostname(OTA_HOSTNAME);
    //ArduinoOTA.setPassword((const char *)OTA_PASSWORD);
    ArduinoOTA.begin();
  
    connectAWS();

    //pinMode(14, INPUT); // D5
}

unsigned long lastPingTime = 0;
void loop() {
  /*
  client.publish(AWS_IOT_PUBLISH_TOPIC, (digitalRead(14) == LOW) ? "\"LOW\"" : "\"HIGH\"");
  delay(1500);
  return;*/
    if (!phoneStepper.run()) phoneStepper.disableOutputs();  // digitalWrite(15, LOW);

   /* if (lastPingTime == 0 || (millis() - lastPingTime >= 10000)) {
          lastPingTime = millis();
          client.publish(AWS_IOT_PUBLISH_TOPIC, "\"10sec\"");
    };*/
    
    if (millis() - lastCmdTime > EMERGENCY_STOP_TIMEOUT_MS) {
        ArduinoOTA.handle();
    };
    //if (!stepper.isRunning()) stepper.disableOutputs();
    //stepper.run();
    //if (stepper.isRunning()) return;
    
  
    if (Serial.available() > 0) { // reading data from Hercules
   /*   char a[100];
      for (int i=0;i<100;i++) a[i] = 0;
      for (int i=0;i<99;i++){
        int c = Serial.read();
        if (c==-1) break;
        a[i] = c;
      }
      Serial.println(a);
      client.publish(AWS_IOT_PUBLISH_TOPIC, a);*/

      DynamicJsonDocument doc(JSON_MAX_SIZE);
      JsonArray array = doc.to<JsonArray>();
      while(Serial.available())
        array.add((uint8_t)Serial.read());
      char s[JSON_MAX_SIZE];
      serializeJson(doc, s);
      client.publish(AWS_IOT_ENCODER_TOPIC, s);
      
    };

    //now = time(nullptr);
  
    if (!client.connected()) {
        //Serial.println(client.state());
        delay(3000);
        connectAWS();
        client.publish(AWS_IOT_PUBLISH_TOPIC, "loop_reconnect");
    } else {
        client.loop();
    }
}
