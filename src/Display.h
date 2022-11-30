#ifndef display_h
#define display_h
#include <U8g2lib.h>
#include "SharedData.h"

class Display {
private:
    U8G2_SH1106_128X64_NONAME_F_HW_I2C* u8g2;
    SharedData* sharedData;
    bool lastBlinkState = false;
    void updatePositionReadings(bool blink);
    void updateOffsetReadings();
    void updateCalibrationText();
    void updateCalibratingText();
    void updateLockingText();
    void updateUnlockingText();
    void updateMovingText();
    void printBorder();
    void printPrevious();
    void printLockState();
    void printSelectionMenu();
    boolean updateBlinkState();
    void printCenterText(const char* text, int size = 24);
public:
    Display(SharedData* sharedData);
    ~Display();
    void tick();
};

#endif