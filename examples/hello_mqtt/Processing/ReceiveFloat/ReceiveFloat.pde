import java.util.Arrays;

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

void setup() {
  size(200, 200);
  frameRate(8);
  client = new MQTTClient(this);
  client.connect("mqtt://try:try@broker.shiftr.io", "p5 write");
  client.subscribe("/circle/r");
  // client.unsubscribe("/example");
  fill(255);
  noStroke();
  textAlign(CENTER);
}

float diam = 10;

int cDiam = 50;

void draw() {
  background(0);
  text("Receive", width/2, 20);
  ellipse(width/2, height/2, diam, diam);
}

void messageReceived(String topic, byte[] payload) {
  println("new message: " + topic + " - " + new String(payload));
  String inString = new String(payload);
  float inFloat = float(inString);
  diam = map(inFloat, 0, 1, 10, 100);
  println(diam);
}