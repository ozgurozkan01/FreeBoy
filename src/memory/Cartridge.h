//
// Created by ozgur on 25/03/2024.
//

#ifndef GAMEBOY_EMULATOR_SDL2_CARTRIDGE_H
#define GAMEBOY_EMULATOR_SDL2_CARTRIDGE_H

#include <cstdint>

enum class CartridgeType : uint8_t
{
    ROM_ONLY = 0x00u,
    MBC_1 = 0x01u,
    MBC_1_RAM = 0x02u,
    MBC_1_RAM_BATTERY = 0x03u,
    MBC_2 = 0x05u,
    MBC_2_BATTERY = 0x06u,
    ROM_RAM = 0x08u,
    ROM_RAM_BATTERY = 0x09u,
    MMM_01 = 0x0Bu,
    MMM_01_RAM = 0x0Cu,
    MMM_01_RAM_BATTERY = 0x0Du,
    MBC_3_TIMER_BATTERY = 0x0Fu,
    MBC_3_TIMER_RAM_BATTERY = 0x10u,
    MBC_3 = 0x11u,
    MBC_3_RAM = 0x12u,
    MBC_3_RAM_BATTERY = 0x13u,
    MBC_5 = 0x19,
    MBC_5_RAM = 0x1Au,
    MBC_5_RAM_BATTERY = 0x1Bu,
    MBC_5_RUMBLE = 0x1Cu,
    MBC_5_RUMBLE_RAM = 0x1Du,
    MBC_5_RUMBLE_RAM_BATTERY = 0x1Eu,
    MBC_6 = 0x20u,
    MBC_7_SENSOR_RUMBLE_RAM_BATTERY = 0x22u,
    POCKET_CAMERA = 0xFCu,
    BANDAI_TAMA_5 = 0xFDu,
    HuC_3 = 0xFEu,
    HuC_1_RAM_BATTERY = 0xFFu,

};

class Cartridge {
};


#endif //GAMEBOY_EMULATOR_SDL2_CARTRIDGE_H
