# Hello MQTT Example

*How to connect an Arduino / Genuino MKR1000 and cables*  

## How to

- Install Arduino / Genuino MKR1000 as described here: [MKR1000 Getting Started](https://www.arduino.cc/en/Guide/MKR1000)
- Install libraries `WiFi101` and `MQTT` (by Joel Gaehwiler) from the Ardiono Library Manager (`Sketch` —> `Library` —> `Manage Libraries`)
- Enter the wlan-name and password in *hello_mqtt.ino:*
  -  `*wlan_name = "your wlan name"`
  -  `*wlan_username = "your wlan password"`
- Enter a unique name for your device:
  - `char *device_name = "Dr. Prof. Funk";`
- Double click the `Reset`-button on your Arduino MKR1000, e.g. with a pencil
- Now the Arduino should show up under `Tools` —> `Port`, select it (something like `/dev/cu.usbmodem1411 (Arduino/Genuino MKR1000)`), if not try reconnecting it or another (shorter USB-cable)
- Set the correct board: `Tools` —> `Board` —> `Arduino/Genuino MKR1000`
- Press the `upload` icon (arrow pointing to the right)
- If everything worked go to [shiftr.io/try](https://shiftr.io/try#terminal) and you should see your device there 
- Open the *Terminal* app
- Enter `curl -X POST "http://try:try@broker.shiftr.io/toarduino" -d "Hi Doc`
- Open the Serial Monitor in Arduino (`Tools` —> `Serial Monitor`)
- Paste the following into the *Terminal* window `curl -X POST "http://try:try@broker.shiftr.io/toarduino" -d "Hi Doc`
- Go back to the *Arduino Serial Monitor*, you should see `incoming: /toarduino - Hi Doc`
- Check check! Communication from and to your Arduino works! 