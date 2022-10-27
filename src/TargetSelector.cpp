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
	
	if(this->sharedData->getState() != MachineState::IDLE) {
		this->prevEncPosition = encoder->getPosition();
		return;
	}

	int32_t delta = encoder->getPosition() - prevEncPosition;	
	if(delta == 0) {
		return;
	}

	if(encoder->getRPM() > 100) {
		Serial.println("Frot unlocked with ");
		Serial.println(encoder->getRPM());
		fastRotationTill = millis() + ROTATION_ACCELERATION_TIMEOUT;
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

	sharedData->setTargetPosition(sharedData->getTargetPosition() + (delta * increment));
	sharedData->scheduleDisplayUpdate();
	prevEncPosition = encoder->getPosition();
}


void TargetSelector::handleInputSelectionButton() {
	if(sharedData->getState() == IDLE && sharedData->enterButton->fell()) { 
		if(sharedData->getTargetPosition() != sharedData->getCurrentPosition()) {
			lastDistance = sharedData->getTargetPosition() - sharedData->getCurrentPosition();
			sharedData->switchState(MachineState::PREP_MOVING);
		} else {
			sharedData->setTargetPosition(sharedData->getCurrentPosition() + lastDistance);
		}
		sharedData->scheduleDisplayUpdate();
	}
}


void TargetSelector::tick() {
	handleEncoder();
	handleInputSelectionButton();
}
