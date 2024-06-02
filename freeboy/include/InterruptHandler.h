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
            void setIE(uint8_t _value);
            void setIF(uint8_t _value);

            bool getIME();
            uint8_t getIE();
            uint8_t getIF();

        private:
            bool interruptMasterEnable; // IME
            uint8_t interruptEnableRegister; // IE
            uint8_t interruptFlagRegister; // IF
        };
    }
}

#endif //FREEBOY_INTERRUPTHANDLER_H
