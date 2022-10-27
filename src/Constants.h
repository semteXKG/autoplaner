static const double INCREMENT_NORMAL_IN_MM = 0.1;
static const double INCREMENT_FROT_IN_MM = 0.5;
static const double INCREMENT_FAST_IN_MM = 5;

static const double MAX_POSITION = 220;
static const double MIN_POSITION = 8;

static const long PULSE_PER_MM = 400;

static const int ROTATION_ACCELERATION_TIMEOUT = 500;

static const int DISPLAY_UPDATE_DELAY = 50;

enum MachineState {
    CALIBRATION_NEEDED, 
    CALIBRATING,
    IDLE,
    PREP_MOVING,
    MOVING_UP,
    MOVING_DOWN_OVERSHOOT,
    MOVING_DOWN_CORRECTION,
};