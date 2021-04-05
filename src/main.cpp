#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "OneButton.h"
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
    "Jack",
    "Trevor",
    "Roy",
    "Bruce",
    "Percy",
    "Ron"};

Display display = Display(names, NUM_NAMES);
Scale scale;

Timer turnOff = Timer(TURN_OFF);
Timer draw = Timer(DRAW_RATE);

OneButton controlButton(CONTROL_PIN, true);

uint8 currentName = 0;
float currentWeight = 0;
int32 rssi;

void buttonClick();
void buttonLongPress();

void drawScreen();

void setup()
{
  Serial.begin(9600);
  display.clear();
  setupMqtt();

  pinMode(CONTROL_PIN, INPUT_PULLUP);

  controlButton.attachClick(buttonClick);
  controlButton.attachLongPressStart(buttonLongPress);
}

void loop()
{
  controlButton.tick();

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

void drawScreen()
{
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

void buttonClick() {
  turnOff.reset();
  currentName = (currentName + 1) % NUM_NAMES;
}

void buttonLongPress() {
  turnOff.reset();
  if (getRSSI() > -999) {
    sendWeight(
        names[currentName],
        currentWeight);

    display.invert(50);
  }
}
