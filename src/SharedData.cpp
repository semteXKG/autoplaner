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
    Serial.print("Switching state to ");
    Serial.println(machineStateDesc[state]);
    this->state = state;
    scheduleDisplayUpdate();
}