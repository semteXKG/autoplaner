#include <Display.h>
#include <SharedData.h>

Display::Display(SharedData* sharedData) {
    this->u8g2 = new U8G2_SH1106_128X64_NONAME_1_HW_I2C(U8G2_R0);
    this->sharedData = sharedData;
    u8g2->begin();
}

Display::~Display() {
}

void Display::printCenterText(char* text, int size) {
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

void Display::updatePositionReadings() {
	u8g2->firstPage();
	do {
		char output[20];	
		int size;
		if(sharedData->getCurrentPosition() == sharedData->getTargetPosition()) {
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
	printCenterText("Calibration needed", 16);
}

void Display::tick() {
	if (this->sharedData->shouldUpdateDisplay()) {
		switch(this->sharedData->getState()) {
			case SharedData::MachineState::CALIBRATION_NEEDED: 
				updateCalibrationText();
				break;
			case SharedData::MachineState::IDLE:
				updatePositionReadings();
				break;
			default:
			break;
		}
	}
}
