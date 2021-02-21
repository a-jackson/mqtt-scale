#include <Adafruit_SSD1306.h>

class Display
{
private:
  const char **names;
  int numNames;
  Adafruit_SSD1306 display;

  void writeMenuItem(
    const char *value,
    bool selected);

  void drawSignalStrength(
    int32 rssi);

public:
  Display(
    const char **names,
    int numNames);
  void draw(
    uint8 selectedName,
    float currentWeight,
    int32 rssi);
  void clear();
  void turnOff();
  void invert(
    unsigned long delay_time);
};
