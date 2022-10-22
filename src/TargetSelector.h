#ifndef target_selector_h
#define target_selector_h

#include <Arduino.h>
#include <RotaryEncoder.h>
#include <SharedData.h>

class TargetSelector {
    private:
        RotaryEncoder* encoder;
        SharedData* sharedData;
        int prevEncPosition;
    public:
        TargetSelector(int pin1, int pin2, SharedData* sharedData);
        ~TargetSelector();
        void tick();
};

#endif