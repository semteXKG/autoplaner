#ifndef shareddata_h
#define shareddata_h

#include <Constants.h>

class SharedData {
    private:
        double targetPosition;
        double currentPosition;
        long nextDisplayUpdate = -1;
        MachineState state;
    public:
        SharedData();
        ~SharedData();
        bool enterButtonPressed, speedButtonPressed, bottomOutPressed;
        void setTargetPosition(double targetPosition);
        double getTargetPosition();
        void setCurrentPosition(double currentPosition);
        double getCurrentPosition();
        void setPosition(double position);
        void scheduleDisplayUpdate();
        bool shouldUpdateDisplay();
        void switchState(MachineState state);
        MachineState getState();
};

#endif