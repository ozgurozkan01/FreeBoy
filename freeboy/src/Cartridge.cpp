//
// Created by ozgur on 25/03/2024.
//

#include "../../freeboy/include/Cartridge.h"
#include <fstream>

Cartridge::Cartridge()
{
}

bool Cartridge::loadCartridge(const std::string& cartridgeName)
{
    std::ifstream cartridgeFile(cartridgeName, std::ios::binary);

    if (!cartridgeFile.is_open())
    {
        printf("Cartridge file could not be opened!!\n");
        return false;
    }

    cartridgeFile.seekg(0, std::ios::end);

    romSize = cartridgeFile.tellg();
    romData = new uint8_t[romSize];

    cartridgeFile.seekg(std::ios::beg);

    if (!cartridgeFile.read((char*) romData, romSize))
    {
        printf("ROM file could not be read properly.\n");
        return false;
    }
    cartridgeFile.close();

    loadRomHeader();

    return true;
}

std::string Cartridge::getCartridgeTypeName()
{
    switch (romHeader.mbcType)
    {
        case MBC_TYPE::ROM_ONLY:
            return "ROM_ONLY";
        case MBC_TYPE::MBC_1:
            return "MBC_1";
        case MBC_TYPE::MBC_1_RAM:
            return "MBC_1_RAM";
        case MBC_TYPE::MBC_1_RAM_BATTERY:
            return "MBC_1_RAM_BATTERY";
        case MBC_TYPE::MBC_2:
            return "MBC_2";
        case MBC_TYPE::MBC_2_BATTERY:
            return "MBC_2_BATTERY";
        case MBC_TYPE::ROM_RAM:
            return "ROM_RAM";
        case MBC_TYPE::ROM_RAM_BATTERY:
            return "ROM_RAM_BATTERY";
        case MBC_TYPE::MMM_01:
            return "MMM_01";
        case MBC_TYPE::MMM_01_RAM:
            return "MMM_01_RAM";
        case MBC_TYPE::MMM_01_RAM_BATTERY:
            return "MMM_01_RAM_BATTERY";
        case MBC_TYPE::MBC_3_TIMER_BATTERY:
            return "MBC_3_TIMER_BATTERY";
        case MBC_TYPE::MBC_3_TIMER_RAM_BATTERY:
            return "MBC_3_TIMER_RAM_BATTERY";
        case MBC_TYPE::MBC_3:
            return "MBC_3";
        case MBC_TYPE::MBC_3_RAM:
            return "MBC_3_RAM";
        case MBC_TYPE::MBC_3_RAM_BATTERY:
            return "MBC_3_RAM_BATTERY";
        case MBC_TYPE::MBC_5:
            return "MBC_5";
        case MBC_TYPE::MBC_5_RAM:
            return "MBC_5_RAM";
        case MBC_TYPE::MBC_5_RAM_BATTERY:
            return "MBC_5_RAM_BATTERY";
        case MBC_TYPE::MBC_5_RUMBLE:
            return "MBC_5_RUMBLE";
        case MBC_TYPE::MBC_5_RUMBLE_RAM:
            return "MBC_5_RUMBLE_RAM";
        case MBC_TYPE::MBC_5_RUMBLE_RAM_BATTERY:
            return "MBC_5_RUMBLE_RAM_BATTERY";
        case MBC_TYPE::MBC_6:
            return "MBC_6";
        case MBC_TYPE::MBC_7_SENSOR_RUMBLE_RAM_BATTERY:
            return "MBC_7_SENSOR_RUMBLE_RAM_BATTERY";
        case MBC_TYPE::POCKET_CAMERA:
            return "POCKET_CAMERA";
        case MBC_TYPE::BANDAI_TAMA_5:
            return "BANDAI_TAMA_5";
        case MBC_TYPE::HuC_3:
            return "HuC_3";
        case MBC_TYPE::HuC_1_RAM_BATTERY:
            return "HuC_1_RAM_BATTERY";
    }
    return "";
}

Cartridge::~Cartridge()
{
}

std::string Cartridge::getRamSize() {
    switch (romHeader.ramSize) {
        case RAM_SIZE::NO_RAM:
            return "NO_RAM";
        case RAM_SIZE::UNUSED:
            return "UNUSED";
        case RAM_SIZE::KIB_8:
            return "KIB_8";
        case RAM_SIZE::KIB_32:
            return "KIB_32";
        case RAM_SIZE::KIB_128:
            return "KIB_128";
        case RAM_SIZE::KIB_64:
            return "KIB_64";
    }
    return "";
}

std::string Cartridge::getRomSize()
{
    switch (romHeader.romSize)
    {
        case ROM_SIZE::KIB_32:
            return "KIB_32";
        case ROM_SIZE::KIB_64:
            return "KIB_64";
        case ROM_SIZE::KIB_128:
            return "KIB_128";
        case ROM_SIZE::KIB_256:
            return "KIB_256";
        case ROM_SIZE::KIB_512:
            return "KIB_512";
        case ROM_SIZE::MIB_1:
            return "MIB_1";
        case ROM_SIZE::MIB_2:
            return "MIB_2";
        case ROM_SIZE::MIB_4:
            return "MIB_4";
        case ROM_SIZE::MIB_8:
            return "MIB_8";
        case ROM_SIZE::MIB_1_1:
            return "MIB_1_1";
        case ROM_SIZE::MIB_1_2:
            return "MIB_1_2";
        case ROM_SIZE::MIB_1_5:
            return "MIB_1_5";
    }
    return "";
}

void Cartridge::loadRomHeader()
{
    // ROM HEADER
    romHeader.entryPoint = romData[static_cast<uint16_t>(ROM_START_LOC::ENTRY_POINT)];

    for (uint16_t i = static_cast<uint16_t>(ROM_START_LOC::NINTENDO_LOGO), index = 0; i < static_cast<uint16_t>(ROM_START_LOC::TITLE); ++i, ++index)
    {
        romHeader.nintendoLogo[index] = romData[i];
    }
    for (uint16_t i = static_cast<uint16_t>(ROM_START_LOC::TITLE), index = 0; i < static_cast<uint16_t>(ROM_START_LOC::MANUFACTURER_CODE); ++i, ++index)
    {
        romHeader.title[index] = (char)romData[i];
    }
    romHeader.manufacturerCode = romData[static_cast<uint16_t>(ROM_START_LOC::MANUFACTURER_CODE)];
    romHeader.cgbFlag = romData[static_cast<uint16_t>(ROM_START_LOC::CGB_FLAG)];
    romHeader.newLicenseeCode = romData[static_cast<uint16_t>(ROM_START_LOC::NEW_LICENSEE_CODE)];
    romHeader.sgbFlag = romData[static_cast<uint16_t>(ROM_START_LOC::SGB_FLAG)];
    romHeader.mbcType = static_cast<MBC_TYPE>(romData[static_cast<uint16_t>(ROM_START_LOC::CARTRIDGE_TYPE)]);
    romHeader.romSize = static_cast<ROM_SIZE>(romData[static_cast<uint16_t>(ROM_START_LOC::ROM_SIZE)]);
    romHeader.ramSize = static_cast<RAM_SIZE>(romData[static_cast<uint16_t>(ROM_START_LOC::RAM_SIZE)]);
    romHeader.destinationCode = romData[static_cast<uint16_t>(ROM_START_LOC::DESTINATION_CODE)];
    romHeader.oldLicenseeCode = romData[static_cast<uint16_t>(ROM_START_LOC::OLD_LICENSEE_CODE)];
    romHeader.maskRomVersionNumber = romData[static_cast<uint16_t>(ROM_START_LOC::MASK_ROM_VERSION_NUMBER)];
    romHeader.headerChecksum = romData[static_cast<uint16_t>(ROM_START_LOC::HEADER_CHECKSUM)];
    romHeader.globalChecksum = romData[static_cast<uint16_t>(ROM_START_LOC::GLOBAL_CHECKSUM)];

    printf("Cartridge Loaded:\n");
    printf("\t Title    : %s\n", romHeader.title);
    printf("\t Type     : %s\n", getCartridgeTypeName().c_str());
    printf("\t ROM Size : %s\n", getRomSize().c_str());
    printf("\t RAM Size : %s\n", getRamSize().c_str());
    printf("\t ROM Vers : %2.2X\n", romHeader.maskRomVersionNumber);

    uint16_t x = 0;
    for (uint16_t i=0x0134; i<=0x014C; i++) {
        x = x - romData[i] - 1;
    }
    printf("\t Checksum : %2.2X (%s)\n", romHeader.headerChecksum, (x & 0xFF) ? "PASSED" : "FAILED");

}
