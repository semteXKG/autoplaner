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
        int lastRotation = 0;
        char* menuEntries[3];
        bool locked = false;
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
        void setMenuEntries(char* upper, char* mid, char* low);
        char** getMenuEntries();
        void setLastRotation(int rotation);
        int getLastRotation();
        bool isLocked();
        void setLocked(bool locked);
        void scheduleDisplayUpdate();
        bool shouldUpdateDisplay();
        void switchState(MachineState state);
        MachineState getState();
};

#endif