#ifndef calibrator_h
#define calibrator_h

#include <Arduino.h>
#include <SharedData.h>

class Calibrator {
private:
    SharedData* sharedData;
    bool calibrationRunning;
    void startCalibration();
public:
    Calibrator(SharedData* sharedData);
    ~Calibrator();
    void tick();
};

#endif