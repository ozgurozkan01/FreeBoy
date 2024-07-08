//
// Created by ozgur on 6/13/2024.
//

#ifndef FREEBOY_INTERRUPTHANDLER_H
#define FREEBOY_INTERRUPTHANDLER_H

#include <cstdint>
#include "Registers.h"
#include <cstdio>

namespace gameboy
{
    class CPU;
    class MMU;

    using namespace cpu_register;

    enum class InterruptType : uint8_t
    {
        vblank = 0x1,
        lcd = 0x2,
        timer = 0x4,
        serial = 0x8,
        joypad = 0x10
    };

    class InterruptHandler {
    public:
        InterruptHandler();

        void run(CPU* _cpu, MMU* _mmu);

        void handleInterrupt(CPU* _cpu, MMU* _mmu);
        void trigger(CPU* _cpu, MMU* _mmu, const uint16_t _address ,const InterruptType _type);

        void setScheduledIME(const bool _isEnable) { isScheduledIME = _isEnable; };
        void setIME(const bool _isEnable) { IME = _isEnable; };
        void setIE(const uint8_t _value)  { IE = _value; };
        void setIF(const uint8_t _value)  { IF = _value; };
        void requestInterrupt
        (const InterruptType _type) { IF |= static_cast<uint8_t>(_type); };

        bool getScheduledIME() const { return isScheduledIME; }
        bool getIME() const { return IME; }
        Register8& getIE()  { return IE; }
        Register8& getIF()  { return IF; }

    private:
        bool isEnableIF(InterruptType _type);
        bool isEnableIE(InterruptType _type);

        bool IME;
        bool isScheduledIME;
        Register8 IE;
        Register8 IF;
    };
}



#endif //FREEBOY_INTERRUPTHANDLER_H
