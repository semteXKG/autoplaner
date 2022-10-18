#ifndef shareddata_h
#define shareddata_h
#include <InputManager.h>

static const int DISPLAY_UPDATE_DELAY = 50;

class SharedData {
    private:
        double targetPosition;
        double currentPosition;
        long nextDisplayUpdate = -1;
    public:
        enum MachineState {
            CALIBRATION_NEEDED, 
            CALIBRATING,
            IDLE,
            MOVING,
            BOTTOM_OUT,
        };
        SharedData();
        ~SharedData();
        MachineState state;
        bool enterButtonPressed;
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