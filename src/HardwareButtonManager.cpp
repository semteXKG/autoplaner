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
    /*
    Serial.print("speed: ");
    Serial.println(this->sharedData->speedButtonPressed);
    */
}

void HardwareButtonManager::handleEnterButton() {
	int read = digitalRead(this->enterButtonPin);
	this->sharedData->enterButtonPressed = read == 0;
    /*
    Serial.print("enter: ");
    Serial.println(this->sharedData->enterButtonPressed);
    */
}

void HardwareButtonManager::handleBottomOutButton() {
    int read = digitalRead(this->bottomOutButtonPin) ;
    this->sharedData->bottomOutPressed = read;
    /*
    Serial.print("bottomOut: ");
    Serial.println(this->sharedData->bottomOutPressed);
    */
}