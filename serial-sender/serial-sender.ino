
#include <ESP8266WiFi.h>
#include <ArduinoMqttClient.h>

//#define LIGHT_SENSOR A0
//#define LED 15
//#define LED 2
#define LED 16
//#define BUTTON 4

const char* ssid = "abcdef";
const char* password = "Afg2Deh3Fed6";

WiFiClient client;
MqttClient mqttClient(client);

// MQTT settings
const char broker[] = "192.168.1.45";
int port = 1883;


//set interval for sending messages (milliseconds)
const long interval = 8000;
unsigned long previousMillis = 0;

int count = 0;

// Serial uses UART0, which is mapped to pins GPIO1 (TX) and GPIO3 (RX). 
// Serial may be remapped to GPIO15 (TX) and GPIO13 (RX) by calling Serial.swap() after Serial.begin. 
// Calling swap again maps UART0 back to GPIO1 and GPIO3.

// Serial1 uses UART1, TX pin is GPIO2. UART1 can not be used to receive data because normally it's RX pin is occupied for flash chip connection. 
// To use Serial1, call Serial1.begin(baudrate).

int incomingByte = 0; // for incoming serial data


void setup() {

  // Configure LED pin as an output
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  //Serial.begin(115200);

  // We start by connecting to a WiFi network

  //Serial.println();
  //Serial.println();
  //Serial.print("Connecting to ");
  //Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }

  //Serial.println("");
  //Serial.println("WiFi connected");
  //Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());

  // Connect to MQTT
  //Serial.print("Attempting to connect to the MQTT broker: ");
  //Serial.println(broker);

  mqttClient.setUsernamePassword("mqtt", "mqtt");
  mqttClient.setId("serial-sender");

  if (!mqttClient.connect(broker, port)) {
    //Serial.print("MQTT connection failed! Error code = ");
    //Serial.println(mqttClient.connectError());

    while (1)
      ;
  }

  //Serial.println("You're connected to the MQTT broker!");
  //Serial.println();

  // send started mqtt message
  mqttClient.beginMessage("sender");
  mqttClient.print("started");
  mqttClient.endMessage();
 

  // Setup serial connection for input
  Serial1.begin(9600, SERIAL_8E1);

  // Disable sending on RS485 port
  //pinMode(21, OUTPUT);
  //digitalWrite(21, LOW);

  digitalWrite(LED, HIGH);
}

void loop() {

  // call poll() regularly to allow the library to send MQTT keep alive which
  // avoids being disconnected by the broker
  digitalWrite(LED, LOW);

  mqttClient.poll();
 
  Serial1.print("Sent: ");
  Serial1.print(count);
  Serial1.print(" X");
  //Serial.flush();
  count ++;

  mqttClient.beginMessage("sender");
  mqttClient.print("Sent: " );
  mqttClient.print(count);
  mqttClient.print(" X");
  mqttClient.endMessage();

  digitalWrite(LED, HIGH);

  delay(5000);

}