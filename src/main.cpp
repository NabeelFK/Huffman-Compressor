#include <iostream>
#include <filesystem>

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

        std::cout << "Compression Complete.\n" << std::endl;

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