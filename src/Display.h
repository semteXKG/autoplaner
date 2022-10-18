#ifndef display_h
#define display_h
#include <U8g2lib.h>
#include "SharedData.h"

class Display {
private:
    U8G2_SH1106_128X64_NONAME_1_HW_I2C* u8g2;
    SharedData* sharedData;
    void updatePositionReadings();
    void updateCalibrationText();
    void printCenterText(char* text, int size = 24);
public:
    Display(SharedData* sharedData);
    ~Display();
    void tick();
};

#endif