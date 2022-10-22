#include <EnvironmentSensors.h>

EnvironmentSensors::EnvironmentSensors(SharedData* sharedData) {
    this->sharedData = sharedData;
}

EnvironmentSensors::~EnvironmentSensors() {
}

void EnvironmentSensors::handleBottomOutSensor() {
    if (sharedData->bottomOutPressed) {
        sharedData->switchState(MachineState::BOTTOM_OUT);
    }
}

void EnvironmentSensors::tick() {
    handleBottomOutSensor();
}
