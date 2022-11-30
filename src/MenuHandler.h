#ifndef menu_handler_h
#define menu_handler_h

#include <SharedData.h>

class MenuHandler {
private:
    SharedData* sharedData;
    LockController* lockController;
    char *menu[4];
    int currentlySelected = 0;
public:
    MenuHandler(SharedData* sharedData, LockController* lockController);
    ~MenuHandler();
    void handleMenuUpdate();
    void handleMenuEnter();
    void handleInputChange();
    void handleEnterPressed();
    void tick();
};

MenuHandler::MenuHandler(SharedData* sharedData, LockController* lockController) {
    this->sharedData = sharedData;
    this->lockController = lockController;
    menu[0] = strdup("1. Offset Adjust");
    menu[1] = strdup("2. LCK Status");
    menu[2] = strdup("3. LCK Operation");
    menu[3] = strdup("4. Shutdown");
}

MenuHandler::~MenuHandler() {

}

void MenuHandler::handleMenuEnter() {
    if(sharedData->getState() == MachineState::IDLE 
        && sharedData->enterButton->isPressed() 
        && sharedData->enterButton->currentDuration() > LONG_PRESS_TIMEOUT_IN_MILLISECONDS) {
            sharedData->switchState(MachineState::SETTINGS_MENU);
    }
}

void MenuHandler::handleMenuUpdate() {  
    if (sharedData->getState() == MachineState::SETTINGS_MENU) {
        
        char* upper = currentlySelected - 1 >= 0 ? menu[currentlySelected-1] : NULL;
        char* mid = menu[currentlySelected];
        char* lower = currentlySelected + 1 < sizeof(menu)/sizeof(char*) ? menu[currentlySelected + 1] : NULL;
        sharedData->setMenuEntries(upper, mid, lower);
        sharedData->scheduleDisplayUpdate();
    }
}

void MenuHandler::handleInputChange() {
    if (sharedData->getState() == MachineState::SETTINGS_MENU) {
        if(sharedData->getLastRotation() > 0 ) {
            currentlySelected = currentlySelected + (currentlySelected < sizeof(menu)/sizeof(char*) - 1 ? 1 : 0);
        } else if (sharedData->getLastRotation() < 0) {
            currentlySelected = currentlySelected - (currentlySelected == 0 ? 0 : 1);
        }        
    }
}

void MenuHandler::handleEnterPressed() {
    if(sharedData->getState() != MachineState::SETTINGS_MENU) {
        return;
    }

    if (!sharedData->enterButton->pressed()) {
        return;
    }

    switch(currentlySelected) {
        case 0: 
            sharedData->switchState(MachineState::SETTINGS_OFFSET_ADJUSTING);        
            break;
        case 1: 
            sharedData->setLocked(!sharedData->isLocked());
            sharedData->switchState(MachineState::IDLE);        
            break;
        case 2: 
            if (sharedData->isLocked()) {
                lockController->unlock();
            } else {
                lockController->lock();
            }
            sharedData->switchState(MachineState::IDLE);        
            break;
        case 3: break;
        default: break;
    }
}

void MenuHandler::tick() {
    handleMenuEnter();
    handleInputChange();
    handleMenuUpdate();  
    handleEnterPressed();
}

#endif