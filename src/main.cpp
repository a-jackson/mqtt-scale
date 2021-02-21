#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "display.h"
#include "scale.h"
#include "mqtt.h"
#include "timer.h"

#define CONTROL_PIN 14
#define LONG_PRESS 500
#define DRAW_RATE 100
#define TURN_OFF 60e3
#define MQTT_UPDATE 1000
#define DEBOUNCE 25
#define NUM_NAMES 6

const char *names[] = {
    "Roy",
    "Bruce",
    "Jack",
    "Trevor",
    "Percy",
    "Ron"};

Display display = Display(names, NUM_NAMES);
Scale scale;

Timer turnOff = Timer(TURN_OFF);
Timer longPress = Timer(LONG_PRESS);
Timer draw = Timer(DRAW_RATE);
Timer debounce = Timer(DEBOUNCE);

bool isButtonDown = false;
bool isLongPress = false;

uint8 currentName = 0;
float currentWeight = 0;
int32 rssi;

void controlButtonPress();
void drawScreen();

void setup()
{
  Serial.begin(9600);
  display.clear();
  setupMqtt();

  pinMode(CONTROL_PIN, INPUT_PULLUP);
}

void loop()
{
  controlButtonPress();

  if (draw.elapsed())
  {
    drawScreen();
    draw.reset();
  }

  if (turnOff.elapsed())
  {
    display.turnOff();
    ESP.deepSleep(0);
  }
}

void drawScreen() {
  float reading = scale.getReading();
  if (abs(reading - currentWeight) > 20)
  {
    turnOff.reset();
  }

  currentWeight = reading;
  rssi = getRSSI();

  display.draw(
      currentName,
      currentWeight,
      rssi);
}

void controlButtonPress()
{
  if (!digitalRead(CONTROL_PIN))
  {
    if (!isButtonDown && debounce.elapsed())
    {
      debounce.reset();
      longPress.reset();
      isButtonDown = true;
    }

    if (longPress.elapsed() && !isLongPress)
    {
      isLongPress = true;

      sendWeight(
        names[currentName],
        currentWeight);

      display.invert(50);
    }

    turnOff.reset();
  }
  else if (isButtonDown)
  {
    if (!isLongPress)
    {
      currentName = (currentName + 1) % NUM_NAMES;
    }

    isLongPress = false;
    isButtonDown = false;
  }
}
