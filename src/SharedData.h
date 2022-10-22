#ifndef shareddata_h
#define shareddata_h

#include <Constants.h>

static const int DISPLAY_UPDATE_DELAY = 50;

class SharedData {
    private:
        double targetPosition;
        double currentPosition;
        long nextDisplayUpdate = -1;
    public:
        SharedData();
        ~SharedData();
        MachineState state;
        bool enterButtonPressed, speedButtonPressed, bottomOutPressed;
        void setTargetPosition(double targetPosition);
        double getTargetPosition();
        void setCurrentPosition(double currentPosition);
        double getCurrentPosition();
        void scheduleDisplayUpdate();
        bool shouldUpdateDisplay();
        void switchState(MachineState state);
        MachineState getState();
};

#endif