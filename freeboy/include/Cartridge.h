//
// Created by ozgur on 10/04/2024.
//

#ifndef FREEBOY_CARTRIDGE_H
#define FREEBOY_CARTRIDGE_H


#include <cstdint>
#include <string>

enum class MBC_TYPE : uint8_t
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

enum class ROM_SIZE : uint8_t
{
    // kibibyte
    KIB_32 = 0x00u,
    KIB_64 = 0x01u,
    KIB_128 = 0x02u,
    KIB_256 = 0x03u,
    KIB_512 = 0x04u,
    // mebibyte
    MIB_1 = 0x05u,
    MIB_2 = 0x06u,
    MIB_4 = 0x07u,
    MIB_8 = 0x08u,
    MIB_1_1 = 0x52u,
    MIB_1_2 = 0x53u,
    MIB_1_5 = 0x54u,
};

enum class RAM_SIZE : uint8_t
{
    NO_RAM = 0x00u,
    UNUSED = 0x01u,
    KIB_8 = 0x02u, // 1 bank
    KIB_32 = 0x03u, // 4 banks of 8 KiB each
    KIB_128 = 0x04u, // 16 banks of 8 KiB each
    KIB_64 = 0x05u, // 8 banks of 8 KiB each
};

enum class ROM_START_LOC : uint16_t
{
    ENTRY_POINT = 0x0100,
    NINTENDO_LOGO = 0x0104,
    TITLE = 0x0134,
    MANUFACTURER_CODE = 0x013F,
    CGB_FLAG = 0x0143,
    NEW_LICENSEE_CODE = 0x0144,
    SGB_FLAG = 0x146,
    CARTRIDGE_TYPE = 0x0147,
    ROM_SIZE = 0x0148,
    RAM_SIZE = 0x0149,
    DESTINATION_CODE = 0x014A,
    OLD_LICENSEE_CODE = 0x014B,
    MASK_ROM_VERSION_NUMBER = 0x014C,
    HEADER_CHECKSUM = 0x014D,
    GLOBAL_CHECKSUM = 0x014E,
};


class Cartridge {

};



#endif //FREEBOY_CARTRIDGE_H
