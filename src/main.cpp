#include <iostream>
#include "frequency.hpp"
#include "huffman_tree.hpp"
#include "huffman_codes.hpp"
#include "encoder.hpp"
#include "decoder.hpp"

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cout << "\nUsage:\n";
        std::cout << "huff compress <input_file> <output_file>\n";
        std::cout << "huff decompress <input_file> <output_file>\n\n";
        return 1;
    }

    std::string command = argv[1];
    std::string input_file = argv[2];
    std::string output_file = argv[3];

    if (command == "compress")
    {
        std::cout << "Compressing " << input_file << " to " << output_file << "\n";
        
        int freq[256] = {0};

        if (buildFrequencyTable(input_file, freq) != 0) {
            return 1;
        }

        Node* root = buildHuffmanTree(freq);

        CodeTable codes = generateCodes(root);

        std::cout << "\nGenerated Huffman Codes:\n";

        for (int i = 0; i < 256; i++) {
            if (!codes[i].empty()) {
                std::cout
                << static_cast<char>(i)
                << " -> "
                << codes[i]
                << std::endl;
            }
        }

        compress(
            codes,
            freq,
            input_file,
            output_file
        );

        std::cout << "Compression Complete" << std::endl;
    }
    else if (command == "decompress")
    {
        std::cout << "Decompressing " << input_file << " to " << output_file << "\n";

        int freq[256] = {0};

        if (buildFrequencyTable(input_file, freq) != 0) {
            return 1;
        }

        Node* root = buildHuffmanTree(freq);

        CodeTable codes = generateCodes(root);

        decompress(
            input_file,
            output_file
        );
    }
    else
    {
        std::cout << "Unknown command: " << command << "\n";
        return 1;
    }
}