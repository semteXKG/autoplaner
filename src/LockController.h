#ifndef LOCK_CONTROLLER_H
#define LOCK_CONTROLLER_H

#include <Arduino.h>
#include <FastAccelStepper.h>
#include <SharedData.h>

class LockController {
    private:
        SharedData* sharedData;
        FastAccelStepper* stepper;
    public:
        LockController(SharedData* sharedData, FastAccelStepperEngine* engine, gpio_num_t pulsePin, gpio_num_t directionPin);
        void tick();
        void lock(bool stateOnly);
        void unlock(bool stateOnly);
};

#endif