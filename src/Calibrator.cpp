#include <Calibrator.h>

Calibrator::Calibrator(SharedData* sharedData) {
    this->sharedData = sharedData;
}

Calibrator::~Calibrator() {
}

void Calibrator::tick() {
    if(this->sharedData->getState() == MachineState::CALIBRATION_NEEDED && this->sharedData->enterButtonPressed) {
        this->calibrationRunning = true;
        this->sharedData->switchState(CALIBRATING);
    }

    if(this->sharedData->getState() == CALIBRATING) {
        if(this->sharedData->bottomOutPressed) {
            this->sharedData->setCurrentPosition(210.0);
            this->sharedData->switchState(MachineState::IDLE);
        }
    }
}

void Calibrator::startCalibration() {
    if (this->sharedData->bottomOutPressed) {
        Serial.println("ERROR; BOTTOM OUT!");
    } else {
        this->calibrationRunning = true;
    } 
}