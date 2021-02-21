# ESP8266 MQTT Scale

The purpose of this project is to weigh our guinea pigs regularly and publish the results on MQTT.
These messages are picked up by another service I wrote [weight-recorder](https://github.com/a-jackson/weight-recorder) and logged to an Influx DB.

Until now I've been using a dashboard in Home Assistant to type in the weights and then press a button to send it on MQTT for logging.
This project allows the scale to publish the weight directly so I don't have to type.

I'm using an ESP8266 D1 Mini with a 5kg load cell on a HK711 amplifier and a 128x64 OLED screen.
A momentary button is used to select which guinea pig I'm weighing, then a long press publishes the weight on MQTT.

A secrets.h file is required to specify the WiFi SSID and password as well as the MQTT connection details.
