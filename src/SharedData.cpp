#include <SharedData.h>
#include <Arduino.h>

SharedData::SharedData() {
    this->currentPosition = MIN_POSITION;
    this->targetPosition = this->currentPosition;
    this->state = MachineState::CALIBRATION_NEEDED;
}

SharedData::~SharedData() {

}

double SharedData::getTargetPosition() {
    return this->targetPosition;
}

void SharedData::setTargetPosition(double targetPosition) {
    this->targetPosition = targetPosition;
    if(this->targetPosition < MIN_POSITION) {
		this->targetPosition = MIN_POSITION;
	} else if (this->targetPosition > MAX_POSITION) {
		this->targetPosition = MAX_POSITION;
	}
}

double SharedData::getCurrentPosition() {
    return this->currentPosition;
}

void SharedData::setCurrentPosition(double currentPosition) {
    this->currentPosition = currentPosition;
}

void SharedData::setPosition(double position) {
    setCurrentPosition(position);
    setTargetPosition(position);
}

void SharedData::setOffset(float offset) {
    this->offset = offset;
}

double SharedData::getOffset() {
    return offset;
}

double SharedData::getLastDistance() {
    return lastDistance;
}

void SharedData::setLastDistance(double lastDistance) {
    this->lastDistance = lastDistance;
}

void SharedData::setMenuEntries(char* upper, char* mid, char* low) {
    menuEntries[0] = upper;
    menuEntries[1] = mid;
    menuEntries[2] = low;
}

char** SharedData::getMenuEntries() {
    return menuEntries;
}

void SharedData::setLastRotation(int lastRotation) {
    this->lastRotation = lastRotation;
}

int SharedData::getLastRotation() {
    return lastRotation;
}

bool SharedData::isLocked() {
    return locked;
}

void SharedData::setLocked(bool locked) {
    Serial.print("locked: ");
    Serial.println(locked);
    this->locked = locked;
    scheduleDisplayUpdate();
}

void SharedData::markCalibrationDone() {
    this->calibrationDone = true;
}

void SharedData::scheduleDisplayUpdate() {
    if(this->nextDisplayUpdate == -1) {
        this->nextDisplayUpdate = millis() + DISPLAY_UPDATE_DELAY;
    }
}

bool SharedData::shouldUpdateDisplay() {
    if (this->nextDisplayUpdate != -1 && millis() > this->nextDisplayUpdate) {
        this->nextDisplayUpdate = -1;
        return true;
    }
    return false;
}

MachineState SharedData::getState() {
    return this->state;
}

void SharedData::switchState(MachineState state) {
    if(state == IDLE && !calibrationDone) {
        Serial.print("Cannot go idle without calibration ");
        state = MachineState::CALIBRATION_NEEDED;
    }
    Serial.print("Switching state to ");
    Serial.print(state);
    Serial.print(" - ");
    Serial.println(machineStateDesc[state]);
    
    this->state = state;
    scheduleDisplayUpdate();
}

bool SharedData::evaluateFastmodeEnablement(long msSinceLast) {
    if (msSinceLast < INPUT_QUALIFIES_AS_FASTMODE) {
        rapidInputReceived++;
    } else {
        rapidInputReceived = 0;
    }

    Serial.println(rapidInputReceived);
    return rapidInputReceived > 2;
}