//
// Created by ozgur on 6/19/2024.
//

#ifndef FREEBOY_PPU_H
#define FREEBOY_PPU_H

#include <cstdint>
#include <array>

namespace gameboy
{
    class PPU {

        enum class PPUMode : uint8_t
        {
            hBlank,
            vBlank,
            oamScan,
            drawPixel
        };

        struct OAM
        {
            uint8_t y;
            uint8_t x;
            uint8_t tileIndex;
            uint8_t attributes;
        } oamRAM[0x28] = {0};

    public:
        PPU() = default;

        void writeVRAM(const uint16_t _address, const uint8_t _value);
        uint8_t readVRAM(const uint16_t _address);

        void writeOAM(const uint16_t _address, const uint8_t _value);
        uint8_t readOAM(const uint16_t _address);

    private:
        std::array<uint8_t, 0x2000> videoRAM = {0};
    };
}

#endif //FREEBOY_PPU_H
