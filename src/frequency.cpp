#include <iostream>
#include <fstream>
#include "frequency.hpp"

using namespace std;

int buildFrequencyTable(const string &filename, int freq[256])
{
    // Open file in binary mode
    std::ifstream file(filename, std::ios::binary);

    if (!file)
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }

    unsigned char byte;

    // Read each byte and update its frequency count
    while (file.read(reinterpret_cast<char *>(&byte), 1))
    {
        freq[byte]++;
    }

    return 0;
}
