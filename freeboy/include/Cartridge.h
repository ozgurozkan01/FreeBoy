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
        const int entry = 0x100;
        const int nintendoLogo = 0x104;
        const int title = 0x134;
        const int manufacturerCode = 0x13F;
        const int cgbFlag = 0x143;
        const int newLicenseCode = 0x144;
        const int sgbFlag = 0x146;
        const int cartridgeType = 0x147;
        const int romSize = 0x148;
        const int ramSize = 0x149;
        const int destinationCode = 0x14A;
        const int oldLicensCode = 0x14B;
        const int versionNumber = 0x14C;
        const int headerChecksum = 0x14D;
        const int globalChecksum = 0x14E;
    };

    class Cartridge {
    public:
        Cartridge() = default;
        ~Cartridge() = default;
        bool load(const std::string& cartridgeName);

        //void writeHeader();
        char* getTitle();
        void write(const uint16_t address, const uint8_t value);
        [[maybe_unused]] uint8_t read(const uint16_t address) const;
    private:
        char title[16];

        uint32_t cartridgeSize{};
        uint8_t* cartridgeData{};
        RomHeader header{};

        std::string getDestinationCode(const uint8_t& _address) const;
        std::string getROMType(const uint8_t& _address) const;
        std::string getRAMType(const uint8_t& _address) const;
        std::string getCartridgeType();
        std::string getCartridgeLicence();
        std::string isChecksumPassed(const uint8_t& _checksum);
    };
}

#endif //FREEBOY_CARTRIDGE_H
