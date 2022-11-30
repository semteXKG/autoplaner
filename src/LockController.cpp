#include <LockController.h>

LockController::LockController(SharedData* sharedData, gpio_num_t pulsePin, gpio_num_t directionPin) {
    stepper = new AccelStepper(AccelStepper::DRIVER, pulsePin, directionPin);
    this->sharedData = sharedData;
    stepper->setSpeed(LOCK_SPEED);
    stepper->setMaxSpeed(LOCK_SPEED);
    stepper->setAcceleration(LOCK_SPEED * 5);
}

void LockController::tick() {
    stepper->run();
    if(sharedData->getState() == PREP_UNLOCK) {
        if(!sharedData->isLocked()) {
            sharedData->switchState(PREP_MOVING);
            return;
        }
        unlock();
        sharedData->switchState(UNLOCKING);
    } else if ( sharedData->getState() == PREP_LOCK) {
        if(sharedData->isLocked()) {
            sharedData->switchState(MachineState::IDLE);
            return;
        }
        lock();
        sharedData->switchState(LOCKING);
    } else if (sharedData->getState() == LOCKING) {
        if (!stepper->isRunning()) {
            sharedData->switchState(IDLE);
        }
    } else if (sharedData->getState() == UNLOCKING) {
        if (!stepper->isRunning()) {
            sharedData->switchState(PREP_MOVING);
        }
    }
}

void LockController::lock() {
    stepper->move(5 * PULSE_LOCK_PER_ROT);
    sharedData->setLocked(true);
}

void LockController::unlock() {
    stepper->move(-5 * PULSE_LOCK_PER_ROT);
    sharedData->setLocked(false);
}
