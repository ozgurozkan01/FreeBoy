//
// Created by ozgur on 5/20/2024.
//

#ifndef FREEBOY_INTERRUPTHANDLER_H
#define FREEBOY_INTERRUPTHANDLER_H

#include <cstdint>

namespace gameboy
{
    namespace instruction
    {
        enum class InterruptEnable : uint8_t
        {
            JOYPAD = (1 << 4),
            SERIAL = (1 << 3),
            TIMER = (1 << 2),
            LCD = (1 << 1),
            VBLANK = (1 << 0)
        };

        class InterruptHandler {
        public:
            InterruptHandler();
            ~InterruptHandler() = default;

            void setIME(bool _isEnable);

        private:
            bool interruptMasterEnable;
        };
    }
}

#endif //FREEBOY_INTERRUPTHANDLER_H
