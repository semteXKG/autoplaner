#include <InputManager.h>

InputManager::InputManager(int pin1, int pin2, gpio_num_t speedButtonPin, gpio_num_t enterButtonPin) {
    this->encoder = new RotaryEncoder(pin1, pin2);
    this->sharedData = sharedData;
    this->speedButtonPin = speedButtonPin;
    this->enterButtonPin = enterButtonPin;
    
    pinMode(speedButtonPin, INPUT_PULLUP);
   	pinMode(enterButtonPin, INPUT_PULLUP);

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

void InputManager::handleSpeedButtonPressed() {
	Serial.println("Speed button pressed");
    this->increment = INCREMENT_FAST_IN_MM;
	this->speedButtonPressed = true;
}

void InputManager::handleSpeedButtonDepressed() {
	Serial.println("Speed button de-pressed");
	this->increment = INCREMENT_NORMAL_IN_MM;
	this->speedButtonPressed = false;
}

void InputManager::handleSpeedButton() {
	int read = digitalRead(this->speedButtonPin);
	if (read == 0 && !this->speedButtonPressed) {
		handleSpeedButtonPressed();
	} 
	if(read == 1 && this->speedButtonPressed) {
		handleSpeedButtonDepressed();
	}
}

void InputManager::handleEnterButton() {
	int read = digitalRead(this->enterButtonPin);
	if(read == 0 && !this->enterButtonPressed) {
        this->sharedData->setCurrentPosition(this->sharedData->getTargetPosition());
		this->sharedData->scheduleDisplayUpdate();
        this->enterButtonPressed = true;
	}
	if(read == 1) {
		this->enterButtonPressed = false;
	}
}