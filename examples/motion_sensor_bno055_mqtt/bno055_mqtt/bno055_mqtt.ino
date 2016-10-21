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
 * 
 * Connections: (BNO055: Arduino MKR1000)
 *   - GND: GND
 *   - VIN: VCC
 *   - SDA: SDA
 *   - SCL: SCL
  */
  
  /* Board layout:
         +----------+
         |         *| RST   PITCH  ROLL  HEADING
     ADR |*        *| SCL
     INT |*        *| SDA     ^            /->
     PS1 |*        *| GND     |            |
     PS0 |*        *| 3VO     Y    Z-->    \-X
         |         *| VIN
         +----------+
  */
 

#include <SPI.h>
#include <WiFi101.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <MQTTClient.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)
#define SEND_DELAY_MS (100)

Adafruit_BNO055 bno = Adafruit_BNO055(55);

// MQTT details
char *mqtt_server = "broker.shiftr.io";
char *mqtt_username = "try";
char *mqtt_password = "try";
char *device_name = "MKR1000"; // can be freely set, e.g. your name

// Wireless LAN details
char ssid[] = "your wlan name";     //  your network SSID (name)
char pass[] = "your wlan password";  // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(2000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();

  // setup MQTT
  Serial.print("Connecting to MQTT-server "); Serial.println(mqtt_server);
  client.begin(mqtt_server, net);
  while (!client.connect(device_name, mqtt_username, mqtt_password)) {
    Serial.print(".");
    delay(1000);
  }
  client.subscribe("/color");
  Serial.println("");
  client.publish("/setup", "MKR1000 check check!");

  Serial.println("Orientation Sensor Test"); Serial.println("");

  /* Initialise the sensor */
  if(!bno.begin()) {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  /* Display some basic information on this sensor */
  displaySensorDetails();
}

void loop() {
  client.loop();

  /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);

  // publish a message when it's time
  if(millis() - lastMillis > SEND_DELAY_MS) {
    lastMillis = millis();
    //String pubString = "{"report":{"light": "" + String(lightRead) + ""}}"; 
    /*
    String pubString = 
            String("{") + 
            """x"": " + String((float)event.orientation.x) + ","
            """y"": " + String((float)event.orientation.y) + ","
            """z"": " + String((float)event.orientation.z) + 
            "}";
    */

    // JSON generation
    /*
    const int BUFFER_SIZE = 200;
    StaticJsonBuffer<BUFFER_SIZE> jsonBuffer; // increase if your object gets much or complex
    JsonObject& root = jsonBuffer.createObject();
  
    root["x"] = event.orientation.x;
    root["y"] = event.orientation.y;
    root["z"] = event.orientation.z;

    String json = String("");
    
    root.printTo(json);
    Serial.print("The JSON: ");
    Serial.println(json);
    */
    /*
    String json = String("{") +
      "\\\"x\\\":" + event.orientation.x + ","
      "\\\"y\\\":" + event.orientation.y + ","
      "\\\"z\\\":" + event.orientation.z
      + "}";
      */
      
      /*
      String json = String("{") +
      "\"x\":" + event.orientation.x + ","
      "\"y\":" + event.orientation.y + ","
      "\"z\":" + event.orientation.z
      + "}";
      */
      String json = String("") +
          event.orientation.x + "," +
          event.orientation.y + "," +
          event.orientation.z
      ;
    
    //String pubString = String((float)event.orientation.x) + "," + String((float)event.orientation.y) + "," + String((float)event.orientation.z);
    // The channel this device sends to, the message
    client.publish("/orientation", json);

    /* roll, pitch, heading, see diagram on top */
    Serial.print(F("Orientation: "));
    Serial.print((float)event.orientation.x);
    Serial.print(F(" "));
    Serial.print((float)event.orientation.y);
    Serial.print(F(" "));
    Serial.print((float)event.orientation.z);
    Serial.println(F(""));
  }
  
  //delay(BNO055_SAMPLERATE_DELAY_MS);
}

void printWifiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);

}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}


void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  Serial.print("incoming: ");
  Serial.print(topic);
  Serial.print(" - ");
  Serial.print(payload);
  Serial.println();
}

/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(void) {
  sensor_t sensor;
  bno.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}


