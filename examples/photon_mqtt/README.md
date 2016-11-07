# Photon MQTT Example

- Upload code to your Particle Photon

- Change the lines:

  ```C
  #define DEVICE_NAME "Waldemar's Photon"
  #define SUBSCRIBE_CHANNEL "waldemar/to_photon"
  #define PUBLISH_CHANNEL "waldemar/from_photon"
  ```

- To include your own name or something more unique, the `to_photon`, `from_photon` part shouldn’t be removed

- Open `Terminal.app` and try out one of the following commands to change the LED of you *Particle Photon* (don’t forget to change `waldemar/to_photon`  to the same as  what you put in `SUBSCRIBE_CHANNEL` above:

```
curl -X POST "http://try:try@broker.shiftr.io/waldemar/to_photon" -d "RED"
curl -X POST "http://try:try@broker.shiftr.io/waldemar/to_photon" -d "GREEN"
curl -X POST "http://try:try@broker.shiftr.io/waldemar/to_photon" -d "BLUE"
```

- Also check [shiftr.io/try](https://shiftr.io/try#terminal) and try to find your name there. Every time you send a message (either way) it will pop up there as well