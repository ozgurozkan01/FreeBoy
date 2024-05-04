//
// Created by ozgur on 25/03/2024.
//

#include "../../freeboy/include/Cartridge.h"
#include <fstream>
#include <unordered_map>

constexpr uint16_t HEADER_START_POINT = 0x0100;

namespace gameboy
{
    static const std::string ROM_TYPES[] = {
            "ROM ONLY",                               // 0x00
            "MBC1",                                   // 0x01
            "MBC1+RAM",                               // 0x02
            "MBC1+RAM+BATTERY",                       // 0x03
            "0x04 ???",                               // 0x04
            "MBC2",                                   // 0x05
            "MBC2+BATTERY",                           // 0x06
            "0x07 ???",                               // 0x07
            "ROM+RAM 1",                              // 0x08
            "ROM+RAM+BATTERY 1",                      // 0x09
            "0x0A ???",                               // 0x0A
            "MMM01",                                  // 0x0B
            "MMM01+RAM",                              // 0x0C
            "MMM01+RAM+BATTERY",                      // 0x0D
            "0x0E ???",                               // 0x0E
            "MBC3+timer+BATTERY",                     // 0x0F
            "MBC3+timer+RAM+BATTERY 2",               // 0x10
            "MBC3",                                   // 0x11
            "MBC3+RAM 2",                             // 0x12
            "MBC3+RAM+BATTERY 2",                     // 0x13
            "0x14 ???",                               // 0x14
            "0x15 ???",                               // 0x15
            "0x16 ???",                               // 0x16
            "0x17 ???",                               // 0x17
            "0x18 ???",                               // 0x18
            "MBC5",                                   // 0x19
            "MBC5+RAM",                               // 0x1A
            "MBC5+RAM+BATTERY",                       // 0x1B
            "MBC5+RUMBLE",                            // 0x1C
            "MBC5+RUMBLE+RAM",                        // 0x1D
            "MBC5+RUMBLE+RAM+BATTERY",                // 0x1E
            "0x1F ???",                               // 0x1F
            "MBC6",                                   // 0x20
            "0x21 ???",                               // 0x21
            "MBC7+SENSOR+RUMBLE+RAM+BATTERY"          // 0x22
    };

    static const std::unordered_map<uint8_t , std::string> LICENCE_CODE = {
            {0x00, "None"},
            {0x01, "Nintendo R&D1"},
            {0x08, "Capcom"},
            {0x13, "Electronic Arts"},
            {0x18, "Hudson Soft"},
            {0x19, "b-ai"},
            {0x20, "kss"},
            {0x22, "pow"},
            {0x24, "PCM Complete"},
            {0x25, "san-x"},
            {0x28, "Kemco Japan"},
            {0x29, "seta"},
            {0x30, "Viacom"},
            {0x31, "Nintendo"},
            {0x32, "Bandai"},
            {0x33, "Ocean/Acclaim"},
            {0x34, "Konami"},
            {0x35, "Hector"},
            {0x37, "Taito"},
            {0x38, "Hudson"},
            {0x39, "Banpresto"},
            {0x41, "Ubi Soft"},
            {0x42, "Atlus"},
            {0x44, "Malibu"},
            {0x46, "angel"},
            {0x47, "Bullet-Proof"},
            {0x49, "irem"},
            {0x50, "Absolute"},
            {0x51, "Acclaim"},
            {0x52, "Activision"},
            {0x53, "American sammy"},
            {0x54, "Konami"},
            {0x55, "Hi tech entertainment"},
            {0x56, "LJN"},
            {0x57, "Matchbox"},
            {0x58, "Mattel"},
            {0x59, "Milton Bradley"},
            {0x60, "Titus"},
            {0x61, "Virgin"},
            {0x64, "LucasArts"},
            {0x67, "Ocean"},
            {0x69, "Electronic Arts"},
            {0x70, "Infogrames"},
            {0x71, "Interplay"},
            {0x72, "Broderbund"},
            {0x73, "sculptured"},
            {0x75, "sci"},
            {0x78, "THQ"},
            {0x79, "Accolade"},
            {0x80, "misawa"},
            {0x83, "lozc"},
            {0x86, "Tokuma Shoten Intermedia"},
            {0x87, "Tsukuda Original"},
            {0x91, "Chunsoft"},
            {0x92, "Video system"},
            {0x93, "Ocean/Acclaim"},
            {0x95, "Varie"},
            {0x96, "Yonezawa/s’pal"},
            {0x97, "Kaneko"},
            {0x99, "Pack in soft"},
            {0xA4, "Konami (Yu-Gi-Oh!)"}
    };

    bool Cartridge::loadCartridge(const std::string& cartridgeName)
    {
        std::ifstream cartridgeFile(cartridgeName, std::ios::binary);

        if (!cartridgeFile.is_open())
        {
            printf("Cartridge file could not be opened!!\n");
            return false;
        }

        cartridgeFile.seekg(0, std::ios::end);

        romSize = static_cast<uint32_t>(cartridgeFile.tellg());
        romData = new uint8_t[romSize];

        cartridgeFile.seekg(std::ios::beg);

        if (!cartridgeFile.read(reinterpret_cast<char*>(romData), romSize))
        {
            printf("ROM file could not be read properly.\n");
            cartridgeFile.close();
            return false;
        }
        cartridgeFile.close();

        // LOAD ROM HEADER
        romHeader = reinterpret_cast<RomHeader*>(romData + HEADER_START_POINT);
        // PRINT HEADER INFORMATIONS
        printf("Successfully loaded ROM from file path: %s:\n", cartridgeName.c_str());
        printf("\tTitle    : %s\n", romHeader->title);
        printf("\tType     : %2.2X (%s)\n", romHeader->cartridgeType, getCartridgeType().c_str());
        printf("\tROM Size : %d KB\n", 32 << romHeader->romSize);
        printf("\tRAM Size : %2.2X\n", romHeader->ramSize);
        printf("\tLIC Code : %2.2X (%s)\n", romHeader->oldLicenseeCode, getCartridgeLicence().c_str());
        printf("\tROM Vers : %2.2X\n", romHeader->romVersion);
        printf("\tChecksum : %2.2X (%s)\n", romHeader->headerChecksum, isChecksumPassed() ? "PASSED" : "FAILED");

        return true;
    }

    std::string Cartridge::getCartridgeType()
    {
        if (romHeader->cartridgeType <= 0x22)
        {
            return ROM_TYPES[romHeader->cartridgeType];
        }

        return "UNKNOWN";
        exit(-1);
    }

    std::string Cartridge::getCartridgeLicence()
    {
        if (romHeader->newLicenseeCode <= 0xA4)
        {
            auto licence = LICENCE_CODE.find( romHeader->oldLicenseeCode);
            if (licence != LICENCE_CODE.end())
            {
                return licence->second;
            }
        }
        return "UNKNOWN";
        exit(-1);
    }

    bool Cartridge::isChecksumPassed()
    {
        uint8_t checksum = 0;
        for (uint16_t address = 0x0134; address <= 0x014C; address++)
        {
            checksum = checksum - romData[address] - 1;
        }
        /* The boot ROM verifies this checksum. If the byte at $014D(Header Checksum) does not match the lower 8 bits of checksum,
         * the boot ROM will lock up and the program in the cartridge won’t run.*/
        return  romHeader->headerChecksum == (checksum & 0xFF);
    }

    void Cartridge::writeCartridge(uint16_t address, uint8_t value) { }
    uint8_t Cartridge:: readCartridge(uint16_t address) { return romData[address]; }
}