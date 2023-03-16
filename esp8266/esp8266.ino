/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com
*********/
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
// NodeMCY 1.0
#include <PubSubClient.h>  // check out https://github.com/dersimn/ArduinoPubSubClientTools
#include <ArduinoJson.h>  //https://arduinojson.org/
#include <Wire.h>
#include <time.h>
#include "secrets.h"
#include "common.h"

WiFiClientSecure net;

BearSSL::X509List cert(cacert);
BearSSL::X509List client_crt(client_cert);
BearSSL::PrivateKey key(privkey);

PubSubClient client(net);

unsigned long lastMillis = 0;
unsigned long previousMillis = 0;
const long interval = 5000;

time_t now;
time_t nowish = 1510592825;


void sendToI2C(int ilen, unsigned char *idata) {
  assert(ilen < JSON_MAX_SIZE);
  Wire.beginTransmission(I2C_HERCULES_ADDRESS);
  for (int i = 0; i < ilen; i++) {
    Wire.write(idata[i]);
  }  // sends one byte
  Wire.endTransmission();                              // stop transmitting

}


void NTPConnect(void)
{
  Serial.print("Setting time using SNTP");
  configTime(TIME_ZONE * 3600, 0 * 3600, NTP_SERVERS);
  now = time(nullptr);
  while (now < nowish)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("done!");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}


void messageReceived(char *topic, byte *payload, unsigned int length)
{
  //Serial.println("his");
  Serial.print("Received [");
  Serial.print(topic);
  Serial.println("]: ");
  Serial.println((char*)payload);
  sendToI2C(length, payload);
  Serial.println("sent to I2C...");
}


void connectAWS()
{
  delay(3000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println(String("Attempting to connect to SSID: ") + String(WIFI_SSID));

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }

  NTPConnect();

  net.setTrustAnchors(&cert);
  net.setClientRSACert(&client_crt, &key);

  client.setServer(MQTT_HOST, 8883);
  client.setCallback(messageReceived);


  Serial.println("Connecting to AWS IOT");

  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(1000);
  }

  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");
}


void onRecieveI2C(int len) {
    DynamicJsonDocument doc(JSON_MAX_SIZE);
    deserializeJson(doc, Wire);
    //doc["time"] = millis();
    char jsonBuffer[JSON_MAX_SIZE];
    serializeJson(doc, jsonBuffer);
    client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}


void setup()
{
  Serial.begin(115200);
  //Wire.begin(D1, D2);
  Wire.begin(I2C_ESP8266_ADDRESS);
  Wire.onReceive(onRecieveI2C);
  connectAWS();
}


void loop()
{
  //delay(2000);

  now = time(nullptr);

  if (!client.connected())
  {
    Serial.println(client.state());
    connectAWS();
  }
  else
  {
    client.loop();
    if (millis() - lastMillis > 5000)
    {
      lastMillis = millis();
      //publishMessage();
    }
  }
}

/*
  // Replace with your network credentials
  const char* ssid     = "WiFi-1463";
  const char* password = "14678370";

  // Set web server port number to 80
  WiFiServer server(80);

  // Variable to store the HTTP request
  String header;

  // Auxiliar variables to store the current output state
  String output5State = "off";
  String output4State = "off";

  // Assign output variables to GPIO pins
  const int output5 = 5;
  const int output4 = 4;

  // Current time
  unsigned long currentTime = millis();
  // Previous time
  unsigned long previousTime = 0;
  // Define timeout time in milliseconds (example: 2000ms = 2s)
  const long timeoutTime = 2000;

  void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output5, OUTPUT);
  pinMode(output4, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output5, LOW);
  digitalWrite(output4, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  }

  void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("GPIO 5 on");
              output5State = "on";
              digitalWrite(output5, HIGH);
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("GPIO 5 off");
              output5State = "off";
              digitalWrite(output5, LOW);
            } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              output4State = "on";
              digitalWrite(output4, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              output4State = "off";
              digitalWrite(output4, LOW);
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>ESP8266 Web Server</h1>");

            // Display current state, and ON/OFF buttons for GPIO 5
            client.println("<p>GPIO 5 - State " + output5State + "</p>");
            // If the output5State is off, it displays the ON button
            if (output5State=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 4
            client.println("<p>GPIO 4 - State " + output4State + "</p>");
            // If the output4State is off, it displays the ON button
            if (output4State=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  }
*/
