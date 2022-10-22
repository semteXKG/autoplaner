#include <TargetSelector.h>

TargetSelector::TargetSelector(int pin1, int pin2, SharedData* sharedData) {
    this->encoder = new RotaryEncoder(pin1, pin2);
    this->sharedData = sharedData;
}

TargetSelector::~TargetSelector() {
    // destructor
}

void TargetSelector::tick() {
    encoder->tick();
	// Loop and read the count
	if(encoder->getPosition() != prevEncPosition) {
		double increment = sharedData->speedButtonPressed ? INCREMENT_FAST_IN_MM : INCREMENT_NORMAL_IN_MM;
		int32_t delta = encoder->getPosition() - prevEncPosition;
		
		sharedData->setTargetPosition(sharedData->getTargetPosition() + delta * increment);
		
		prevEncPosition = encoder->getPosition();

        Serial.println("Encoder count = " + String((int32_t)encoder->getPosition()));	
		Serial.println("Current Pos = " + String(sharedData->getCurrentPosition()));
		Serial.println("Target Position = " + String(sharedData->getTargetPosition()));	
    }
}