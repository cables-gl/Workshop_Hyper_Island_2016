/*
 * This example uses an Arduino / Genuino Zero together with
 * a WiFi101 Shield or a MKR1000 to connect to shiftr.io.
 * 
 * Libraries which need to be installed:
 *     - "WiFi101" (install via Arduino libraries manager)
 *     - "MQTT" (by Joel Gaehwiler, install via Arduino libraries manager)
 *     
 * By undev / cables.gl    
 * https://github.com/cables-gl/Workshop_Hyper_Island_2016
 *     
 * Based on example by Gilberto Conti
 * https://github.com/256dpi/arduino-mqtt       
 * 
 * You can check if your device successfully sends messages here:
 * https://shiftr.io/try
 * Also open the serial monitor in Arduino to check for errors
 */

#include <SPI.h>
#include <WiFi101.h>
#include <MQTTClient.h>

// MQTT details
char *mqtt_server = "broker.shiftr.io";
char *mqtt_username = "try";
char *mqtt_password = "try";
char *device_name = "Dr. Prof. Funk"; // can be freely set, e.g. your name

// Wireless LAN details
char *wlan_name = "your wlan name";
char *wlan_password = "your wlan password";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void setup() {
  Serial.begin(9600);
  WiFi.begin(wlan_name, wlan_password);
  client.begin(mqtt_server, net);

  connect();
}

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnsecting...");
  while (!client.connect(device_name, mqtt_username, mqtt_password)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  // The channel this device listenes to
  client.subscribe("/toarduino"); 
}

void loop() {
  client.loop();

  if(!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if(millis() - lastMillis > 1000) {
    lastMillis = millis();
    // The channel this device sends to, the message
    client.publish("/fromarduino", "hello world");
  }
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  Serial.print("incoming: ");
  Serial.print(topic);
  Serial.print(" - ");
  Serial.print(payload);
  Serial.println();
}
