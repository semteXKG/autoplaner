#ifndef environment_sensors_h
#define environment_sensors_h

#include <Arduino.h>
#include <SharedData.h>

class EnvironmentSensors {
private:
    SharedData* sharedData;
public:
    EnvironmentSensors(SharedData* sharedData);
    ~EnvironmentSensors();
    void tick();
    void handleBottomOutSensor();
};

#endif