#include <iostream>
#include "frequency.hpp"
#include "huffman_tree.hpp"

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

    }
    else if (command == "decompress")
    {
        std::cout << "Decompressing " << input_file << " to " << output_file << "\n";
        return 0;
    }
    else
    {
        std::cout << "Unknown command: " << command << "\n";
        return 1;
    }
}