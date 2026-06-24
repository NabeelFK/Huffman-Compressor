#include <iostream>
#include <filesystem>
#include <iomanip>

#include "frequency.hpp"
#include "huffman_tree.hpp"
#include "huffman_codes.hpp"
#include "encoder.hpp"
#include "decoder.hpp"

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
    if (argc >= 4) {
        output_file = argv[3];
    } else if (command == "compress") {
        output_file = "compressed.huff";
    } else {
        std::string extension = getStoredExtension(input_file);

        output_file = "decompressed";

        if (!extension.empty()) {
            output_file += "." + extension;
        }
    }

    if (command == "compress")
    {
        std::cout << "\nCompressing " << input_file << " to " << output_file << "\n";
        
        int freq[256] = {0};

        if (buildFrequencyTable(input_file, freq) != 0) {
            return 1;
        }

        Node* root = buildHuffmanTree(freq);

        CodeTable codes = generateCodes(root);

        compress(
            codes,
            freq,
            input_file,
            output_file
        );

        std::uintmax_t originalSize = std::filesystem::file_size(input_file);

        std::uintmax_t compressedSize = std::filesystem::file_size(output_file);

        size_t dotPos = input_file.find_last_of('.');

        std::string extension;

        if (dotPos == std::string::npos) {
            extension = "";
        } else {
            extension = input_file.substr(dotPos + 1);
        }

        std::uintmax_t headerSize = 
            sizeof(size_t) + 
            extension.length() + 
            (256 * sizeof(int)) + 
            sizeof(std::size_t);

        double savings = 0.0;

        if (originalSize > 0) {
            savings = 100.0 * 
            (1.0 - 
            static_cast<double>(compressedSize) / 
            static_cast<double>(originalSize)
            );
        }

        std::cout << "\nCompression Complete.\n";

        std::cout << "\nCompression Statistics\n";
        std::cout << "----------------------\n";

        std::cout << "Original Size: " << originalSize << " bytes\n";

        std::cout << "Compressed Size: " << compressedSize << " bytes\n";

        std::cout << "Header Size: " << headerSize << " bytes\n";

        std::cout << "Savings: " << std::fixed << std::setprecision(2) << savings << "%\n\n";

        deleteTree(root);
    }
    else if (command == "decompress")
    {
        std::cout << "\nDecompressing " << input_file << " to " << output_file << "\n";

        decompress(
            input_file,
            output_file
        );

        std::cout << "Decompression complete.\n" << std::endl;
    }
    else
    {
        std::cout << "Unknown command: " << command << "\n";
        return 1;
    }
}