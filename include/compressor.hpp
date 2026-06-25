#pragma once

#include <string>
#include <cstdint>

struct CompressionStats {

    bool success;

    std::uintmax_t originalSize;
    std::uintmax_t compressedSize;
    std::uintmax_t headerSize;

    double savings;
};


CompressionStats runCompression(
    const std::string &inputFile,
    const std::string &outputFile
);

bool runDecompression(
    const std::string &inputFile,
    const std::string &outputFile
);