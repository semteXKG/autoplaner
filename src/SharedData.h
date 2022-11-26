#ifndef shareddata_h
#define shareddata_h

#include <Constants.h>
#include <Bounce2.h>

class SharedData {
    private:
        double targetPosition;
        double currentPosition;
        double offset;
        long nextDisplayUpdate = -1;
        double lastDistance = 0;
        MachineState state;
    public:
        SharedData();
        ~SharedData();
        Bounce2::Button* enterButton;
        Bounce2::Button* speedButton;
        Bounce2::Button* bottomOut;
        Bounce2::Button* moveToConversion;
        void setTargetPosition(double targetPosition);
        double getTargetPosition();
        void setCurrentPosition(double currentPosition);
        double getCurrentPosition();
        void setPosition(double position);
        void setOffset(float offset);
        double getOffset();
        double getLastDistance();
        void setLastDistance(double lastDistance);
        void scheduleDisplayUpdate();
        bool shouldUpdateDisplay();
        void switchState(MachineState state);
        MachineState getState();
};

#endif