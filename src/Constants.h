static const double INCREMENT_NORMAL_IN_MM = 0.1;
static const double INCREMENT_FROT_IN_MM = 0.2;
static const double INCREMENT_FAST_IN_MM = 5;

static const double MAX_POSITION = 210;
static const double MIN_POSITION = 8;

static const int PULSE_PER_MM = 800;

static const int OP_SPEED = 8 * PULSE_PER_MM;
static const int CAL_SPEED = 0.5 * PULSE_PER_MM;

static const double CAL_POSITION_MM = 209.3;

static const int DEAD_ZONE = PULSE_PER_MM * 0.05;
static const int OVERSHOOT = PULSE_PER_MM;

static const int ROTATION_ACCELERATION_TIMEOUT = 1000;
static const int ROTATION_ACCELERATION_ENGAGE_DELAY = 10;
static const int LONG_PRESS_TIMEOUT_IN_MILLISECONDS = 1000;

static const int DISPLAY_UPDATE_DELAY = 50;

static const int MAX_INPUTS = 5;
static const int IGNORE_ACCIDENTAL_INPUT_DELAY_MS = 250;

static const int TIMEOUT_MOVE_TO_CONVERSION = 500;

static const char* CAL_NEEDED_TEXT = "Cal needed";
static const char* CAL_RUNNING_TEXT = "Cal running...";
static const char* MOVING_TEXT = "moving...";

enum MachineState {
    CALIBRATION_NEEDED, 
    CALIBRATING,
    IDLE,
    OFFSET_ADJUSTING,
    PREP_MOVING,
    MOVING_UP,
    MOVING_DOWN_OVERSHOOT,
    MOVING_DOWN_CORRECTION,
};