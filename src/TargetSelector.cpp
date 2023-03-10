#include <TargetSelector.h>

TargetSelector::TargetSelector(int pin1, int pin2, SharedData* sharedData) {
    this->encoder = new RotaryEncoder(pin1, pin2, RotaryEncoder::LatchMode::TWO03);
    this->sharedData = sharedData;
	this->prevEncPosition = 0;
}

TargetSelector::~TargetSelector() {
    // destructor
}

void TargetSelector::handleEncoder() {
	sharedData->setLastRotation(0);
    encoder->tick();
	// Loop and read the count
	int newPos = encoder->getPosition();


 	unsigned long ms = encoder->getMillisBetweenRotations();

	if(encoder->getPosition() == prevEncPosition) {
		return;
	}

	bool fastMode = sharedData->evaluateFastmodeEnablement(ms);

	int32_t delta = encoder->getPosition() - prevEncPosition;	
	sharedData->setLastRotation(delta);
	prevEncPosition = encoder->getPosition();	

	if(this->sharedData->getState() != MachineState::IDLE && this->sharedData->getState() != MachineState::SETTINGS_OFFSET_ADJUSTING) {
		return;
	}

	long increment;
	if (sharedData->speedButton->isPressed()) {
		increment =  INCREMENT_SLOW_IN_DENOM;
	} else if (fastMode) {
		increment = INCREMENT_NORMAL_IN_DENOM * 4;
	} else {
		increment = INCREMENT_NORMAL_IN_DENOM;
	}

	lastValues[currentInputPosition % MAX_INPUTS] = delta * increment;
	lastTimestamps[currentInputPosition % MAX_INPUTS] = millis();
	currentInputPosition++;

	if(sharedData->getState() == MachineState::IDLE) {
		sharedData->setTargetPosition(sharedData->getTargetPosition() + (delta * increment));
	} else if (sharedData->getState() == MachineState::SETTINGS_OFFSET_ADJUSTING) {
		sharedData->setOffset(sharedData->getOffset() + delta * increment);
	}

	sharedData->scheduleDisplayUpdate();
}


void TargetSelector::handleInputSelectionButton() {
	if (sharedData->getState() == IDLE) {
		if (sharedData->enterButton->rose() && hasCorrectInputSource) {
			correctAccidentalInputs();
			if (sharedData->getTargetPosition() != sharedData->getCurrentPosition()) {
				sharedData->setLastDistance(sharedData->getTargetPosition() - sharedData->getCurrentPosition());
				sharedData->switchState(MachineState::PREP_UNLOCK);
			} else {
				sharedData->setTargetPosition(sharedData->getCurrentPosition() + sharedData->getLastDistance());
			}
			sharedData->scheduleDisplayUpdate();	
		}
		if(sharedData->enterButton->fell()) {
			hasCorrectInputSource = true;
		}
	} else {
		hasCorrectInputSource = false;
	}
	
}

void TargetSelector::correctAccidentalInputs() {
	double correction = 0;
	long currentTimestamp = millis();
	for (int i = currentInputPosition - 1; i >= currentInputPosition - MAX_INPUTS; i--) {
		Serial.print("CHecking index: ");
		Serial.println(i);
		if (currentTimestamp - IGNORE_ACCIDENTAL_INPUT_DELAY_MS < lastTimestamps[i % MAX_INPUTS]) {
			Serial.print("removed accidental input " );
			Serial.println(lastValues[i % MAX_INPUTS]);
			correction += lastValues[i % MAX_INPUTS] * -1;
		} else {
			break;
		}
	}
	sharedData->setTargetPosition(sharedData->getTargetPosition() + correction);
}

void TargetSelector::handleMoveToConversionButton() {
	if (sharedData->getState() == IDLE && sharedData->moveToConversion->isPressed() && sharedData->moveToConversion->currentDuration() > TIMEOUT_MOVE_TO_CONVERSION) {
		sharedData->setTargetPosition(IDLE_POSITION_IN_DENOM);
		sharedData->switchState(PREP_UNLOCK);
	}
}

void TargetSelector::tick() {
	handleEncoder();
	handleInputSelectionButton();
	handleMoveToConversionButton();
}
