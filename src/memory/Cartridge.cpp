//
// Created by ozgur on 25/03/2024.
//

#include "Cartridge.h"
#include <fstream>

Cartridge::Cartridge()
{
}

bool Cartridge::loadCartridge(char *cartridgeName)
{
    std::fstream cartridgeFile;
    cartridgeFile.open(cartridgeName, std::ios::binary);

    if (!cartridgeFile.is_open())
    {
        return false;
    }

    cartridgeFile.seekg(0, std::ios::end);
    romSize = cartridgeFile.tellg();
    cartridgeFile.seekg(std::ios::beg);

    if (!cartridgeFile.read((char*) romData, romSize))
    {
        printf("ROM file : %s could not be read properly.\n", cartridgeName);
        return false;
    }
    cartridgeFile.close();

    romHeader = (RomHeader *)(romData + 0x100);
    romHeader->title[15] = 0;
    return true;
}

/*std::string Cartridge::getCartridgeLicenseeName()
{
    if (romHeader->newLicenseeCode <= 0xA4)
    {
        return licenseeCode[romHeader->oldLicenseeCode];
    }
    return "UKNOWN";
}*/

CartridgeType Cartridge::getCartridgeTypeName()
{
    return romHeader->cartridgeType;
}

Cartridge::~Cartridge()
{
    delete romHeader;
}
