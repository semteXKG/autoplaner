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

void handleBottomOutButton() {
	int read = digitalRead(BOTTOM_OUT_BUTTON);
}

void setup() {
	Serial.begin(115200);
	pinMode(BOTTOM_OUT_BUTTON, INPUT_PULLDOWN);  
}

void loop() {
	handleBottomOutButton();
}