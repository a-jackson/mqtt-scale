#include "HX711.h"
#include "scale.h"

#define LOADCELL_DOUT_PIN  13
#define LOADCELL_SCK_PIN  12
#define CALIBRATION_FACTOR 417.5f

Scale::Scale() {
    this->scale.begin(
        LOADCELL_DOUT_PIN,
        LOADCELL_SCK_PIN);
    this->scale.set_scale(
        CALIBRATION_FACTOR);
    this->scale.tare();
}

float Scale::getReading() {
    return this->scale.get_units(3);
}
