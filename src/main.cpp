#include <iostream>
#include <iomanip>

#include "decoder.hpp"
#include "compressor.hpp"
#include "utils.hpp"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "\nUsage:\n";
        std::cout << "huff compress <input_file> optional:<output_file>\n";
        std::cout << "huff decompress <input_file> optional:<output_file>\n\n";
        return 1;
    }

    std::string command = argv[1];
    std::string input_file = argv[2];
    std::string output_file = "";
    if (argc >= 4)
    {
        output_file = argv[3];
    }
    else if (command == "compress")
    {
        output_file = getAvailableFileName(
            "compressed",
            "huff"
        );
    }
    else
    {
        std::string extension = getStoredExtension(input_file);

        output_file = getAvailableFileName(
            "decompressed",
            extension
        );

    }

    if (command == "compress")
    {
        std::cout << "\nCompressing " << input_file << " to " << output_file << "\n";

        CompressionStats stats = runCompression(input_file, output_file);

        if (!stats.success) {
            return 1;
        }

        std::cout << "\nCompression Complete.\n";

        std::cout << "\nCompression Statistics\n";
        std::cout << "----------------------\n";

        std::cout << "Original Size:      " << formatFileSize(stats.originalSize) << "\n";

        std::cout << "Compressed Size:    " << formatFileSize(stats.compressedSize) << "\n";

        std::cout << "Header Size:        " << formatFileSize(stats.headerSize) << "\n";

        std::cout << "Savings: " << std::fixed << std::setprecision(2) << stats.savings << "%\n\n";

    }
    else if (command == "decompress")
    {
        std::cout << "\nDecompressing " << input_file << " to " << output_file << "\n";

        if (!runDecompression(input_file, output_file)) {
            return 1;
        }

        std::cout << "Decompression complete.\n"
                  << std::endl;
    }
    else
    {
        std::cout << "Unknown command: " << command << "\n";
        return 1;
    }
}