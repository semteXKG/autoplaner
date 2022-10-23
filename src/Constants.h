static const double INCREMENT_NORMAL_IN_MM = 0.1;
static const double INCREMENT_FAST_IN_MM = 5;

static const double MAX_POSITION = 220;
static const double MIN_POSITION = 8;

enum MachineState {
    CALIBRATION_NEEDED, 
    CALIBRATING,
    IDLE,
    MOVING  
};