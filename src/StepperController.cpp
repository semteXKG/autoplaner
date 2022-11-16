#include <StepperController.h>

StepperController::StepperController(SharedData* sharedData, gpio_num_t pulsePin, gpio_num_t directionPin) {
    stepper = new AccelStepper(AccelStepper::DRIVER, pulsePin, directionPin);
    this->sharedData = sharedData;
    stepper->setMaxSpeed(OP_SPEED);
    stepper->setAcceleration(10 * PULSE_PER_MM);
    stepper->setMinPulseWidth(50);
    stepper->setCurrentPosition(0);  
}

void StepperController::handleBottomOut() {
    if (sharedData->bottomOut->rose()) {
        stepper->stop();
    }
}

void StepperController::handleCalibration() {
    if(sharedData->getState() == MachineState::CALIBRATING) {
        if (!calibPhase1 && !calibPhase2) {
            Serial.println("ph1");
            calibPhase1 = true;
            stepper->move(-5000000);
        }

        if (calibPhase1 && sharedData->bottomOut->isPressed() && !this->stepper->isRunning()) {
            Serial.println("ph2");
            calibPhase2 = true;
            stepper->setMaxSpeed(CAL_SPEED);
            stepper->setSpeed(CAL_SPEED);
            stepper->move(5000000);
        }

        if(calibPhase2 && !sharedData->bottomOut->isPressed()) {
            Serial.println("end");
            stepper->stop();
            calibrationDone();
        }
    }
}

void StepperController::calibrationDone() {
    stepper->setMaxSpeed(OP_SPEED);
    stepper->setSpeed(OP_SPEED);
    calibPhase1 = false;
    calibPhase2 = false;
    sharedData->switchState(MachineState::IDLE);
    sharedData->setCurrentPosition(CAL_POSITION_MM);
    sharedData->setPosition(CAL_POSITION_MM);
    sharedData->scheduleDisplayUpdate();
}

void StepperController::handlePosition() {
    if(sharedData->getState() == MachineState::PREP_MOVING) {
        double distance = sharedData->getCurrentPosition() - sharedData->getTargetPosition();
        totalDistanceInPulses = distance * PULSE_PER_MM;
        if(distance < 0) {
           sharedData->switchState(MachineState::MOVING_DOWN_OVERSHOOT);
           totalDistanceInPulses = totalDistanceInPulses - DEAD_ZONE - OVERSHOOT; 
        } else {
            sharedData->switchState(MachineState::MOVING_UP);
        }
        Serial.print("Pulses: ");
        Serial.println(totalDistanceInPulses);
        stepper->move(totalDistanceInPulses);
    } else if(sharedData->getState() == MachineState::MOVING_DOWN_OVERSHOOT && !stepper->isRunning()) {
        Serial.println("correcting overshoot");
        sharedData->switchState(MachineState::MOVING_DOWN_CORRECTION);
        stepper->move(DEAD_ZONE + OVERSHOOT);
    } else if((sharedData->getState() == MachineState::MOVING_DOWN_CORRECTION || sharedData->getState() == MachineState::MOVING_UP) && !stepper->isRunning()) {
        Serial.println("going lock");
        sharedData->switchState(PREP_LOCK);
    }
}


void StepperController::updateCurrentPosition() {
    if(sharedData->getState() == MachineState::MOVING_UP || sharedData->getState() == MachineState::MOVING_DOWN_CORRECTION || sharedData->getState() == MOVING_DOWN_OVERSHOOT) {
       double distanceLeft = stepper->distanceToGo() / (PULSE_PER_MM * 1.0);
       sharedData->setCurrentPosition(sharedData->getTargetPosition() + distanceLeft);
    }
}

void StepperController::tick() {
    handleBottomOut();
    handleCalibration();
    handlePosition();
    if (calibPhase2) {
        stepper->runSpeed();
    } else {
        stepper->run();
    }
    updateCurrentPosition();
}

StepperController::~StepperController() {
}
