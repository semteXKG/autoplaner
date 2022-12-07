#include <StepperController.h>

StepperController::StepperController(SharedData* sharedData, FastAccelStepperEngine* engine, gpio_num_t pulsePin, gpio_num_t directionPin) {
    this->sharedData = sharedData;
    stepper = engine->stepperConnectToPin(pulsePin);
    stepper->setDirectionPin(directionPin);
    stepper->setSpeedInHz(OP_SPEED);
    stepper->setAcceleration(10 * PULSE_PER_MM);
    stepper->setCurrentPosition(0);  
}

void StepperController::handleBottomOut() {
    if (sharedData->bottomOut->rose()) {
        stepper->stopMove();
    }
}

void StepperController::handleCalibration() {
    if(sharedData->getState() == MachineState::CALIBRATING) {
        if (!calibPhase1 && !calibPhase2) {
            Serial.println("ph1");
            calibPhase1 = true;
            stepper->runBackward();
        }

        if (calibPhase1 && sharedData->bottomOut->isPressed() && !this->stepper->isQueueEmpty()) {
            Serial.println("ph2");
            calibPhase2 = true;
            stepper->setSpeedInHz(CAL_SPEED);
            stepper->runForward();
        }

        if(calibPhase2 && !sharedData->bottomOut->isPressed()) {
            Serial.println("end");
            stepper->forceStop();
            calibrationDone();
        }
    }
}

void StepperController::calibrationDone() {
    stepper->setSpeedInHz(OP_SPEED);
    calibPhase1 = false;
    calibPhase2 = false;
    sharedData->switchState(MachineState::IDLE);
    sharedData->setPosition(CAL_POSITION_MM + sharedData->getOffset());
    sharedData->scheduleDisplayUpdate();
}

void StepperController::handlePosition() {
    if(sharedData->getState() == MachineState::PREP_MOVING) {
        double distance = sharedData->getCurrentPosition() - sharedData->getTargetPosition(); // needs decoupeling of the position
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
        sharedData->setCurrentPosition(sharedData->getTargetPosition());
    } else if(sharedData->getState() == MachineState::MOVING_DOWN_OVERSHOOT && !stepper->isRunning()) {
        Serial.println("correcting overshoot");
        sharedData->switchState(MachineState::MOVING_DOWN_CORRECTION);
        stepper->move(DEAD_ZONE + OVERSHOOT);
    } else if((sharedData->getState() == MachineState::MOVING_DOWN_CORRECTION || sharedData->getState() == MachineState::MOVING_UP) && !stepper->isRunning()) {
        Serial.println("going lock");
        sharedData->switchState(PREP_LOCK);
    }
}

void StepperController::tick() {
    handleBottomOut();
    handleCalibration();
    handlePosition();
}

StepperController::~StepperController() {
}
