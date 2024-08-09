#include <iostream>
#include <vector>
#include <cstring>
#include "LzmaDec.h"
#include "LzmaEnc.h"
#include "LzmaLib.h"

void TestLzma() {
    // Initialize LZMA stream
    CLzmaEncProps props;
    CLzmaEncProps_Init(&props);

    // Test setting and initializing encoder
    CCodecs->SetEncoderProps(&props);

    // Initialize compression and decompression
    LzmaStream stream = LZMA_STREAM_INIT;

    if (LzmaEnc_Init(&stream, &props) != LZMA_OK) {
        std::cerr << "LZMA encoder initialization failed." << std::endl;
        return;
    }

    std::cout << "LZMA headers and functions are accessible." << std::endl;

    // Clean up
    LzmaEnc_End(&stream);
}

int main() {
    TestLzma();
    return 0;
}
