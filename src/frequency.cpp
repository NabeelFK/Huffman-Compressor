#include <iostream>
#include <fstream>

using namespace std;

int buildFrequencyTable(const string &filename, int freq[256])
{
    std::ifstream file(filename, std::ios::binary);

    if (!file)
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }

    unsigned char byte;

    while (file.read(reinterpret_cast<char *>(&byte), 1))
    {
        freq[byte]++;
    }

    return 0;
}
