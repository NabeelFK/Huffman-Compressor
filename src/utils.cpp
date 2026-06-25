#include "utils.hpp"

#include <fstream>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <filesystem>


std::string getFileExtension(
    const std::string &fileName
) {
    std::size_t dotPos = fileName.find_last_of('.');

    if (dotPos == std::string::npos)
    {
        return "";
    }

    return fileName.substr(dotPos + 1);
}


// Helper to format to more readable file and header sizes 
std::string formatFileSize(std::uintmax_t bytes)
{

    std::ostringstream output;

    if (bytes >= 1024 * 1024)
    {
        output << std::fixed << std::setprecision(2)
               << static_cast<double>(bytes) / (1024.0 * 1024.0)
               << " MB";
    }
    else if (bytes >= 1024)
    {
        output << std::fixed << std::setprecision(2)
               << static_cast<double>(bytes) / 1024.0 << " KB";
    }
    else
    {
        output << bytes << " bytes";
    }

    return output.str();
}


std::string getStoredExtension(
    const std::string &fileInput
) {
    std::ifstream input(fileInput, std::ios::binary);

    if (!input) {
        std::cerr << "Error opening file: " << fileInput << std::endl;
        return "";
    }

   std::size_t extensionLen;

    // Read extension length
    input.read(reinterpret_cast<char *>(&extensionLen), sizeof(extensionLen));

    std::string extension(extensionLen, '\0');

    // Read extension type
    if (extensionLen > 0) {
        input.read(&extension[0], extensionLen);
    }

    return extension;

}

std::string getAvailableFileName(
    const std::string &baseName,
    const std::string &extension
) {
    std::string fileName = baseName;

    if (!extension.empty()) {
        fileName += "." + extension;
    }

    if (!std::filesystem::exists(fileName)) {
        return fileName;
    }

    std::size_t counter = 1;

    while (true) {
        fileName = baseName + std::to_string(counter);

        if (!extension.empty()) {
            fileName += "." + extension;
        }

        if (!std::filesystem::exists(fileName)) {
            return fileName;
        }

        counter++;
    }
}