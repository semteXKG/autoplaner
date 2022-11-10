#include <TargetSelector.h>

TargetSelector::TargetSelector(int pin1, int pin2, SharedData* sharedData) {
    this->encoder = new RotaryEncoder(pin1, pin2);
    this->sharedData = sharedData;
	this->prevEncPosition = 0;
}

TargetSelector::~TargetSelector() {
    // destructor
}

void TargetSelector::handleEncoder() {
    encoder->tick();
	// Loop and read the count
	
	if(this->sharedData->getState() != MachineState::IDLE && this->sharedData->getState() != MachineState::OFFSET_ADJUSTING) {
		this->prevEncPosition = encoder->getPosition();
		return;
	}

	int32_t delta = encoder->getPosition() - prevEncPosition;	
	if(delta == 0) {
		return;
	}

	if(encoder->getRPM() > 100) {
		Serial.println("fast rotations rpm reached");
		Serial.println(encoder->getRPM());
		if(fastRotationSince == 0) {
			fastRotationSince = millis();
		} 
		if(fastRotationSince < millis() - ROTATION_ACCELERATION_ENGAGE_DELAY) {
			Serial.println("fast rotation enabled ");
			fastRotationTill = millis() + ROTATION_ACCELERATION_TIMEOUT;
		}	
	} else {
		fastRotationSince = 0;
	}

	double increment;
	if (sharedData->speedButton->isPressed()) {
		increment =  INCREMENT_FAST_IN_MM;
	} else {
		if(fastRotationTill > millis()) {
			increment = INCREMENT_FROT_IN_MM;
		} else {
			increment = INCREMENT_NORMAL_IN_MM;
		}
	}

	lastValues[currentInputPosition % MAX_INPUTS] = delta * increment;
	lastTimestamps[currentInputPosition % MAX_INPUTS] = millis();
	currentInputPosition++;
	
	sharedData->setTargetPosition(sharedData->getTargetPosition() + (delta * increment));
	sharedData->scheduleDisplayUpdate();
	prevEncPosition = encoder->getPosition();
}


void TargetSelector::handleInputSelectionButton() {
	if (sharedData->enterButton->rose()) {
		if (sharedData->getState() == IDLE) {
			Serial.print("PrevDur: ");
			Serial.println(sharedData->enterButton->previousDuration());
			if (sharedData->enterButton->previousDuration() > LONG_PRESS_TIMEOUT_IN_MILLISECONDS) {
				sharedData->switchState(MachineState::OFFSET_ADJUSTING);
			} else {
				correctAccidentalInputs();
				if (sharedData->getTargetPosition() != sharedData->getCurrentPosition()) {
					lastDistance = sharedData->getTargetPosition() - sharedData->getCurrentPosition();
					sharedData->switchState(MachineState::PREP_MOVING);
				} else {
					sharedData->setTargetPosition(sharedData->getCurrentPosition() + lastDistance);
				}
			}
			sharedData->scheduleDisplayUpdate();	
		} else if (sharedData->getState() == MachineState::OFFSET_ADJUSTING) {
			sharedData->setCurrentPosition(sharedData->getTargetPosition());
			sharedData->switchState(MachineState::IDLE);
		}
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
		sharedData->setTargetPosition(170.0);
		sharedData->switchState(PREP_MOVING);
	}
}

void TargetSelector::tick() {
	handleEncoder();
	handleInputSelectionButton();
	handleMoveToConversionButton();
}
