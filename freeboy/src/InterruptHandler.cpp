//
// Created by ozgur on 6/28/2024.
//

#include "../include/InterruptHandler.h"
#include "../include/CPU.h"
#include "../include/MMU.h"

namespace gameboy
{
    InterruptHandler::InterruptHandler() :
    IME(false),
    isScheduledIME(false),
    IF(0),
    IE(0)
    {}

    void InterruptHandler::handleInterrupt(CPU* _cpu, MMU* _mmu)
    {
        if      (isEnableIF(InterruptType::vblank) && isEnableIE(InterruptType::vblank)) { trigger(_cpu, _mmu, 0x40, InterruptType::vblank); }
        else if (isEnableIF(InterruptType::lcd)    && isEnableIE(InterruptType::lcd))    { trigger(_cpu, _mmu, 0x48, InterruptType::lcd);    }
        else if (isEnableIF(InterruptType::timer)  && isEnableIE(InterruptType::timer))  { trigger(_cpu, _mmu, 0x50, InterruptType::timer);  }
        else if (isEnableIF(InterruptType::serial) && isEnableIE(InterruptType::serial)) { trigger(_cpu, _mmu, 0x58, InterruptType::serial); }
        else if (isEnableIF(InterruptType::joypad) && isEnableIE(InterruptType:: joypad)) { trigger(_cpu, _mmu, 0x60, InterruptType::joypad); }
    }

    bool InterruptHandler::isEnableIF(InterruptType _type) { return IF.read() & static_cast<uint8_t>(_type); }
    bool InterruptHandler::isEnableIE(InterruptType _type) { return IE.read() & static_cast<uint8_t>(_type); }

    void InterruptHandler::trigger(CPU* _cpu, MMU* _mmu, const uint16_t _address ,const InterruptType _type)
    {
        // printf("\nTRIGGERED\n");
        _mmu->push(_cpu->sp, _cpu->pc);
        _cpu->pc = _address;
        _cpu->isHalted = false;
        IF &= ~(static_cast<uint8_t>(_type));
        IME = false;

        _cpu->emulateCycles(5);
    }

    void InterruptHandler::run(CPU* _cpu, MMU* _mmu)
    {
        if (_cpu->isHalted)
        {
            _cpu->emulateCycles(1);

            if (getIF().read() & getIE().read() & 0x1F)
            {
                _cpu->isHalted = false;
            }
        }

        if (getIME())
        {
            handleInterrupt(_cpu, _mmu);
            setScheduledIME(false);
        }

        if (getScheduledIME())
        {
            setIME(true);
            setScheduledIME(false);
        }
    }
}