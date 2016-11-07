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
  client = new MQTTClient(this);
  client.connect("mqtt://try:try@broker.shiftr.io", "Waldemar’s P5");
  client.subscribe("/waldemar/from_photon");
  // client.unsubscribe("/example");
}

void draw() {}

void keyPressed() {
  int i = round(random(0, 2));
  switch(i) {
    case 0:
      client.publish("/waldemar/to_photon", "RED");
      break;
  case 1:
    client.publish("/waldemar/to_photon", "BLUE");
    break;
  default:
    client.publish("/waldemar/to_photon", "GREEN");
    break;
  }
  
}

void messageReceived(String topic, byte[] payload) {
  println("new message: " + topic + " - " + new String(payload));
}