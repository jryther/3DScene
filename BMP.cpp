#include "BMP.h"

BMP::BMP(const char* FilePath)
{
    std::fstream hFile(FilePath, std::ios::in | std::ios::binary);
    if (!hFile.is_open()) throw std::invalid_argument("Error: File Not Found.");

    hFile.seekg(0, std::ios::end);
    std::size_t Length = hFile.tellg();
    hFile.seekg(0, std::ios::beg);
    std::vector<std::uint8_t> FileInfo(Length);
    hFile.read(reinterpret_cast<char*>(FileInfo.data()), 54);

    if (FileInfo[0] != 'B' && FileInfo[1] != 'M')
    {
        hFile.close();
        throw std::invalid_argument("Error: Invalid File Format. Bitmap Required.");
    }

    if (FileInfo[28] != 24 && FileInfo[28] != 32)
    {
        hFile.close();
        throw std::invalid_argument("Error: Invalid File Format. 24 or 32 bit Image Required.");
    }

    BitsPerPixel = FileInfo[28];
    width = FileInfo[18] + (FileInfo[19] << 8);
    height = FileInfo[22] + (FileInfo[23] << 8);
    std::uint32_t PixelsOffset = FileInfo[10] + (FileInfo[11] << 8);
    std::uint32_t size = ((width * BitsPerPixel + 31) / 32) * 4 * height;
    Pixels.resize(size);

    hFile.seekg(PixelsOffset, std::ios::beg);
    hFile.read(reinterpret_cast<char*>(Pixels.data()), size);
    hFile.close();
}