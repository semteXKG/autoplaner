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

	double increment = sharedData->speedButtonPressed ? INCREMENT_FAST_IN_MM : INCREMENT_NORMAL_IN_MM;

	sharedData->setTargetPosition(sharedData->getTargetPosition() + (delta * increment));
	sharedData->scheduleDisplayUpdate();
	prevEncPosition = encoder->getPosition();

    Serial.println("Encoder count = " + String((int32_t)encoder->getPosition()));	
	Serial.println("Current Pos = " + String(sharedData->getCurrentPosition()));
	Serial.println("Target Position = " + String(sharedData->getTargetPosition()));	
}


void TargetSelector::handleInputSelectionButton() {
	if(sharedData->getState() == IDLE && sharedData->enterButtonPressed) {
		sharedData->switchState(MachineState::MOVING);
	}
}


void TargetSelector::tick() {
	handleEncoder();
	handleInputSelectionButton();
}
