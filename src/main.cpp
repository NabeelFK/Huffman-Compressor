#include <iostream>
#include "frequency.hpp"

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cout << "\nUsage:\n";
        std::cout << "huff compress <input_file> <output_file>\n";
        std::cout << "huff decompress <input_file> <output_file>\n\n";
        return 1;
    }

    string command = argv[1];
    string input_file = argv[2];
    string output_file = argv[3];

    if (command == "compress")
    {
        cout << "Compressing " << input_file << " to " << output_file << "\n";
        // int freq[256] = {0};
        // buildFrequencyTable(input_file, freq);

        // for (int i = 0; i < 256; i++) {
        //     if (freq[i] > 0) {
        //         cout << i << " : " << freq[i] << endl;
        //     }
        // }
        return 0;
    }
    else if (command == "decompress")
    {
        cout << "Decompressing " << input_file << " to " << output_file << "\n";
        return 0;
    }
    else
    {
        cout << "Unknown command: " << command << "\n";
        return 1;
    }
}