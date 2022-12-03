#ifndef calibrator_h
#define calibrator_h

#include <Arduino.h>
#include <SharedData.h>
#include <LockController.h>

class Calibrator {
private:
    SharedData* sharedData;
    LockController* lockController;
    bool calibrationRunning;
public:
    Calibrator(SharedData* sharedData, LockController* lockController);
    ~Calibrator();
    void tick();
};

#endif