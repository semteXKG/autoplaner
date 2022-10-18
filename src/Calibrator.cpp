#include <Calibrator.h>

Calibrator::Calibrator(SharedData* sharedData) {
    this->sharedData = sharedData;
}

Calibrator::~Calibrator() {
}

void Calibrator::tick() {
    if(this->calibrationRunning) {
        if(this->sharedData->getState() == SharedData::MachineState::BOTTOM_OUT) {
            this->calibrationRunning = false;
            this->sharedData->setCurrentPosition(210.0);
        }
    }
}

void Calibrator::startCalibration() {
    if (this->sharedData->getState() == SharedData::MachineState::BOTTOM_OUT) {
        Serial.println("ERROR; BOTTOM OUT!");
    } else {
        this->calibrationRunning = true;
    } 
}