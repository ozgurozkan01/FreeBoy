//
// Created by ozgur on 6/13/2024.
//

#ifndef FREEBOY_INTERRUPTHANDLER_H
#define FREEBOY_INTERRUPTHANDLER_H

#include <cstdint>

namespace gameboy
{
    enum class InterruptFlags : uint8_t
    {
        vblank = 0,
        lcd = 1,
        timer = 2,
        serial = 3,
        joypad = 4
    };

    class InterruptHandler {
    public:

        InterruptHandler() : IME(false) {}

        void setIME(const bool _isEnable) { IME = _isEnable; };
        void setIE(const uint8_t _value) { IE = _value; };
        void setIF(const uint8_t _value) { IF = _value; };

        bool getIME() const { return IME; }
        uint8_t getIE() const { return IE; }
        uint8_t getIF() const { return IF; }

    private:
        bool IME;
        uint8_t IE;
        uint8_t IF;
    };
}



#endif //FREEBOY_INTERRUPTHANDLER_H
