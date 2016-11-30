// This example sketch connects to shiftr.io
// and sends a message on every kestroke.
//
// After starting the sketch you can find the
// client here: https://shiftr.io/try.
//
// by Joël Gähwiler
// https://github.com/256dpi/processing-mqtt

import mqtt.*;

MQTTClient client;

int SEND_DELAY = 1000; // how often messages are sent in milliseconds
String CLIENT_NAME = "Waldemar"; // change this to whatever you like, e.g. your name

String MQTT_SERVER = "broker.shiftr.io";
String MQTT_USERNAME = "try";
String MQTT_PASSWORD = "try";

void setup() {
  client = new MQTTClient(this);
  client.connect("mqtt://" + MQTT_USERNAME + ":" + MQTT_PASSWORD + "@" + MQTT_SERVER, CLIENT_NAME);
}

void draw() {}

void keyPressed() {
  int randomNumber = round(random(0, 10));
  client.publish("/justtesting", str(randomNumber)); // send random number as a string, str() converts a number to a string
  println("MQTT message send: " + randomNumber);
}