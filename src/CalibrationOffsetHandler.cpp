#include <CalibrationOffsetHandler.h>

CalibrationOffsetHandler::CalibrationOffsetHandler(SharedData* sharedData) {
    this->sharedData = sharedData;
    float offset = NVS.getFloat(KEY_OFFSET);
    sharedData->setOffset(offset);
}

CalibrationOffsetHandler::~CalibrationOffsetHandler() {
}

void CalibrationOffsetHandler::tick() {
    if(sharedData->getState() == MachineState::SETTINGS_OFFSET_ADJUSTING &&
        sharedData->enterButton->pressed()) {
            float oldOffset = NVS.getFloat(KEY_OFFSET);
            double diff = sharedData->getOffset() - oldOffset;
            sharedData->setCurrentPosition(sharedData->getCurrentPosition() + diff);
            sharedData->setTargetPosition(sharedData->getCurrentPosition());
            NVS.setFloat(KEY_OFFSET, sharedData->getOffset());
            sharedData->switchState(MachineState::IDLE);
    }
}

