#ifndef stepper_controller
#define stepper_controller

#include <Arduino.h>
#include <AccelStepper.h>
#include <SharedData.h>

class StepperController {
private:
    AccelStepper* stepper;
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
    StepperController(SharedData* sharedData, gpio_num_t pulsePin, gpio_num_t directionPin);
    void tick();
    ~StepperController();
};

#endif