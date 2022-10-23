#include <EnvironmentSensors.h>

EnvironmentSensors::EnvironmentSensors(SharedData* sharedData) {
    this->sharedData = sharedData;
}

EnvironmentSensors::~EnvironmentSensors() {
}

void EnvironmentSensors::handleBottomOutSensor() {
    
}

void EnvironmentSensors::tick() {
    handleBottomOutSensor();
}
