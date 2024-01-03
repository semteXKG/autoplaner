#ifndef shareddata_h
#define shareddata_h

#include <Constants.h>
#include <Bounce2.h>

struct BackingData
{
    long targetPosition;
    long currentPosition;
    int offset;
    long nextDisplayUpdate = -1;
    long lastDistance = 0;
    int lastRotation = 0;
    bool locked = false;
    bool calibrationDone = false;
    MachineState state;
};


class SharedData {
    private:
        BackingData backingData;
        char* menuEntries[3];
        int rapidInputReceived = 0;
    public:
        SharedData();
        ~SharedData();
        Bounce2::Button* enterButton;
        Bounce2::Button* speedButton;
        Bounce2::Button* bottomOut;
        Bounce2::Button* moveToConversion;
        void setTargetPosition(long targetPosition);
        long getTargetPosition();
        void setCurrentPosition(long currentPosition);
        long getCurrentPosition();
        void setPosition(long position);
        void setOffset(int offset);
        int getOffset();
        long getLastDistance();
        void setLastDistance(long lastDistance);
        void setMenuEntries(char* upper, char* mid, char* low);
        char** getMenuEntries();
        void setLastRotation(int rotation);
        int getLastRotation();
        bool isLocked();
        void setLocked(bool locked);
        void markCalibrationDone();
        void scheduleDisplayUpdate();
        bool shouldUpdateDisplay();
        void switchState(MachineState state);
        bool evaluateFastmodeEnablement(long msSinceLast);
        MachineState getState();
};

#endif