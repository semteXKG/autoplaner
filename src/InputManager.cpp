#include <InputManager.h>

InputManager::InputManager(int pin1, int pin2) {
    this->encoder = new RotaryEncoder(pin1, pin2);
    this->sharedData = sharedData;
    increment = INCREMENT_NORMAL_IN_MM;
}

InputManager::~InputManager() {
    // destructor
}

void InputManager::tick() {
    encoder->tick();
	// Loop and read the count
	if(encoder->getPosition() != prevEncPosition) {
		int32_t delta = encoder->getPosition() - prevEncPosition;
		
		sharedData->setTargetPosition(sharedData->getTargetPosition() + delta * increment);
		
		prevEncPosition = encoder->getPosition();

        Serial.println("Encoder count = " + String((int32_t)encoder->getPosition()));	
		Serial.println("Current Pos = " + String(sharedData->getCurrentPosition()));
		Serial.println("Target Position = " + String(sharedData->getTargetPosition()));	
    }
}