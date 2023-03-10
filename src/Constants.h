static const long DENOMINATION = 1000000L;

static const long INPUT_QUALIFIES_AS_FASTMODE = 10;

static const long INCREMENT_NORMAL_IN_DENOM = 1 * DENOMINATION;
static const long INCREMENT_SLOW_IN_DENOM = DENOMINATION / 10;

static const long MAX_POSITION = 210 * DENOMINATION;
static const long MIN_POSITION = 8 * DENOMINATION;

static const int PULSE_PER_MM = 800;
static const int DENOM_PER_PULSE = 1250;
static const int PULSE_LOCK_PER_ROT = 1600;

static const int OP_SPEED = 8 * PULSE_PER_MM;
static const int CAL_SPEED = 0.5 * PULSE_PER_MM;

static const long CAL_POSITION_DENOM = 209300000;

static const int DEAD_ZONE_PULSE = PULSE_PER_MM * 0.05;
static const int OVERSHOOT_PULSE = PULSE_PER_MM;

static const int LOCK_SPEED = 6 * PULSE_LOCK_PER_ROT;

static const int ROTATION_ACCELERATION_TIMEOUT = 1000;
static const int ROTATION_ACCELERATION_ENGAGE_DELAY = 10;
static const int LONG_PRESS_TIMEOUT_IN_MILLISECONDS = 1000;

static const long IDLE_POSITION_IN_DENOM = 170 * DENOMINATION;

static const int DISPLAY_UPDATE_DELAY = 50;

static const int MAX_INPUTS = 5;
static const int IGNORE_ACCIDENTAL_INPUT_DELAY_MS = 250;

static const int TIMEOUT_MOVE_TO_CONVERSION = 500;

static const char* CAL_NEEDED_TEXT = "Calib needed";
static const char* CAL_RUNNING_TEXT = "Calibrating.";
static const char* MOVING_TEXT = "moving...";

static const char* LOCKING_TEXT = "locking";
static const char* UNLOCKING_TEXT = "unlocking";

#define locked_width 9
#define locked_height 12
static unsigned char locked_bits[] = {
   0x38, 0x00, 0x6c, 0x00, 0x44, 0x00, 0x44, 0x00, 0xfe, 0x00, 0xff, 0x01,
   0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01 };


#define unlocked_width 11
#define unlocked_height 12
static unsigned char unlocked_bits[] = {
   0x80, 0x03, 0xc0, 0x06, 0x40, 0x04, 0x40, 0x04, 0xfe, 0x00, 0xff, 0x01,
   0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01 };


 static const char *machineStateDesc[] =
{ "CALIBRATION_NEEDED", 
"PREP CALIBRATION",
"CALIBRATING", 
"IDLE", 
"SETTINGS_MENU", 
"SETTINGS_OFFSET_ADJUSTING", 
"SETTINGS_LOCKING_STATUS_ADJUST",
"SETTINGS_LOCK_OPERATION",
"SETTINGS_SHUTDOWN",
"PREP_LOCK", 
"PREP_UNLOCK", 
"LOCKING", 
"UNLOCKING", 
"PREP_MOVING", 
"MOVING_UP", 
"MOVING_DOWN_OVERSHOOT", 
"MOVING_DOWN_CORRECTION" }; 


enum MachineState {
    CALIBRATION_NEEDED,
    PREP_CALIBRATION, 
    CALIBRATING,
    IDLE,
    SETTINGS_MENU,
    SETTINGS_OFFSET_ADJUSTING,
    SETTINGS_LOCKING_STATUS_ADJUST,
    SETTINGS_LOCK_OPERATION,
    SETTINGS_SHUTDOWN,
    PREP_LOCK,
    PREP_UNLOCK,
    LOCKING,
    UNLOCKING,
    PREP_MOVING,
    MOVING_UP,
    MOVING_DOWN_OVERSHOOT,
    MOVING_DOWN_CORRECTION,
};