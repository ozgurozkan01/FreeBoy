//
// Created by ozgur on 5/31/2024.
//

#ifndef FREEBOY_RAM_H
#define FREEBOY_RAM_H

#include <array>
#include <cstdint>

namespace gameboy::ram
{
    class WRAM
    {
    public:
        WRAM() = default;

        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t value);

    private:
        static constexpr uint16_t  MEMORY_SIZE = 0x2000;
        static constexpr uint16_t MEMORY_OFFSET = 0xC000;
        std::array<uint8_t, MEMORY_SIZE> memory;
    };

    class HRAM
    {
    public:
        HRAM() = default;
        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t value);
    private:
        static constexpr uint16_t MEMORY_SIZE = 0x80;
        static constexpr uint16_t MEMORY_OFFSET = 0xFF80;
        std::array<uint8_t, MEMORY_SIZE> memory;
    };

    class VRAM
    {
    public:
        VRAM() = default;
        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t value);
    private:
        static constexpr int MEMORY_SIZE = 0x2000;
        std::array<uint8_t, MEMORY_SIZE> memory; // Video RAM (tile data storage from $8000-97FF)
    };
}

#endif //FREEBOY_RAM_H
