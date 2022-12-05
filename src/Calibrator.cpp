#include <Calibrator.h>

Calibrator::Calibrator(SharedData* sharedData, LockController* lockController) {
    this->sharedData = sharedData;
    this->lockController = lockController;
}

Calibrator::~Calibrator() {
}

void Calibrator::tick() {
    if(this->sharedData->getState() == MachineState::CALIBRATION_NEEDED && this->sharedData->enterButton->rose() && !this->sharedData->bottomOut->isPressed()) {
        Serial.println("Starting Calibration");
        calibrationRunning = true;
        lockController->unlock(false);
        sharedData->switchState(PREP_CALIBRATION);
    }
}