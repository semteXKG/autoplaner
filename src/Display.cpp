#include <Display.h>
#include <SharedData.h>

Display::Display(SharedData* sharedData) {
    this->sharedData = sharedData;
    this->u8g2 = new U8G2_SH1106_128X64_NONAME_1_HW_I2C(U8G2_R0);
	u8g2->begin();
}

Display::~Display() {
}

void Display::printCenterText(const char* text, int size) {
	switch (size) {
	case 10:
		u8g2->setFont(u8g2_font_profont17_mf);
		break;
	case 24:
		u8g2->setFont(u8g2_font_inb24_mr);
		break;
	default:
		u8g2->setFont(u8g2_font_profont17_mf);
		break;
	}
    u8g2->drawHLine(0, 0, u8g2->getDisplayWidth());
    u8g2->drawHLine(0, u8g2->getDisplayHeight()-1 , u8g2->getDisplayWidth());
    u8g2_uint_t width = u8g2->getUTF8Width(text);

    u8g2->drawStr(
      u8g2->getDisplayWidth() / 2 - width / 2, 
      u8g2->getDisplayHeight() / 2 + size / 2, 
      text);
}

void Display::updatePositionReadings(bool blink) {
	bool shouldClear = (millis() / 1000) % 2;
	if (blink && shouldClear) {
		u8g2->clearDisplay();
		return;
	}

	u8g2->firstPage();
	do {
		char output[20];	
		int size;
		if((sharedData->getCurrentPosition() == sharedData->getTargetPosition())) {
			sprintf(&output[0], "%03.1f", sharedData->getCurrentPosition());
			size = 24;
		} else {
			sprintf(&output[0], "%03.1f (%03.1f)", sharedData->getTargetPosition(), 
                sharedData->getTargetPosition() - sharedData->getCurrentPosition());
			size = 10;
		}	
		printCenterText(output, size);
	} while ( u8g2->nextPage() );
}

void Display::updateCalibrationText() {
	u8g2->firstPage();
	do {
		printCenterText(CAL_NEEDED_TEXT, 10);
	} while (u8g2->nextPage());
}


void Display::updateCalibratingText() {
	u8g2->firstPage();
	do {
		printCenterText(CAL_RUNNING_TEXT, 10);
	} while (u8g2->nextPage());
}

void Display::updateMovingText() {
	u8g2->firstPage();
	do {
		printCenterText(MOVING_TEXT, 10);
	} while (u8g2->nextPage());
}

bool Display::updateBlinkState() {
	if (sharedData->getState() == OFFSET_ADJUSTING) {
		bool currentState = (millis() / 1000) % 2;
		bool stateChanged = currentState != lastBlinkState;
		lastBlinkState = currentState;
		return stateChanged;
	}
	return false;
}

void Display::tick() {
	if (this->sharedData->shouldUpdateDisplay() || updateBlinkState()) {
		switch(this->sharedData->getState()) {
			case MachineState::CALIBRATION_NEEDED: 
				updateCalibrationText();
				break;
			case MachineState::CALIBRATING: 
				updateCalibratingText();
				break;
			case MachineState::IDLE:
				updatePositionReadings(false);
				break;
			case MachineState::OFFSET_ADJUSTING:
				updatePositionReadings(true);
				break;
			case MachineState::PREP_MOVING:
			case MachineState::MOVING_DOWN_OVERSHOOT:
			case MachineState::MOVING_DOWN_CORRECTION:
			case MachineState::MOVING_UP:
				updateMovingText();
				break;
			default:
			break;
		}
	}
}

