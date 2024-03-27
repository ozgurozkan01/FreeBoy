//
// Created by ozgur on 25/03/2024.
//

#ifndef GAMEBOY_EMULATOR_SDL2_CARTRIDGE_H
#define GAMEBOY_EMULATOR_SDL2_CARTRIDGE_H

#include <cstdint>
#include <string>

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

enum class RomSize : uint8_t
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

enum class RamSize : uint8_t
{
    NO_RAM = 0x00u,
    UNUSED = 0x01u,
    KIB_8 = 0x02u, // 1 bank
    KIB_32 = 0x03u, // 4 banks of 8 KiB each
    KIB_128 = 0x04u, // 16 banks of 8 KiB each
    KIB_64 = 0x05u, // 8 banks of 8 KiB each
};

struct RomHeader
{
    uint32_t entryPoint; // 0x0104-0x0133
    uint8_t nintendoLogo[0x30u];
    char title[0x10];
    uint8_t manifacturerCode;
    uint8_t sgbFlag;
    CartridgeType cartridgeType; // what kind of hardware is present on the cartridge
    RamSize ramSize; // how much RAM is present on the cartridge
    RomSize romSize; // how much ROM is present on the cartridge
    uint8_t destinationCode;
    uint8_t oldLicenseeCode;
    uint16_t newLicenseeCode;
    uint8_t romVersionNumber; // the version number of the game
    uint8_t checksum;
    uint16_t globalChecksum;
};

class Cartridge {
public:
    Cartridge();
    ~Cartridge();
    bool loadCartridge(char* cartridgeName);
    //std::string getCartridgeLicenseeName();
    CartridgeType getCartridgeTypeName();
private:
    char fileName[1024]{};
    uint32_t romSize{};
    uint8_t* romData{};
    RomHeader* romHeader{};
    std::string *licenseeCode;
};


#endif //GAMEBOY_EMULATOR_SDL2_CARTRIDGE_H
