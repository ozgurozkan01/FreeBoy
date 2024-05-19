//
// Created by ozgur on 10/04/2024.
//

#ifndef FREEBOY_CARTRIDGE_H
#define FREEBOY_CARTRIDGE_H


#include <cstdint>
#include <string>

namespace gameboy
{
    struct RomHeader
    {
        uint8_t entryPoint[4];
        uint8_t nintendoLogo[0x30u];

        char title[0x10u];
        uint16_t newLicenseeCode;
        uint8_t sgbFlag; // Super GameBoy support flag
        uint8_t cartridgeType;
        uint8_t romSize;
        uint8_t ramSize;
        uint8_t destinationCode;
        uint8_t oldLicenseeCode;
        uint8_t romVersion;
        uint8_t headerChecksum;
        uint16_t globalChecksum;
    };

    class Cartridge {
    public:
        Cartridge() = default;
        ~Cartridge() = default;
        bool load(const std::string& cartridgeName);

        //void writeHeader();
        void write(uint16_t address, uint8_t value);
        [[maybe_unused]] uint8_t read(uint16_t address);
    private:
        uint32_t romSize{};
        uint8_t* romData{};
        RomHeader* romHeader{};

        std::string getCartridgeType();
        std::string getCartridgeLicence();
        bool isChecksumPassed();
    };
}

#endif //FREEBOY_CARTRIDGE_H
