
#include <iostream>
#include <fstream>
#include <cstdint>

using namespace std;

#pragma pack(push, 1)

struct BMPFileHeader
{

    uint16_t fileType{0x4D42}; // "BM"

    uint32_t fileSize{0};

    uint16_t reserved1{0};

    uint16_t reserved2{0};

    uint32_t offsetData{0};
};

struct BMPInfoHeader
{

    uint32_t size{0};

    int32_t width{0};

    int32_t height{0};

    uint16_t planes{1};

    uint16_t bitCount{0};

    uint32_t compression{0};

    uint32_t sizeImage{0};

    int32_t xPixelsPerMeter{0};

    int32_t yPixelsPerMeter{0};

    uint32_t colorsUsed{0};

    uint32_t colorsImportant{0};
};

#pragma pack(pop)

void generateBMP(const char *filename, vector<uint8_t> *p_BD)
{

    int size1 = p_BD->size();

    int size = ceil(size1 / 3);

    int WIDTH = ceil(sqrt(size) / 4) * 4;

    int HEIGHT = ceil((size / WIDTH) / 4) * 4;

    if (WIDTH * HEIGHT < size)

        HEIGHT = HEIGHT + 4;

    BMPFileHeader fileHeader;

    BMPInfoHeader infoHeader;

    infoHeader.size = sizeof(BMPInfoHeader);

    infoHeader.width = WIDTH;

    infoHeader.height = HEIGHT;

    infoHeader.bitCount = 24; // 24-bit BMP

    infoHeader.compression = 0;

    infoHeader.sizeImage = WIDTH * HEIGHT * 3;

    infoHeader.xPixelsPerMeter = 2400;

    infoHeader.yPixelsPerMeter = 2400;

    fileHeader.offsetData = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

    fileHeader.fileSize = fileHeader.offsetData + infoHeader.sizeImage;

    ofstream file(filename, ios::binary);

    if (!file)
    {

        cerr << "Could not open file for writing." << endl;

        return;
    }

    // Write headers

    file.write(reinterpret_cast<const char *>(&fileHeader), sizeof(fileHeader));

    file.write(reinterpret_cast<const char *>(&infoHeader), sizeof(infoHeader));

    // Generate pixel data

    int z = 0;

    for (int y = 0; y < HEIGHT; ++y)
    {

        for (int x = 0; x < WIDTH; ++x)
        {

            uint8_t r = 0;

            uint8_t g = 0;

            uint8_t b = 0;

            if (z < size1)

                r = static_cast<uint8_t>(p_BD->at(z));

            z++;

            if (z < size1)

                g = static_cast<uint8_t>(p_BD->at(z));

            z++;

            if (z < size1)

                b = static_cast<uint8_t>(p_BD->at(z));

            z++;

            // Write pixel in BGR format (BMP uses BGR instead of RGB)

            file.put(b);

            file.put(g);

            file.put(r);
        }
    }

    file.close();
}