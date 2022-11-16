#include <LockController.h>

LockController::LockController(SharedData* sharedData, gpio_num_t pulsePin, gpio_num_t directionPin) {
    stepper = new AccelStepper(AccelStepper::DRIVER, pulsePin, directionPin);
    this->sharedData = sharedData;
    stepper->setSpeed(LOCK_SPEED);
    stepper->setMaxSpeed(LOCK_SPEED);
    stepper->setAcceleration(LOCK_SPEED * 5);
}

void LockController::tick() {
    if(sharedData->getState() == PREP_UNLOCK) {
        stepper->move(5 * PULSE_LOCK_PER_ROT);
        sharedData->switchState(UNLOCKING);
    } else if ( sharedData->getState() == PREP_LOCK) {
        stepper->move(-5 * PULSE_LOCK_PER_ROT);
        sharedData->switchState(LOCKING);
    } else if (sharedData->getState() == LOCKING) {
        stepper->run();
        if (!stepper->isRunning()) {
            sharedData->switchState(IDLE);
        }
    } else if (sharedData->getState() == UNLOCKING) {
        stepper->run();
        if (!stepper->isRunning()) {
            sharedData->switchState(PREP_MOVING);
        }
    }
}