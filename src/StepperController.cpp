#include <StepperController.h>

static const int RPS = 400;

static const int OP_SPEED = 8 * RPS;
static const int CAL_SPEED = 0.2 * RPS;

StepperController::StepperController(SharedData* sharedData, gpio_num_t pulsePin, gpio_num_t directionPin) {
    stepper = new AccelStepper(AccelStepper::DRIVER, pulsePin, directionPin);
    this->sharedData = sharedData;
    stepper->setMaxSpeed(OP_SPEED);
    stepper->setAcceleration(10 * RPS);
    stepper->setMinPulseWidth(50);
    stepper->setCurrentPosition(0);  
}

void StepperController::handleBottomOut() {
    if (this->sharedData->bottomOutPressed) {
        stepper->stop();
    }
}

void StepperController::handleCalibration() {
    if(sharedData->getState() == MachineState::CALIBRATING) {
        if (!phase1 && !phase2) {
            Serial.println("ph1");
            phase1 = true;
            stepper->move(5000000);
        }

        if (phase1 && this->sharedData->bottomOutPressed && !this->stepper->isRunning()) {
            Serial.println("ph2");
            phase2 = true;
            stepper->setMaxSpeed(CAL_SPEED);
            stepper->move(-5000000);
        }

        if(phase2 && !this->sharedData->bottomOutPressed) {
            Serial.println("end");
            stepper->stop();
            stepper->setMaxSpeed(OP_SPEED);
            calibrationDone();
        }
    }
}

void StepperController::calibrationDone() {
    stepper->setMaxSpeed(OP_SPEED);
    phase1 = false;
    phase2 = false;
    sharedData->switchState(MachineState::IDLE);
    sharedData->setCurrentPosition(210.0);
    sharedData->setPosition(210.0);
    sharedData->scheduleDisplayUpdate();
}

void StepperController::handlePosition() {
    if(this->sharedData->getState() == MachineState::MOVING && !stepper->isRunning()) {
        if(sharedData->getCurrentPosition() != sharedData->getTargetPosition()) {
            double distance = sharedData->getCurrentPosition() - sharedData->getTargetPosition();
            totalDistanceInPulses = distance * PULSE_PER_MM;
            stepper->move(totalDistanceInPulses);
        } else {
            sharedData->switchState(MachineState::IDLE);
        }
    }
}

void StepperController::updateCurrentPosition() {
    if(sharedData->getState() == MachineState::MOVING) {
       double distanceLeft = stepper->distanceToGo() / (PULSE_PER_MM * 1.0);
       sharedData->setCurrentPosition(sharedData->getTargetPosition() + distanceLeft);
    }
}

void StepperController::tick() {
    handleBottomOut();
    handleCalibration();
    handlePosition();
    if (phase2) {
        stepper->runSpeed();
    } else {
        stepper->run();
    }
    updateCurrentPosition();
}

StepperController::~StepperController() {
}
