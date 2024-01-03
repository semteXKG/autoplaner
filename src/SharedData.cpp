#include <SharedData.h>
#include <Arduino.h>

SharedData::SharedData() {
    backingData.currentPosition = MIN_POSITION;
    backingData.targetPosition = backingData.currentPosition;
    backingData.state = MachineState::CALIBRATION_NEEDED;
}

SharedData::~SharedData() {

}

long SharedData::getTargetPosition() {
    return backingData.targetPosition;
}

void SharedData::setTargetPosition(long targetPosition) {
    backingData.targetPosition = targetPosition;
    if(backingData.targetPosition < MIN_POSITION) {
		backingData.targetPosition = MIN_POSITION;
	} else if (backingData.targetPosition > MAX_POSITION) {
		backingData.targetPosition = MAX_POSITION;
	}
}

long SharedData::getCurrentPosition() {
    return backingData.currentPosition;
}

void SharedData::setCurrentPosition(long currentPosition) {
    backingData.currentPosition = currentPosition;
}

void SharedData::setPosition(long position) {
    setCurrentPosition(position);
    setTargetPosition(position);
}

void SharedData::setOffset(int offset) {
    backingData.offset = offset;
}

int SharedData::getOffset() {
    return backingData.offset;
}

long SharedData::getLastDistance() {
    return backingData.lastDistance;
}

void SharedData::setLastDistance(long lastDistance) {
    backingData.lastDistance = lastDistance;
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
    backingData.lastRotation = lastRotation;
}

int SharedData::getLastRotation() {
    return backingData.lastRotation;
}

bool SharedData::isLocked() {
    return backingData.locked;
}

void SharedData::setLocked(bool locked) {
    Serial.print("locked: ");
    Serial.println(locked);
    backingData.locked = locked;
    scheduleDisplayUpdate();
}

void SharedData::markCalibrationDone() {
    backingData.calibrationDone = true;
}

void SharedData::scheduleDisplayUpdate() {
    if(backingData.nextDisplayUpdate == -1) {
        backingData.nextDisplayUpdate = millis() + DISPLAY_UPDATE_DELAY;
    }
}

bool SharedData::shouldUpdateDisplay() {
    if (backingData.nextDisplayUpdate != -1 && millis() > backingData.nextDisplayUpdate) {
        backingData.nextDisplayUpdate = -1;
        return true;
    }
    return false;
}

MachineState SharedData::getState() {
    return backingData.state;
}

void SharedData::switchState(MachineState state) {
    if(state == IDLE && !backingData.calibrationDone) {
        Serial.print("Cannot go idle without calibration ");
        state = MachineState::CALIBRATION_NEEDED;
    }
    Serial.print("Switching state to ");
    Serial.print(state);
    Serial.print(" - ");
    Serial.println(machineStateDesc[state]);
    
    backingData.state = state;
    scheduleDisplayUpdate();
}

bool SharedData::evaluateFastmodeEnablement(long msSinceLast) {
    if (msSinceLast < INPUT_QUALIFIES_AS_FASTMODE) {
        rapidInputReceived++;
    } else {
        rapidInputReceived = 0;
    }
    return rapidInputReceived > 2;
}