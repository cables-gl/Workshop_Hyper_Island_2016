// This example sketch connects to shiftr.io
// and listenes for incoming messages

import mqtt.*;
// imports to capture console output
import java.io.PrintStream;

MQTTClient client;

int SEND_DELAY = 1000; // how often messages are sent in milliseconds
String CLIENT_NAME = "Waldemar"; // change this to whatever you like, e.g. your name

String MQTT_SERVER = "broker.shiftr.io";
String MQTT_USERNAME = "try";
String MQTT_PASSWORD = "try";

String lastMessage = "";

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
  client.subscribe("/toprocessing");
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