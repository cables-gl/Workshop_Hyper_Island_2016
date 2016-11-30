// This example sketch connects to shiftr.io
// and listenes for incoming messages

import mqtt.*;

MQTTClient client;

int SEND_DELAY = 1000; // how often messages are sent in milliseconds
String CLIENT_NAME = "Waldemar"; // change this to whatever you like, e.g. your name

String MQTT_SERVER = "broker.shiftr.io";
String MQTT_USERNAME = "try";
String MQTT_PASSWORD = "try";

String lastMessage = "";

void setup() {
  client = new MQTTClient(this);
  client.connect("mqtt://" + MQTT_USERNAME + ":" + MQTT_PASSWORD + "@" + MQTT_SERVER, CLIENT_NAME);
  client.subscribe("/justtesting");
  fill(0);
  noStroke();
}

void draw() {
  background(255);
  textMode(CENTER);
  text(lastMessage, width/2, height/2);
}

void messageReceived(String topic, byte[] payload) {
  String message = new String(payload);
  println("new message: " + topic + " - " + message);
  lastMessage = message;
}