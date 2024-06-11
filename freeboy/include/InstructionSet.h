//
// Created by ozgur on 3/04/2024.
//

#ifndef FREEBOY_Info_H
#define FREEBOY_Info_H

#include <cstdint>
#include <array>
#include <string>

namespace gameboy
{
    namespace instruction
    {
        class InstructionSet
        {
        public:

            struct Header
            {
                std::string nmenomic;
                uint8_t length;
                uint8_t mCycles;
            };

            InstructionSet();

            std::array<Header, 0x100> standardInstructions;
            std::array<Header, 0x100> cbInstructions;
        };
    }
}
#endif //FREEBOY_Info_H
