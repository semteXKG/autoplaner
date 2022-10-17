#include <Arduino.h>
#include <U8g2lib.h>
#include <RotaryEncoder.h>
#include <AccelStepper.h>
#include <InputManager.h>
#include <Display.h>
#include <SharedData.h>

#define DEBUG ""

static const gpio_num_t SPEED_BUTTON = GPIO_NUM_12;
static const gpio_num_t GO_BUTTON = GPIO_NUM_14;
static const gpio_num_t BOTTOM_OUT_BUTTON = GPIO_NUM_27;


bool speedButtonPressed = false;
bool goButtonPressed = false;


void handleSpeedButtonPressed() {
	Serial.println("Speed button pressed");
//	increment = INCREMENT_FAST_IN_MM;
	speedButtonPressed = true;
}

void handleSpeedButtonDepressed() {
	Serial.println("Speed button de-pressed");
//	increment = INCREMENT_NORMAL_IN_MM;
	speedButtonPressed = false;
}

void handleSpeedButton() {
	int read = digitalRead(SPEED_BUTTON);
	if (read == 0 && !speedButtonPressed) {
		handleSpeedButtonPressed();
	} 
	if(read == 1 && speedButtonPressed) {
		handleSpeedButtonDepressed();
	}
}

void handleGoButton() {
	int read = digitalRead(GO_BUTTON);
	if(read == 0 && !goButtonPressed) {
//		current_position = target_position;
//		updateNeeded = millis() + UPDATE_BACKOFF;
		goButtonPressed = true;
	}
	if(read == 1) {
		goButtonPressed = false;
	}
}

void handleBottomOutButton() {
	int read = digitalRead(BOTTOM_OUT_BUTTON);
}

void setup() {
	Serial.begin(115200);
	pinMode(SPEED_BUTTON, INPUT_PULLUP);
	pinMode(GO_BUTTON, INPUT_PULLUP);
	pinMode(BOTTOM_OUT_BUTTON, INPUT_PULLDOWN);  
}

void loop() {
	handleSpeedButton();
	handleGoButton();
	handleBottomOutButton();
}