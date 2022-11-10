#ifndef hardware_button_manager_h
#define hardware_button_manager_h

#include <Arduino.h>
#include <SharedData.h>

class HardwareButtonManager {
private:
    SharedData* sharedData;
public:
    HardwareButtonManager(gpio_num_t enterButtonPin, gpio_num_t bottomOutButtonPin, gpio_num_t speedButtonPin, gpio_num_t moveToConversionButtonPin, SharedData* sharedData);
    ~HardwareButtonManager();
    void tick();
};

#endif