// This example sketch connects to shiftr.io
// and sends a message every second
//
// After starting the sketch you can find the
// client here: https://shiftr.io/try.

import mqtt.*;
import java.io.PrintStream;

MQTTClient client;

private class Interceptor extends PrintStream {
    public Interceptor(OutputStream out)
    {
        super(out, true);
    }
    @Override
    public void print(String s)
    {//do what ever you like
        super.print(s);
    }
}
public static void main(String[] args)
{
    PrintStream origOut = System.out;
    PrintStream interceptor = new Interceptor(origOut);
    System.setOut(interceptor);// just add the interceptor
}

int SEND_DELAY = 1000; // how often messages are sent in milliseconds
String CLIENT_NAME = "Waldemar"; // change this to whatever you like, e.g. your name

String MQTT_SERVER = "broker.shiftr.io";
String MQTT_USERNAME = "try";
String MQTT_PASSWORD = "try";

long lastSend = 0; // the last time a MQTT message was sent

void setup() {
  client = new MQTTClient(this);
  client.connect("mqtt://" + MQTT_USERNAME + ":" + MQTT_PASSWORD + "@" + MQTT_SERVER, CLIENT_NAME);
}

void draw() {
  // send a message every second
  if(millis() > lastSend + SEND_DELAY) {
    client.publish("/justtesting", "Hi");
    println("MQTT message sent...");
    lastSend = millis();
  }
}