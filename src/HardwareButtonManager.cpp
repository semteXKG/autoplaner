#include <HardwareButtonManager.h>

HardwareButtonManager::HardwareButtonManager(gpio_num_t enterButtonPin, gpio_num_t bottomOutButtonPin, gpio_num_t speedButtonPin, SharedData* sharedData) {
    this->enterButtonPin = enterButtonPin;
    this->bottomOutButtonPin = bottomOutButtonPin;
    this->speedButtonPin = speedButtonPin;
    this->sharedData = sharedData;

    pinMode(enterButtonPin, INPUT_PULLUP);
    pinMode(speedButtonPin, INPUT_PULLUP);
    pinMode(bottomOutButtonPin, INPUT_PULLUP);
}

HardwareButtonManager::~HardwareButtonManager() {
}

void HardwareButtonManager::tick() {
    handleSpeedButton();
    handleEnterButton();
    handleBottomOutButton();
}

void HardwareButtonManager::handleSpeedButton() {
	int read = digitalRead(this->speedButtonPin);
    this->sharedData->speedButtonPressed = read == 0;
    monitorTriggerButton(&speedButtonAlreadyPressed, &this->sharedData->speedButtonPressed, &this->sharedData->speedButtonTriggered);
}

void HardwareButtonManager::handleEnterButton() {
	int read = digitalRead(this->enterButtonPin);
	this->sharedData->enterButtonPressed = read == 0;
    monitorTriggerButton(&enterAlreadyPressed, &this->sharedData->enterButtonPressed, &this->sharedData->enterButtonTriggered);
}

void HardwareButtonManager::handleBottomOutButton() {
    int read = digitalRead(this->bottomOutButtonPin) ;
    this->sharedData->bottomOutPressed = read;
}

void HardwareButtonManager::monitorTriggerButton(bool* monitorVar, bool* buttonPressed, bool* buttonTriggered) {
    if(*buttonPressed) {
        if (!*monitorVar) {
            *monitorVar = true;
            *buttonTriggered = true;
        } else {
            *buttonTriggered = false;            
        }
    } else {
        *monitorVar = false;
    }
}