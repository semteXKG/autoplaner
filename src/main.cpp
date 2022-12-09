#include <Arduino.h>
#include <U8g2lib.h>
#include <RotaryEncoder.h>
#include <TargetSelector.h>
#include <Display.h>
#include <SharedData.h>
#include <EnvironmentSensors.h>
#include <HardwareButtonManager.h>
#include <Calibrator.h>
#include <StepperController.h>
#include <LockController.h>
#include <CalibrationOffsetHandler.h>
#include <MenuHandler.h>
#include <ArduinoNvs.h>
#include <FastAccelStepper.h>

#define DEBUG ""

static const gpio_num_t SPEED_BUTTON = GPIO_NUM_12;
static const gpio_num_t GO_BUTTON = GPIO_NUM_14;
static const gpio_num_t BOTTOM_OUT_BUTTON = GPIO_NUM_27;
static const gpio_num_t MOVE_TO_CONVERSION_BUTTON = GPIO_NUM_16;

static const gpio_num_t HEIGHT_STEPPER_PULSE = GPIO_NUM_33;
static const gpio_num_t HEIGHT_STEPPER_DIR = GPIO_NUM_32;

static const gpio_num_t LOCK_STEPPER_PULSE = GPIO_NUM_15;
static const gpio_num_t LOCK_STEPPER_DIR = GPIO_NUM_2;


Display* display;
TargetSelector* inputManager;
SharedData* sharedData;
EnvironmentSensors* environmentSensors;
HardwareButtonManager* buttonManager;
Calibrator* calibrator;
StepperController* stepperController;
LockController* lockController;
CalibrationOffsetHandler* calibrationOffsetHandler;
MenuHandler* menuHandler;
FastAccelStepperEngine* engine;

void setup() {
	Serial.begin(115200);
	
	if (!NVS.begin("planer")) {
		ESP.restart();
	}

	engine = new FastAccelStepperEngine();
	engine->init();
	sharedData = new SharedData();
	inputManager = new TargetSelector(18, 19, sharedData);
	buttonManager = new HardwareButtonManager(GO_BUTTON, BOTTOM_OUT_BUTTON, SPEED_BUTTON, MOVE_TO_CONVERSION_BUTTON, sharedData);
	environmentSensors = new EnvironmentSensors(sharedData);
	display = new Display(sharedData);
	stepperController = new StepperController(sharedData, engine, HEIGHT_STEPPER_PULSE, HEIGHT_STEPPER_DIR);
	lockController = new LockController(sharedData, engine, LOCK_STEPPER_PULSE, LOCK_STEPPER_DIR);
	calibrator = new Calibrator(sharedData, lockController);
	calibrationOffsetHandler = new CalibrationOffsetHandler(sharedData);
	menuHandler = new MenuHandler(sharedData, lockController);
	sharedData->scheduleDisplayUpdate();
	sharedData->switchState(MachineState::CALIBRATION_NEEDED);
}

void loop() {
	buttonManager->tick();
	environmentSensors->tick();
	inputManager->tick();
	environmentSensors->tick();
	calibrator->tick();
	display->tick();
	stepperController->tick();
	lockController->tick();
	calibrationOffsetHandler->tick();
	menuHandler->tick();
}