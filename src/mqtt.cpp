#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>
#include <ctype.h>
#include "secrets.h"
#include "mqtt.h"

#define DEVICE_ID "esp-scale-"
#define TOPIC "test/weights/%s/set"

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(SECRET_SSID, SECRET_PSK);
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("Connected to Wi-Fi.");
  connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi.");
  mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.once(2, connectToWifi);
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.print("Disconnected from MQTT. Reason: ");
  Serial.println((int8_t)reason);

  if (WiFi.isConnected()) {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}

void onMqttPublish(uint16_t packetId) {
  Serial.println("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void toLower(char *str)
{
  for (; *str; ++str)
  {
    *str = tolower(*str);
  }
}

void setupMqtt() {
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(SECRET_MQTT_HOST, SECRET_MQTT_PORT);
  mqttClient.setCredentials(SECRET_MQTT_USER, SECRET_MQTT_PASS);
}

void sendWeight(
  const char *name,
  float weight)
{
  char topic[30];
  snprintf(topic, 30, TOPIC, name);
  toLower(topic);
  char value[10];
  snprintf(value, 10, "%.0f", weight);

  mqttClient.publish(
      topic,
      2,
      false,
      value);
}

int32 getRSSI()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        return -999;
    }

    return WiFi.RSSI();
}
