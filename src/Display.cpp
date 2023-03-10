#include <Display.h>
#include <SharedData.h>
#include <Wire.h>

Display::Display(SharedData* sharedData) {
    this->sharedData = sharedData;
	this->u8g2 = new U8G2_SH1106_128X64_NONAME_F_HW_I2C(U8G2_R0);
	u8g2->setBusClock(100000);
	u8g2->begin();
	Serial.print("clock: ");
	Serial.println(Wire.getClock());
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
    u8g2_uint_t width = u8g2->getUTF8Width(text);

    u8g2->drawStr(
      u8g2->getDisplayWidth() / 2 - width / 2, 
      u8g2->getDisplayHeight() / 2 + size / 2, 
      text);
}

void Display::updatePositionReadings(bool blink) {
	bool shouldClear = (millis() / 250) % 4;
	if (blink && shouldClear) {
		u8g2->clearDisplay();
		return;
	}

	char output[20];	
	int size;
	if((sharedData->getCurrentPosition() == sharedData->getTargetPosition())) {
		sprintf(&output[0], "%03.1f", sharedData->getCurrentPosition() / (double)DENOMINATION);
		size = 24;
	} else {
		sprintf(&output[0], "%03.1f (%03.1f)", sharedData->getTargetPosition() / (double)DENOMINATION, (sharedData->getTargetPosition() - sharedData->getCurrentPosition()) / (double)DENOMINATION);
		size = 10;
	}	
	printCenterText(output, size);
}

void Display::updateOffsetReadings() {
	u8g2->setFont(u8g2_font_profont11_tr);
	u8g2->drawStr(4, u8g2->getMaxCharHeight() + 1, "Offset Adjust");
	char output[20];	
	sprintf(&output[0], "%03.1f", sharedData->getOffset() / (double)DENOMINATION);
	printCenterText(output, 24);
}

void Display::updateCalibrationText() {
	printCenterText(CAL_NEEDED_TEXT, 10);
}


void Display::updateLockingText() {
	printCenterText(LOCKING_TEXT, 10);
}

void Display::updateUnlockingText() {
	printCenterText(UNLOCKING_TEXT, 10);
}

void Display::updateCalibratingText() {
	printCenterText(CAL_RUNNING_TEXT, 10);
}

void Display::updateMovingText() {
	printCenterText(MOVING_TEXT, 10);
}

bool Display::updateBlinkState() {
	if (sharedData->getState() == MachineState::SETTINGS_OFFSET_ADJUSTING) {
		bool currentState = (millis() / 250) % 4;
		bool stateChanged = currentState != lastBlinkState;
		lastBlinkState = currentState;
		return stateChanged;
	}
	return false;
}

void Display::printBorder() {
    u8g2->drawHLine(0, 0, u8g2->getDisplayWidth());
    u8g2->drawHLine(0, u8g2->getDisplayHeight()-1 , u8g2->getDisplayWidth());
	u8g2->drawVLine(0, 0, u8g2->getDisplayHeight()-1);
	u8g2->drawVLine(u8g2->getDisplayWidth() -1, 0, u8g2->getDisplayHeight()-1);
}

void Display::printPrevious() {
	u8g2->setFont(u8g2_font_profont11_tr);
	char output[20];	
	sprintf(&output[0], "Prev: %03.1f", sharedData->getLastDistance() / DENOMINATION);
	int width = u8g2->getUTF8Width(output);
	u8g2->drawStr(u8g2->getDisplayWidth() - width - 4, u8g2->getMaxCharHeight() + 1, output);
}

void Display::printLockState() {
	if(sharedData->isLocked()) {
		u8g2->drawXBMP(4, 4, locked_width, locked_height, locked_bits);
	} else {
		u8g2->drawXBMP(4, 4, unlocked_width, unlocked_height, unlocked_bits);
	}
}

void Display::printSelectionMenu() {
	char* upper = sharedData->getMenuEntries()[0];
	if (upper != NULL) {
		u8g2->setFont(u8g2_font_profont10_tr);
		int width = u8g2->getUTF8Width(upper);
		u8g2->drawStr(u8g2->getDisplayWidth() / 2 - width / 2, u8g2->getMaxCharHeight() + 5, upper);
	}

	char* mid = sharedData->getMenuEntries()[1];
	if(mid !=  NULL) {
		u8g2->setFont(u8g2_font_profont15_tf);
		int width = u8g2->getUTF8Width(mid);
		u8g2->drawStr(u8g2->getDisplayWidth() / 2 - width / 2, u8g2->getDisplayHeight() / 2 + u8g2->getMaxCharHeight() / 2 - 4, mid);
	}
	
	char* lower = sharedData->getMenuEntries()[2];
	if (lower != NULL) {
		u8g2->setFont(u8g2_font_profont10_tr);
		int width = u8g2->getUTF8Width(lower);
		u8g2->drawStr(u8g2->getDisplayWidth() / 2 - width / 2, u8g2->getDisplayHeight() - u8g2->getMaxCharHeight() - 2, lower);
	}
}

void Display::tick() {
	if (this->sharedData->shouldUpdateDisplay() || updateBlinkState()) {
		u8g2->clearBuffer();
		printBorder();
		switch(this->sharedData->getState()) {
			case MachineState::CALIBRATION_NEEDED: 
				updateCalibrationText();
				break;
			case MachineState::CALIBRATING: 
			case MachineState::PREP_CALIBRATION:
				updateCalibratingText();
				break;
			case MachineState::IDLE:
				printPrevious();
				printLockState();
				updatePositionReadings(false);
				break;
			case MachineState::SETTINGS_MENU:
				printSelectionMenu();
				break;
			case MachineState::SETTINGS_OFFSET_ADJUSTING:
				updateOffsetReadings();
				break;
			case MachineState::LOCKING: 
				updateLockingText();
				break;
			case MachineState::UNLOCKING: 
				updateUnlockingText();
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
		u8g2->sendBuffer();
	}
}

