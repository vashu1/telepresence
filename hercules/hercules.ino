// TODO set board Duemilanove

#include <Hercules.h>
#include <Wire.h>
#include <ArduinoJson.h>

// 0 - 100
#define MAX_MOTOR_SPEED 100
#define TURN_COEFF 0.5

// if motors are running and no cmd recieved for EMERGENCY_STOP_TIMEOUT_MS then stop
#define EMERGENCY_STOP_TIMEOUT_MS 333

#define I2C_ESP8266_ADDRESS  1
#define I2C_HERCULES_ADDRESS 2

#define CMD_MOVE   1
#define CMD_CAMERA 2

unsigned long lastCmdTime;
bool moving = false;

void sendToI2C(unsigned char ilen, unsigned char *idata) {
    Wire.beginTransmission(I2C_ESP8266_ADDRESS);
    for(int i = 0; i<ilen; i++) {Wire.write(idata[i]);}  // sends one byte
    Wire.endTransmission();                              // stop transmitting
    
}

void onRecieveI2C(int len) {
    lastCmdTime = millis();
    /*while(Wire.available()) {
        char c = Wire.read();    // Receive a byte as character
        Serial.print(c);         // Print the character
    }
    char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
    ...
    deserializeJson(doc, json);
    */
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, Wire);
    int cmd = doc["cmd"];
    switch (cmd) {
        case CMD_MOVE:
          int x = doc["x"];
          int y = doc["y"];
          moving = x != 0 || y != 0;
          if (!moving) {
            MOTOR.setStop1();
            MOTOR.setStop2();
          } else { // MAX_MOTOR_SPEED
            int l, r;
            l = r = y;
            l += x * TURN_COEFF;
            r -= x * TURN_COEFF;
            
            unsigned char dLeft, dRight;  // DIRF or DIRR
            dLeft = dRight = DIRF;
            if (l < 0) {
              l *= -1;
              dLeft = DIRR;
            }
            if (r < 0) {
              r *= -1;
              dRight = DIRR;
            }

            if (l > MAX_MOTOR_SPEED) l = MAX_MOTOR_SPEED;
            if (r > MAX_MOTOR_SPEED) r = MAX_MOTOR_SPEED;
            
            MOTOR.setSpeedDir1(l, dLeft);
            MOTOR.setSpeedDir2(r, dRight);
          }
          break;
    
        case CMD_CAMERA:
          break;

        default:
          Serial.println("\nUnknown cmd!");
    }
    //TODO transmit state
}

void setup() {
    MOTOR.begin();
    Wire.begin(I2C_HERCULES_ADDRESS);
    Wire.onReceive(onRecieveI2C);
}

void loop() {
    // emergency stop if connection lost
    if (moving && (lastCmdTime + EMERGENCY_STOP_TIMEOUT_MS < millis())) {
        MOTOR.setStop1();
        MOTOR.setStop2();
        moving = false;
    }
}
