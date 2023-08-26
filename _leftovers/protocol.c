
#define PACKET_MAX_LENGTH 128

struct Packet {
  uint8_t startMarker;
  uint8_t dataLen;
  uint8_t dataChksum;
  uint8_t data[];
};

bool serialAvailable(SoftwareSerial *serial) {
  return serial ? serial->available() : Serial.available();
}

uint8_t serialRead(SoftwareSerial *serial) {
  return serial ? serial->read() : Serial.read();
}

int8_t dataChecksum(int8_t *s, int len) {
  int8_t c = 0;
  for (int i=0; i < len ; i++)
    c ^= s[i]; 
  return c;
}

bool readSerial2(SoftwareSerial *serial, uint8_t *buffer, int *bufferIndex, Packet *packetResult) {
  if (!serialAvailable(serial) && *bufferIndex == 0) return;

  // get data from Serial buffer
  while (serialAvailable(serial) && *bufferIndex < PACKET_MAX_LENGTH)
      buffer[(*bufferIndex)++] = serialRead(serial);

  // align with cmd packet start marker
  int movIndx = 0;
  while (movIndx < *bufferIndex && buffer[movIndx] != SERIAL_PACKET_START)
      movIndx++;

  if (movIndx > 0) {
    *bufferIndex -= movIndx;
    memmove(serial, serial + movIndx, *bufferIndex);
  };
 
  // packet_start direction len ... sum
  if (*bufferIndex < sizeof(Packet)) return false; // no min size yet
  Packet *packet = (void*)buffer;
  int packetLen = packet->dataLen + sizeof(Packet);
  if (bufferIndex < packetLen) return false; // not all data yet
  if (packet->dataChksum != dataChecksum(packet->data, packet->dataLen)) { // bad checksum
    bufferIndex = 0;
    return false;
  }
  // copy to cmd's
  memcpy(packetResult, buffer, packetLen);
  *bufferIndex -= packetLen;
  memmove(buffer, buffer + packetLen, *bufferIndex);
  return true;
}



      if (array.size() == 3 && array[0] == SERIAL_PACKET_START && array[1] == 'v') {
        char s[JSON_MAX_SIZE];
        itoa(array[2], s, 10);
        client.publish(AWS_IOT_VOLTAGE_TOPIC, s);
      } else {
        char s[JSON_MAX_SIZE];
        //memset(s, 0, sizeof(s));
        serializeJson(doc, s);
        client.publish(AWS_IOT_ENCODER_TOPIC, s);
      };