#include <LockController.h>
#include <ArduinoNvs.h>

static const char* KEY_LOCKSTATE = "lockState";

LockController::LockController(SharedData* sharedData, gpio_num_t pulsePin, gpio_num_t directionPin) {
    stepper = new AccelStepper(AccelStepper::DRIVER, pulsePin, directionPin);
    this->sharedData = sharedData;

    Serial.print("Recovered LockState: ");
    Serial.println(NVS.getInt(KEY_LOCKSTATE));
    sharedData->setLocked(NVS.getInt(KEY_LOCKSTATE));

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
        unlock(false);
        sharedData->switchState(UNLOCKING);
    } else if ( sharedData->getState() == PREP_LOCK) {
        if(sharedData->isLocked()) {
            sharedData->switchState(MachineState::IDLE);
            return;
        }
        lock(false);
        sharedData->switchState(LOCKING);
    } else if (sharedData->getState() == LOCKING) {
        if (!stepper->isRunning()) {
            sharedData->switchState(IDLE);
        }
    } else if (sharedData->getState() == UNLOCKING) {
        if (!stepper->isRunning()) {
            sharedData->switchState(PREP_MOVING);
        }
    } else if (sharedData->getState() == MachineState::PREP_CALIBRATION) {
        if(!stepper->isRunning()) {
            sharedData->switchState(CALIBRATING);
        }
    }
}

void LockController::lock(bool stateOnly) {
    if (sharedData->isLocked()) {
        return;
    }

    if (!stateOnly) {
        stepper->move(PULSE_LOCK_PER_ROT);
    }
    sharedData->setLocked(true);
    NVS.setInt(KEY_LOCKSTATE, 1);
    NVS.commit();
}

void LockController::unlock(bool stateOnly) {
    if (!sharedData->isLocked()) {
        return; 
    }
    
    if (!stateOnly) {
        stepper->move(PULSE_LOCK_PER_ROT);
    }
    sharedData->setLocked(false);
    NVS.setInt(KEY_LOCKSTATE, 0);
    NVS.commit();
}
