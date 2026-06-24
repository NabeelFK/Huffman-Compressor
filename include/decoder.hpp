#pragma once

#include <string>

std::string getStoredExtension (
    const std::string &fileInput
);

void decompress(
    const std::string &fileInput,
    const std::string &fileOutput
);