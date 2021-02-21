#include "HX711.h"

class Scale {
private:
    HX711 scale;
public:
    Scale();

    float getReading();
};
