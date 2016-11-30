
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
  client.subscribe("/example");
  // client.unsubscribe("/example");
  fill(255);
  noStroke();
}

PVector pos = new PVector(width/2, 0);
int dir = 1;
float inc = 15;

int cDiam = 50;

void draw() {
  background(0);
  pos.y += (dir*inc);
  if(pos.y >= height || pos.y <= 0) {
    dir *= -1;  
  }
  ellipse(pos.x, pos.y, cDiam, cDiam);
  float val = map(pos.y, 0, height, 0, 1);
  //println("Sending: " + val);
  client.publish("/circle/r", str(val));
}

void messageReceived(String topic, byte[] payload) {
  println("new message: " + topic + " - " + new String(payload));
}