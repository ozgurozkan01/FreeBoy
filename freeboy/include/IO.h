//
// Created by ozgur on 6/15/2024.
//

#ifndef FREEBOY_IO_H
#define FREEBOY_IO_H

#include <array>
#include <cstdint>

namespace gameboy
{
    class IO {
    public:
        IO() = default;

        uint8_t read(const uint16_t _address);
        void write(const uint16_t _address, const uint8_t _value);
    };
}

#endif //FREEBOY_IO_H
