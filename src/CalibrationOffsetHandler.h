#ifndef calibration_offset_handler
#define calibration_offset_handler

#include <Arduino.h>
#include <SharedData.h>
#include <ArduinoNvs.h>

static const char* KEY_OFFSET = "offset";

class CalibrationOffsetHandler {
private:
    SharedData* sharedData;
public:
    CalibrationOffsetHandler(SharedData* sharedData);
    ~CalibrationOffsetHandler();
    void tick();
};

#endif