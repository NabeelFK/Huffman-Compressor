#include <iostream>
#include <fstream>
#include <string>

#include "encoder.hpp"

void compress(
    const CodeTable &codeTable,
    const int freq[256],
    const std::string &fileInput,
    const std::string &fileOutput
) {
    // Open input and output files in binary mode
    std::ifstream input(fileInput, std::ios::binary);
    std::ofstream output(fileOutput, std::ios::binary);

    if (!input) {
        std::cerr << "Error opening file: " << fileInput << std::endl;
        return;
    } else if (!output) {
        std::cerr << "Error opening file: " << fileOutput << std::endl;
        return;
    }

    // Total bit count
    std::size_t totalBitCount = 0;

    // Calculate total bits in file
    for (int i = 0; i < 256; ++i) {
        totalBitCount += freq[i] * codeTable[i].length();
    }

    // Add header to output file for decomp
    for (int i = 0; i < 256; ++i) {
        output.write(reinterpret_cast<const char*>(&freq[i]), sizeof(int));
    }

    output.write(reinterpret_cast<const char*>(&totalBitCount), sizeof(totalBitCount));

    
    // Current byte read from input file
    unsigned char byte;
    // Number of bits currently stored in currentByte
    int bitCount = 0;
    // Output byte currently being built
    unsigned char currentByte = 0;

    while (input.read(reinterpret_cast<char *>(&byte), sizeof(byte))) {
        std::string code = codeTable[byte];

        for (char bit : code) {
            
            // Shift byte to the left
            currentByte <<= 1;

            if (bit == '1') {
                // Then:
                // Bitwise OR sets last bit to a 1
                currentByte |= 1;

                // Else:
                // Shifting to the left set bit as 0
            }

            bitCount++;

            if (bitCount == 8) {
                output.write(reinterpret_cast<const char *>(&currentByte), sizeof(currentByte));
                currentByte = 0;
                bitCount = 0;
            }
        }
    }

    // Write remaining bits to output file
    if (bitCount > 0) {
        currentByte <<= 8 - bitCount;
        output.write(reinterpret_cast<const char *>(&currentByte), sizeof(currentByte));
    }

}