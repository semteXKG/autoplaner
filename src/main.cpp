#include <Arduino.h>
#include <U8g2lib.h>
#include <RotaryEncoder.h>

#define DEBUG ""

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0);
RotaryEncoder encoder(18, 19, RotaryEncoder::LatchMode::FOUR0);

static const gpio_num_t SPEED_BUTTON = GPIO_NUM_12;
static const gpio_num_t GO_BUTTON = GPIO_NUM_14;
static const gpio_num_t BOTTOM_OUT_BUTTON = GPIO_NUM_27;

static const long UPDATE_BACKOFF = 50;
static const double INCREMENT_NORMAL_IN_MM = 0.1;
static const double INCREMENT_FAST_IN_MM = 5;

static const double MAX_POSITION = 220;
static const double MIN_POSITION = 8;

volatile int32_t encoder_previous = 0;
volatile double current_position = MIN_POSITION;
volatile double target_position = current_position;
double increment = INCREMENT_NORMAL_IN_MM; 

long updateNeeded = 0;
long lastInputAt = -1;


bool speedButtonPressed = false;
bool goButtonPressed = false;

void  printCenterText(char* text, int size = 24) {
	switch (size) {
	case 10:
		u8g2.setFont(u8g2_font_profont17_mf);

		break;
	case 24:
		u8g2.setFont(u8g2_font_inb24_mr);
		break;
	default:
		u8g2.setFont(u8g2_font_profont17_mf);
		break;
	}
    u8g2.drawHLine(0, 0, u8g2.getDisplayWidth());
    u8g2.drawHLine(0, u8g2.getDisplayHeight()-1 , u8g2.getDisplayWidth());
    u8g2_uint_t width = u8g2.getUTF8Width(text);

    u8g2.drawStr(
      u8g2.getDisplayWidth() / 2 - width / 2, 
      u8g2.getDisplayHeight() / 2 + size / 2, 
      text);
}

void updateText() {
	u8g2.firstPage();
	do {
		char output[20];	
		int size;
		if(current_position == target_position) {
			sprintf(&output[0], "%03.1f", current_position);
			size = 24;
		} else {
			sprintf(&output[0], "%03.1f (%03.1f)", target_position, target_position - current_position);
			size = 10;
		}	
		printCenterText(output, size);
	} while ( u8g2.nextPage() );
}

void handleSpeedButtonPressed() {
	Serial.println("Speed button pressed");
	increment = INCREMENT_FAST_IN_MM;
	speedButtonPressed = true;
}

void handleSpeedButtonDepressed() {
	Serial.println("Speed button de-pressed");
	increment = INCREMENT_NORMAL_IN_MM;
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
		current_position = target_position;
		updateNeeded = millis() + UPDATE_BACKOFF;
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
	u8g2.begin();  
}

void loop() {
	encoder.tick();
	// Loop and read the count
	if(encoder.getPosition() != encoder_previous) {
		int32_t delta = encoder.getPosition() - encoder_previous;
		
		target_position = target_position + delta * increment;
		if(target_position < MIN_POSITION) {
			target_position = MIN_POSITION;
		} else if (target_position > MAX_POSITION) {
			target_position = MAX_POSITION;
		}

		encoder_previous = encoder.getPosition();
		#ifdef DEBUG
		Serial.println("Encoder count = " + String((int32_t)encoder.getPosition()));	
		Serial.println("Current Pos = " + String(current_position));
		Serial.println("Target Position = " + String(target_position));	
		#endif
		if(updateNeeded == -1) {
			updateNeeded = millis() + UPDATE_BACKOFF;
		}
	}

	if (updateNeeded != -1 && updateNeeded < millis()) {
		updateText();
		updateNeeded = -1;
	}

	handleSpeedButton();
	handleGoButton();
	handleBottomOutButton();
}