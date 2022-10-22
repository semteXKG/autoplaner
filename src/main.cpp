#include <Arduino.h>
#include <U8g2lib.h>
#include <RotaryEncoder.h>
#include <AccelStepper.h>
#include <TargetSelector.h>
#include <Display.h>
#include <SharedData.h>
#include <EnvironmentSensors.h>
#include <HardwareButtonManager.h>
#include <Calibrator.h>


#define DEBUG ""

static const gpio_num_t SPEED_BUTTON = GPIO_NUM_12;
static const gpio_num_t GO_BUTTON = GPIO_NUM_14;
static const gpio_num_t BOTTOM_OUT_BUTTON = GPIO_NUM_27;

Display* display;
TargetSelector* inputManager;
SharedData* sharedData;
EnvironmentSensors* environmentSensors;
HardwareButtonManager* buttonManager;
Calibrator* calibrator;

void handleBottomOutButton() {
	int read = digitalRead(BOTTOM_OUT_BUTTON);
}

void setup() {
	Serial.begin(115200);
	sharedData = new SharedData();
	inputManager = new TargetSelector(18, 19, sharedData);
	buttonManager = new HardwareButtonManager(GO_BUTTON, BOTTOM_OUT_BUTTON, SPEED_BUTTON, sharedData);
	environmentSensors = new EnvironmentSensors(sharedData);
	calibrator = new Calibrator(sharedData);
}

void loop() {
	buttonManager->tick();
	environmentSensors->tick();
	inputManager->tick();
	environmentSensors->tick();
	calibrator->tick();

}