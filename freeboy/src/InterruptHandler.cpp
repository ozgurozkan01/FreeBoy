//
// Created by ozgur on 5/20/2024.
//

#include "../include/InterruptHandler.h"

namespace gameboy
{
    namespace instruction
    {
        InterruptHandler::InterruptHandler() : interruptMasterEnable(false) {}

        void InterruptHandler::setIME(bool _isEnable)
        {
            interruptMasterEnable = _isEnable;
        }
    }
}
