#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "display.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Display::Display(
    const char **names,
    int numNames)
{
  this->names = names;
  this->numNames = numNames;

  this->display = Adafruit_SSD1306(
      SCREEN_WIDTH,
      SCREEN_HEIGHT,
      &Wire,
      OLED_RESET);

  this->display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
}

void Display::drawInitialise()
{
  this->clear();
  this->display.setTextSize(2);
  this->display.setCursor(0, 0);
  this->display.setTextColor(
      SSD1306_WHITE);
  this->display.print("Starting ...");
  this->display.display();
}

void Display::draw(
    uint8 selectedName,
    float currentWeight,
    int32 rssi)
{
  this->clear();

  this->display.setTextSize(1);
  this->display.setCursor(0, 0);

  for (int i = 0; i < this->numNames; i++)
  {
    this->writeMenuItem(
        this->names[i],
        i == selectedName);
  }

  this->display.setTextSize(2);
  this->display.setCursor(50, 30);
  this->display.setTextColor(
      SSD1306_WHITE);
  this->display.print(
      currentWeight,
      0);
  this->display.print(F("g"));

  this->drawSignalStrength(
      rssi);

  this->display.display();
}

void Display::drawSignalStrength(
    int32 rssi)
{
  int bars;
  if (rssi > -55 && rssi < 0)
  {
    bars = 5;
  }
  else if (rssi <= -55 && rssi > -65)
  {
    bars = 4;
  }
  else if (rssi <= -65 && rssi > -70)
  {
    bars = 3;
  }
  else if (rssi <= -70 && rssi > -78)
  {
    bars = 2;
  }
  else if (rssi <= -78 && rssi > -100)
  {
    bars = 1;
  }
  else
  {
    bars = 0;
  }

  for (int b = 0; b <= bars; b++)
  {
    this->display.fillRect(
        100 + (b * 5),
        20 - (b * 4),
        3,
        b * 4,
        WHITE);
  }
}

void Display::clear()
{
  this->display.clearDisplay();
}

void Display::turnOff()
{
  this->display.clearDisplay();
  this->display.display();
  this->display.ssd1306_command(SSD1306_DISPLAYOFF);
}

void Display::invert(
    unsigned long delay_time)
{
  this->display.invertDisplay(true);
  delay(delay_time);
  this->display.invertDisplay(false);
}

void Display::writeMenuItem(
    const char *value,
    bool selected)
{
  if (selected)
  {
    this->display.setTextColor(
        SSD1306_BLACK,
        SSD1306_WHITE);
  }
  else
  {
    this->display.setTextColor(
        SSD1306_WHITE);
  }

  this->display.println(value);
}
