#ifndef shareddata_h
#define shareddata_h
#include <InputManager.h>

class SharedData {
    private:
        double targetPosition;
        double currentPosition;
    public:
        enum MachineState {
            CALIBRATION_NEEDED, 
            POSITION_REACHED,
            INPUT_PROVIDED,
            MOVING
        };
        SharedData();
        ~SharedData();
        MachineState state;
        void setTargetPosition(double targetPosition);
        double getTargetPosition();
        void setCurrentPosition(double currentPosition);
        double getCurrentPosition();
};

#endif