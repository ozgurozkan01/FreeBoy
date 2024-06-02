//
// Created by ozgur on 5/20/2024.
//

#include "../include/InterruptHandler.h"

namespace gameboy
{
    namespace instruction
    {
        InterruptHandler::InterruptHandler() : interruptMasterEnable(false) {}

        void InterruptHandler::setIME(bool _isEnable) { interruptMasterEnable = _isEnable; }
        void InterruptHandler::setIE(uint8_t _value) { interruptEnableRegister = _value; }
        void InterruptHandler::setIF(uint8_t _value) { interruptFlagRegister = _value; }

        bool InterruptHandler::getIME() { return interruptMasterEnable; }
        uint8_t InterruptHandler::getIE() { return interruptEnableRegister; }
        uint8_t InterruptHandler::getIF() { return interruptFlagRegister; }
    }
}
