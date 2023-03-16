// set board Duemilanove   proc  ATMega32       serial 9600

#include <Hercules.h>
#include <Wire.h>
#include <ArduinoJson.h> // https://arduinojson.org/
#include "common.h"

unsigned long lastCmdTime;
int leftSpeed, rightSpeed;

int getCmdCode(const char* cmd) {
  if (strcmp(CMD_STR_MOVE,   cmd) == 0) return CMD_INT_MOVE;
  if (strcmp(CMD_STR_CAMERA, cmd) == 0) return CMD_INT_CAMERA;
  return -1;
}

/*
void sendToI2C(int ilen, unsigned char *idata) {
    //TODO assert(ilen < JSON_MAX_SIZE);
    Wire.beginTransmission(I2C_ESP8266_ADDRESS);
    for(int i = 0; i<ilen; i++) {Wire.write(idata[i]);}  // sends one byte
    Wire.endTransmission();                              // stop transmitting
    
}
*/

void onRecieveI2C(int len) {
    lastCmdTime = millis();
    /*while(Wire.available()) {
        char c = Wire.read();    // Receive a byte as character
        Serial.print(c);         // Print the character
    }
    char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";*/
    //DynamicJsonDocument doc(JSON_MAX_SIZE);
    //deserializeJson(doc, json);
    //Serial.println(json);
    
    DynamicJsonDocument doc(JSON_MAX_SIZE);
    deserializeJson(doc, Wire);
    const char* cmd = doc["cmd"];
    Serial.println(cmd);
    int cmdCode = getCmdCode(cmd);
    switch (cmdCode) {
        case CMD_INT_MOVE:
          int x = doc["x"];
          int y = doc["y"];
          if (y < 0 && y < -MAX_REVERSE_SPEED) y =-MAX_REVERSE_SPEED;
          Serial.println(y); //TODO del

          int l, r;
          l = r = y;
          l += x * TURN_COEFF;
          r -= x * TURN_COEFF;

          if (l > MAX_MOTOR_SPEED) l = MAX_MOTOR_SPEED;
          if (r > MAX_MOTOR_SPEED) r = MAX_MOTOR_SPEED;
          if (l < -MAX_MOTOR_SPEED) l = -MAX_MOTOR_SPEED;
          if (r < -MAX_MOTOR_SPEED) r = -MAX_MOTOR_SPEED;

          Serial.println("sub");
          Serial.println(l);
          leftSpeed =  l;
          rightSpeed = r;
          break;
    
        case CMD_INT_CAMERA:
          break;

        default:
          Serial.println("\nUnknown cmd!");
    }
    //TODO transmit state
}

void setup() {
    leftSpeed = rightSpeed = 0;
    Serial.begin(9600);
    MOTOR.begin();
    Wire.begin(I2C_HERCULES_ADDRESS);
    Wire.onReceive(onRecieveI2C);
}

void loop() {
    Serial.println("loop");
    Serial.println(leftSpeed);
    // emergency stop if connection lost
    if ((leftSpeed == 0 && rightSpeed == 0) || (millis() - lastCmdTime > EMERGENCY_STOP_TIMEOUT_MS)) {
        MOTOR.setStop1();
        MOTOR.setStop2();
    } else {
        MOTOR.setSpeedDir1(abs(leftSpeed),  leftSpeed  > 0 ? DIRF : DIRR);
        MOTOR.setSpeedDir2(abs(rightSpeed), rightSpeed > 0 ? DIRF : DIRR);
    };
}
