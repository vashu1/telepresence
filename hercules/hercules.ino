// set board Duemilanove   proc  ATMega32       serial 9600

#include <Hercules.h>
#include <Wire.h>
#include "common.h"
#include <SoftwareSerial.h>

// pins must support change interrupts
#define RXPIN 2
#define TXPIN 3
SoftwareSerial espSerial(RXPIN, TXPIN);  // no neet to set pinMode - https://forum.arduino.cc/t/softwareserial-need-to-set-pinmode/307190/4

unsigned long lastCmdTime = 0;

void setup() {
    Serial.begin(9600);
    espSerial.begin(SERIAL_SPEED);
    MOTOR.begin();
    // ?? analogReadResolution(12);
}

const int LEN = 8;
int8_t cmd[LEN];
int cmdIndx = 0;

/*
// get data from Serial buffer
while (Serial.available() && cmdIndx < LEN)
    cmd[cmdIndx++] = Serial.read();

// align with cmd packet start marker
int movIndx = 0;
while (movIndx < cmdIndx && cmd[movIndx] != SERIAL_PACKET_START)
    movIndx++;

if (movIndx > 0) {
    cmdIndx -= movIndx;
    memmove(cmd, cmd + movIndx, cmdIndx);
};

if (cmdIndx >= 3) {
    lastCmdTime = millis();
    int8_t leftSpeed  = cmd[1];
    int8_t rightSpeed = cmd[2];
    
};
 */
void loop() {
    if (millis() - lastCmdTime > EMERGENCY_STOP_TIMEOUT_MS) {
        MOTOR.setStop1();
        MOTOR.setStop2();
    };
    
    if (espSerial.available() < 3) return;  // serial receive buffer (which holds 64 bytes)
    
    while (espSerial.read() != SERIAL_PACKET_START);
    if (espSerial.available() < 2) {
      espSerial.println("partial");
      espSerial.println(espSerial.available());
      delay(200);
      return;
    }
    int8_t leftSpeed  = espSerial.read();
    int8_t rightSpeed = espSerial.read();

    lastCmdTime = millis();
    espSerial.println(leftSpeed);  //TODO del
    espSerial.println(rightSpeed);

    if (leftSpeed == 0 && rightSpeed == 0) {
        MOTOR.setStop1();
        MOTOR.setStop2();
    } else {
        MOTOR.setSpeedDir1(abs(leftSpeed),  leftSpeed  > 0 ? DIRF : DIRR);
        MOTOR.setSpeedDir2(abs(rightSpeed), rightSpeed > 0 ? DIRF : DIRR);
    };

    // return sensor data
    uint16_t v = analogRead(VOLTMETER_PIN);
    uint16_t a = analogRead(AMMEMETER_PIN);
    espSerial.write((uint8_t)SERIAL_PACKET_START);
    espSerial.write((const uint8_t *)&v, 2);
    espSerial.write((const uint8_t *)&a, 2);
}
