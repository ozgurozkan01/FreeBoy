//
// Created by ozgur on 12/04/2024.
//

#ifndef FREEBOY_UTILITY_H
#define FREEBOY_UTILITY_H

#include <cstdint>
#include <iostream>

namespace gameboy
{
    static void bitSet(int& _variable, int _shifting) { _variable |= 1 << _shifting; }
    static void bitReset(int& _variable, int _shifting) { _variable &= ~(1 << _shifting); }
    static uint8_t bitGet(int& _variable, int _shifting) { return _variable &= 1 << _shifting; }
    static void noImplementation() { std::cout << "NOT YET IMPLEMENTED!!\n"; }
}

#endif //FREEBOY_UTILITY_H