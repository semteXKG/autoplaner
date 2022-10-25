#ifndef hardware_button_manager_h
#define hardware_button_manager_h

#include <Arduino.h>
#include <SharedData.h>

class HardwareButtonManager {
private:
    gpio_num_t enterButtonPin, bottomOutButtonPin, speedButtonPin;
    bool enterAlreadyPressed = false, speedButtonAlreadyPressed = false;
    SharedData* sharedData;
    void handleSpeedButton();
    void handleEnterButton();
    void handleBottomOutButton();
    void monitorTriggerButton(bool* monitorVar, bool* buttonPressed, bool* buttonTriggered);
public:
    HardwareButtonManager(gpio_num_t enterButtonPin, gpio_num_t bottomOutButtonPin, gpio_num_t speedButtonPin, SharedData* sharedData);
    ~HardwareButtonManager();
    void tick();
};

#endif