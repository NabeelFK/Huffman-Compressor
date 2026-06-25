#pragma once

#include <string>
#include <cstdint>

std::string getFileExtension(
    const std::string &fileName
);

std::string formatFileSize(
    std::uintmax_t bytes
);

std::string getStoredExtension (
    const std::string &fileInput
);

std::string getAvailableFileName(
    const std::string &baseName,
    const std::string &extension
);