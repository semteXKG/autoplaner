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

void SharedData::scheduleDisplayUpdate() {
    this->nextDisplayUpdate = millis() + DISPLAY_UPDATE_DELAY;
}

bool SharedData::shouldUpdateDisplay() {
    if (millis() > this->nextDisplayUpdate) {
        this->nextDisplayUpdate = -1;
        return true;
    }
    return false;
}

MachineState SharedData::getState() {
    return this->state;
}

void SharedData::switchState(MachineState state) {
    this->state = state;
}