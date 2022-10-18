#ifndef environment_sensors_h
#define environment_sensors_h

#include <Arduino.h>
#include <SharedData.h>

class EnvironmentSensors {
private:
    SharedData* sharedData;
    gpio_num_t bottomOutSensorPin;
public:
    EnvironmentSensors(SharedData* sharedData, gpio_num_t bottomOutSensorPin);
    ~EnvironmentSensors();
    void tick();
    void handleBottomOutSensor();
};

EnvironmentSensors::EnvironmentSensors(SharedData* sharedData, gpio_num_t bottomOutSensorPin) {
    this->bottomOutSensorPin = bottomOutSensorPin;
    pinMode(bottomOutSensorPin, PULLDOWN);
}

EnvironmentSensors::~EnvironmentSensors() {
}

void EnvironmentSensors::handleBottomOutSensor() {
    int val = digitalRead(this->bottomOutSensorPin);
    if (val == 0) {
        sharedData->switchState(SharedData::MachineState::BOTTOM_OUT);
    }
}

void EnvironmentSensors::tick() {
    handleBottomOutSensor();
}



#endif