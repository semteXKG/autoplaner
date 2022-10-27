#include <HardwareButtonManager.h>

HardwareButtonManager::HardwareButtonManager(gpio_num_t enterButtonPin, gpio_num_t bottomOutButtonPin, gpio_num_t speedButtonPin, SharedData* sharedData) {
    this->sharedData = sharedData;

    Bounce2::Button* enterButton = new Bounce2::Button();
    enterButton->setPressedState(LOW);
    enterButton->interval(5);
    enterButton->attach(enterButtonPin, INPUT_PULLUP);

    Bounce2::Button* speedButton = new Bounce2::Button();
    speedButton->setPressedState(LOW);
    enterButton->interval(5);
    speedButton->attach(speedButtonPin, INPUT_PULLUP);

    Bounce2::Button* bottomOutButton = new Bounce2::Button();
    enterButton->interval(5);
    bottomOutButton->attach(bottomOutButtonPin, INPUT_PULLUP);

    sharedData->bottomOut = bottomOutButton;
    sharedData->enterButton = enterButton;
    sharedData->speedButton = speedButton;
}

HardwareButtonManager::~HardwareButtonManager() {
}

void HardwareButtonManager::tick() {
    sharedData->speedButton->update();
    sharedData->enterButton->update();
    sharedData->bottomOut->update();
}
