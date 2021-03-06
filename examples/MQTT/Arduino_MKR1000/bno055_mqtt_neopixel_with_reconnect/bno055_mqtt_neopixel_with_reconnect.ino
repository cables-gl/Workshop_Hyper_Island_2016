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
 * Connections: 
 * - BNO055: Arduino MKR1000
 *   - GND: GND
 *   - VIN: VCC
 *   - SDA: SDA
 *   - SCL: SCL
 *  - Adafruit NeoPixel Ring 12: Arduino MKR1000
 *    - GND: GND
 *    - 5V Power: VCC
 *    - Data Input: 6
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
#include <MQTTClient.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

bool debug = false;


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include "config.h" // WLAN / MQTT user credentials

// how often messages are send (in milliseconds)
#define SEND_DELAY_MS (100)

Adafruit_BNO055 bno = Adafruit_BNO055(55);

int status = WL_IDLE_STATUS;     // the Wifi radio's status
unsigned long lastMillis = 0;

#define NEOPIXEL_PIN 6
#define NUMPIXELS 12

WiFiClient net;
MQTTClient client;

// rgb color values
int r = 50;
int g = 50;
int b = 50;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void connectToWlan();
void connectToMqttServer();
void connect();

void connectToWlan() {
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    if(debug) {
      Serial.println("WiFi shield not present");  
    }
    // don't continue:
    while (true);
  }

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    if(debug) {
      Serial.print("Attempting to connect to WPA SSID: ");
      Serial.println(ssid);  
    }
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);  
    // wait 10 seconds for connection:
    delay(2000);
  }
  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();
}

void connect() {
  connectToWlan();  
  connectToMqttServer();
  client.subscribe("/color");
  if(debug) {
    Serial.println("");  
  }
  client.publish("/setup", "MKR1000 check check!");
}

void connectToMqttServer() {
  if(debug) {
    Serial.print("Connecting to MQTT-server "); Serial.println(mqtt_server);  
  }
  client.begin(mqtt_server, net);
  while (!client.connect(device_name, mqtt_username, mqtt_password)) {
    if(debug) {
      Serial.print(".");  
    }
    delay(1000);
  }
}

void setup() {
  if(debug) {
    Serial.begin(115200);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }  
  }
  
  pixels.begin(); // This initializes the NeoPixel library.
  connect();

  if(debug) {
    Serial.println("Orientation Sensor Test"); Serial.println("");  
  }

  /* Initialise the sensor */
  if(!bno.begin()) {
    /* There was a problem detecting the BNO055 ... check your connections */
    if(debug) {
      Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");  
    }
    while(1);
  }

  /* Display some basic information on this sensor */
  displaySensorDetails();
}

void loop() {
  client.loop();
  if(!client.connected()) {
    connect();
  }

  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
  pixels.show(); // This sends the updated pixel color to the hardware.

  /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);

  // publish a message when it's time
  if(millis() - lastMillis > SEND_DELAY_MS) {
    lastMillis = millis();
    
    
      String orientationString = String("") +
          event.orientation.x + "," +
          event.orientation.y + "," +
          event.orientation.z
      ;
    
    // The channel this device sends to, the message
    client.publish("/orientation", orientationString);

    /* roll, pitch, heading, see diagram on top */
    /*
    Serial.print(F("Orientation: "));
    Serial.print((float)event.orientation.x);
    Serial.print(F(" "));
    Serial.print((float)event.orientation.y);
    Serial.print(F(" "));
    Serial.print((float)event.orientation.z);
    Serial.println(F(""));
    */
  }
}

void printWifiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  if(debug) {
    Serial.print("IP Address: ");
    Serial.println(ip);
    Serial.println(ip);  
  }

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  if(debug) {
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
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  if(!debug) {
    return;
  }

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
  if(debug) {
    Serial.print("incoming: ");
    Serial.print(topic);
    Serial.print(" - ");
    Serial.print(payload);
    Serial.println();  
  }
  
  int commaIndex = payload.indexOf(',');
  //  Search for the next comma just after the first
  int secondCommaIndex = payload.indexOf(',', commaIndex+1);
  String firstValue = payload.substring(0, commaIndex);
  String secondValue = payload.substring(commaIndex+1, secondCommaIndex);
  String thirdValue = payload.substring(secondCommaIndex+1); // To the end of the string
  r = firstValue.toInt();
  g = secondValue.toInt();
  b = thirdValue.toInt();
  if(debug) {
    Serial.print("New color: "); Serial.print(r); Serial.print(", "); Serial.print(g); Serial.print(", "); Serial.println(b);  
  }
}

/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(void) {
  if(!debug) {
    return;
  }
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


