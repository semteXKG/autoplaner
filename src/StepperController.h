#ifndef stepper_controller
#define stepper_controller

#include <Arduino.h>
#include <SharedData.h>
#include <FastAccelStepper.h>

class StepperController {
private:
    FastAccelStepper* stepper;
    SharedData* sharedData;
    bool calibPhase1 = false, calibPhase2 = false;
    bool positionMoving;
    long totalDistanceInPulses;
    void handleBottomOut();
    void handleCalibration();
    void handlePosition();
    void updateCurrentPosition();
    void calibrationDone();
public:
    StepperController(SharedData* sharedData, FastAccelStepperEngine* engine, gpio_num_t pulsePin, gpio_num_t directionPin);
    void tick();
    ~StepperController();
};

#endif