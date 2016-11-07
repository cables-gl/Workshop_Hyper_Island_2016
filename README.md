# Workshop_Hyper_Island_2016
Examples and tutorials for the cables-workshop at Hyper Island School of Design in Stockholm, Sweden 2016/12

## What will be covered

- How cables works
- How to get going in cables / how to find information


- How to connect Arduino MKR1000 to a MQTT-broker
- How to send data from Arduino to cables
- How to send data from cables to Arduino
- (How to store data in a database – [ThingSpeak](https://thingspeak.com) / [Arduino Cloud](https://cloud.arduino.cc))
- How to use data coming from an Arduino (e.g. proximity / orientation) to …
  - change existing visualizations
  - play a video file and seek to a certain position
  - play an audio file
  - (control an audio-effect)

## Connecting the MKR1000 to the internet

- Certain router-encryptions are not supported (e.g. WPA-WPA2 mixed), an update to WPA2 helps here
- Test if your connection works by running `Arduino` —> `Examples` —> `WiFi101` —> `ConnectWithWPA` / `ConnectWithWEP` (depending on the encryption type of the router)
- More infos about the [WiFi101-Library](https://www.arduino.cc/en/Reference/WiFi101)

## Connecting Devices

- ​There are multiple ways on how to make microcontrollers / computers talk to each other 

### Talking over the internet

- Every major microcontroller-brand has their own data cloud

- For Particle Photon / Core there is the [Particle Cloud](https://www.particle.io/products/platform/particle-cloud)

- For Arduino there is the [Arduino Cloud](https://cloud.arduino.cc/cloud)

- Using the cloud-service of your microcontroller is the **easiest** way to connect things, but maybe **not the fastest**

- You can use regular HTTP Get request to receive data, e.g. via an API (e.g. weather, twitter stream) or HTTP POST requests to send data (post to Twitter, log sensor data, …)

  - See [programmableweb.com/apis/directory](http://www.programmableweb.com/apis/directory) for an overview of public APIs

- A fast and easy standard for M2M (machine to machine) communication is **MQTT**

  - To use MQTT you need an MQTT-server (broker) where you send the information to
  - [shiftr.io](https://shiftr.io) is a free broker which allows you to send and receive data very fast

## Useful Ressources

- [Arduino code to trigger IFTTT event](https://gist.github.com/outofmbufs/d6ced37b49a484c495f0)
- [How to trigger actions on Arduino using thinger.io](https://community.thinger.io/t/make-your-iot-things-react-to-hundred-of-ifttt-events/41)
- [Maker Channel on IFTTT](https://ifttt.com/maker)
- [thinger.io](https://thinger.io/)
- [parse.com](http://parse.com/)
  - Push notifications to Arduino (uses MQTT!?)
  - Data-store

