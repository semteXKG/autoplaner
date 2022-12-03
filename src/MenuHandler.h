#ifndef menu_handler_h
#define menu_handler_h

#include <SharedData.h>
#include <LockController.h>

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

#endif