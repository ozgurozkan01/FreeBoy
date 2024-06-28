//
// Created by ozgur on 6/13/2024.
//

#ifndef FREEBOY_INTERRUPTHANDLER_H
#define FREEBOY_INTERRUPTHANDLER_H

#include <cstdint>
#include "Registers.h"

namespace gameboy
{
    class CPU;
    class MMU;

    using namespace cpu_register;

    enum class InterruptType : uint8_t
    {
        vblank = 0x0,
        lcd = 0x2,
        timer = 0x4,
        serial = 0x8,
        joypad = 0x10
    };

    class InterruptHandler {
    public:

        InterruptHandler() : IME(false) {}

        void handle(CPU* _cpu, MMU* _mmu);

        void setIME(const bool _isEnable) { IME = _isEnable; };
        void setIE(const uint8_t _value)  { IE = _value; };
        void setIF(const uint8_t _value)  { IF = _value; };

        bool getIME() const { return IME; }
        Register8& getIE()  { return IE; }
        Register8& getIF()  { return IF; }

    private:
        void trigger(CPU* _cpu, MMU* _mmu, const uint16_t _address ,const InterruptType _type);

        bool isEnableIF(InterruptType _type);
        bool isEnableIE(InterruptType _type);

        bool IME;
        Register8 IE; // 0xFFFF
        Register8 IF; // 0xFF0F
    };
}



#endif //FREEBOY_INTERRUPTHANDLER_H
