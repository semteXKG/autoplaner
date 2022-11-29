#ifndef menu_handler_h
#define menu_handler_h

#include <SharedData.h>

class MenuHandler {
private:
    SharedData* sharedData;
    char *menu[3] = { "Offset Adjust", "Locking Status", "Lock Operation" };
    int currentlySelected = 0;
public:
    MenuHandler(SharedData* sharedData);
    ~MenuHandler();
    void handleMenuUpdate();
    void handleMenuEnter();
    void handleInputChange();
    void handleEnterPressed();
    void tick();
};

MenuHandler::MenuHandler(SharedData* sharedData) {
    this->sharedData = sharedData;
}

MenuHandler::~MenuHandler() {

}

void MenuHandler::handleMenuEnter() {
    if(sharedData->getState() == MachineState::IDLE 
        && sharedData->enterButton->isPressed() 
        && sharedData->enterButton->currentDuration() > LONG_PRESS_TIMEOUT_IN_MILLISECONDS) {
            sharedData->switchState(MachineState::SELECTION_MENU);
    }
}

void MenuHandler::handleMenuUpdate() {  
    if (sharedData->getState() == MachineState::SELECTION_MENU) {
        
        char* upper = currentlySelected - 1 >= 0 ? menu[currentlySelected-1] : NULL;
        char* mid = menu[currentlySelected];
        char* lower = currentlySelected + 1 < sizeof(menu)/sizeof(char*) ? menu[currentlySelected + 1] : NULL;
        sharedData->setMenuEntries(upper, mid, lower);
        sharedData->scheduleDisplayUpdate();
    }
}

void MenuHandler::handleInputChange() {
    if (sharedData->getState() == MachineState::SELECTION_MENU) {
        if(sharedData->getLastRotation() > 0 ) {
            currentlySelected = currentlySelected + (currentlySelected < sizeof(menu)/sizeof(char*) - 1 ? 1 : 0);
        } else if (sharedData->getLastRotation() < 0) {
            currentlySelected = currentlySelected - (currentlySelected == 0 ? 0 : 1);
        }        
    }
}

void MenuHandler::handleEnterPressed() {
    if(sharedData->getState() != MachineState::SELECTION_MENU) {
        return;
    }

    if (!sharedData->enterButton->pressed()) {
        return;
    }

    switch(currentlySelected) {
        case 0: 
            sharedData->switchState(MachineState::OFFSET_ADJUSTING);        
            break;
        case 1: break;
        case 2: break;
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