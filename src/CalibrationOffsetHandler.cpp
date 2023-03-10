#include <CalibrationOffsetHandler.h>

CalibrationOffsetHandler::CalibrationOffsetHandler(SharedData* sharedData) {
    this->sharedData = sharedData;
    int offset = NVS.getInt(KEY_OFFSET);
    sharedData->setOffset(offset);
}

CalibrationOffsetHandler::~CalibrationOffsetHandler() {
}

void CalibrationOffsetHandler::tick() {
    if(sharedData->getState() == MachineState::SETTINGS_OFFSET_ADJUSTING &&
        sharedData->enterButton->pressed()) {
            int oldOffset = NVS.getInt(KEY_OFFSET);
            int diff = sharedData->getOffset() - oldOffset;
            sharedData->setCurrentPosition(sharedData->getCurrentPosition() + diff);
            sharedData->setTargetPosition(sharedData->getCurrentPosition());
            NVS.setInt(KEY_OFFSET, sharedData->getOffset());
            sharedData->switchState(MachineState::IDLE);
    }
}

