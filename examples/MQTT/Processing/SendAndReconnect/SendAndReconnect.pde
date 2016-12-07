// This example sketch connects to shiftr.io
// and sends a message every second
//
// After starting the sketch you can find the
// client here: https://shiftr.io/try.

import mqtt.*;
// imports to capture console output
import java.io.PrintStream;

MQTTClient client;

int SEND_DELAY = 1000; // how often messages are sent in milliseconds
String CLIENT_NAME = "Waldemar"; // change this to whatever you like, e.g. your name

String MQTT_SERVER = "broker.shiftr.io";
String MQTT_USERNAME = "try";
String MQTT_PASSWORD = "try";

long lastSend = 0; // the last time a MQTT message was sent

void connect(){
  println("Connecting to MQTT Broker...");
  client.connect("mqtt://" + MQTT_USERNAME + ":" + MQTT_PASSWORD + "@" + MQTT_SERVER, CLIENT_NAME);
}

void setup() {
  client = new MQTTClient(this);
  connect();
  // automatic reconnection
  PrintStream origOut = System.out;
  PrintStream interceptor = new Interceptor(origOut);
  System.setOut(interceptor);// just add the interceptor
}

void draw() {
  // send a message every second
  if(millis() > lastSend + SEND_DELAY) {
    client.publish("/justtesting", "Hi");
    println("MQTT message sent...");
    lastSend = millis();
  }
}

// Helper, to reconnect on disconnect
private class Interceptor extends PrintStream {
    public Interceptor(OutputStream out) {
        super(out, true);
    }
    @Override
    public void print(String s) {
        super.print(s);
        if(s.indexOf("[MQTT] failed to publish") != -1) {
          connect();
        }
    }
}