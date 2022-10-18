#ifndef InputManager_h
#define InputManager_h

#include "SharedData.h"
#include <RotaryEncoder.h>
#include <Arduino.h>

static const double INCREMENT_NORMAL_IN_MM = 0.1;
static const double INCREMENT_FAST_IN_MM = 5;

static const double MAX_POSITION = 220;
static const double MIN_POSITION = 8;

class SharedData;

class InputManager {
    private:
        RotaryEncoder* encoder;
        SharedData* sharedData;
        gpio_num_t speedButtonPin, enterButtonPin;
        double increment;
        int prevEncPosition;
        bool speedButtonPressed, enterButtonPressed;
        void handleSpeedButton();
        void handleSpeedButtonPressed();
        void handleSpeedButtonDepressed();
        void handleEnterButton();
    public:
        InputManager(int pin1, int pin2, gpio_num_t speedButtonPin, gpio_num_t enterButtonPin);
        ~InputManager();
        void tick();
};

#endif