/*
 * This example sketch reads a value from Arduino via Serial, 
 * connects to shiftr.io and forwards the same message via MQTT.

 * After starting the sketch you can see the data here:
 * https://shiftr.io/try
 * (if Arduino is connected, and sends out data)
 *
 * Dependencies:
 *   - MQTT library by Joey Gaehwiler (go to "Sketch" -> "Import Library" -> "Manage Libraries")
 */

import processing.serial.*;
import mqtt.*;
import java.io.PrintStream; // imports to capture console output

String CLIENT_NAME = "Waldemar"; // change this to whatever you like, e.g. your name

String MQTT_SERVER = "broker.shiftr.io";
String MQTT_USERNAME = "try";
String MQTT_PASSWORD = "try";

MQTTClient client;
Serial myPort;
float val;

long lastSend = 0; // the last time a MQTT message was sent

void setup() {
  size(200, 200);
 
  String portName = Serial.list()[5]; // change the '0' to whatever port is right for you (check console)
  printArray(Serial.list());
  myPort = new Serial(this, portName, 9600);
  
  client = new MQTTClient(this);
  connect();
  // automatic reconnection
  PrintStream origOut = System.out;
  PrintStream interceptor = new Interceptor(origOut);
  System.setOut(interceptor);
}

void draw() {
  if ( myPort.available() > 0) {  // If data is available,
    String in = myPort.readStringUntil('\n'); // read in the whole line
    try {
      val = parseFloat(in);
      client.publish("/justtesting", in);
      println("MQTT message sent...");
      lastSend = millis();
    } catch(Exception e) {
      val = 0;
    }
    
    println("Received: " + val);
  }
  int greyTone = int(map(val, 0, 1000, 0, 255)); // map the incoming float from range [0..1000] to [0..255] to use it as a grey-tone
  background(greyTone);
}

// Helper, to reconnect when connection to MQTT broker is lost
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

void connect(){
  println("Connecting to MQTT Broker...");
  client.connect("mqtt://" + MQTT_USERNAME + ":" + MQTT_PASSWORD + "@" + MQTT_SERVER, CLIENT_NAME);
}