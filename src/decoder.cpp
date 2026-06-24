#include <iostream>
#include <fstream>
#include <string>

#include "huffman_tree.hpp"

void decompress(
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

    unsigned char byte;
    std::size_t totalBits = 0;

    int freq[256] = {0};

    // Read frequencies of all bits in compressed file
    for (int i = 0; i < 256; ++i) {
        input.read(reinterpret_cast<char *>(&freq[i]), sizeof(int));
    }

    // Read totalBits in compressed file
    input.read(reinterpret_cast<char *>(&totalBits), sizeof(totalBits));
    
    // Rebuild the tree for decoding
    Node* root = buildHuffmanTree(freq);

    if (root == nullptr) {
        return;
    }

    // Handle edgecase if only 1 char type is in the decompressed file
    if (root->isLeaf()) {
        for (int i = 0; i < freq[root->byte]; ++i) {
            output.write(reinterpret_cast<const char *>(&root->byte), sizeof(root->byte));
        }
        return;
    }

    Node* currentNode = root;
    std::size_t bitsProcessed = 0;

    while (input.read(reinterpret_cast<char *>(&byte), sizeof(byte))) {

        // Move current bit to rightmost position in byte and extract
        for (int i = 0; i < 8; ++i) {
            // Move bit to end of byte
            int bit = byte >> (7 - i) & 1;

            if (bitsProcessed >= totalBits) {
                return;
            }

            // Traverse huffman tree
            if (bit == 0) {
                currentNode = currentNode->left;
            } else {
                currentNode = currentNode->right;
            }

            bitsProcessed++;

            if (currentNode->isLeaf()) {
                output.write(reinterpret_cast<const char *>(&currentNode->byte), sizeof(currentNode->byte));
                currentNode = root;
                continue;
            }

        }

    }
}