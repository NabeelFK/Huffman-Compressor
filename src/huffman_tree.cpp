#include <iostream>
#include <stdlib.h>
#include <vector>

#include "huffman_tree.hpp"


// Leaf Node Constructor
Node::Node(unsigned char b, int freq) {
    byte = b;

    frequency = freq;

    left = nullptr;
    right = nullptr;

}


// Internal Node Constructor
Node::Node(Node* leftChild, Node* rightChild) {
    left = leftChild;

    right = rightChild;

    frequency = leftChild->frequency + rightChild->frequency;

    // Internal nodes don't represent a real byte
    byte = '\0';

}


bool Node::isLeaf() const {
    return left == nullptr && right == nullptr;
}


Node* buildHuffmanTree(int freq[256]) {

    std::vector<Node*> leafNodes = {};

    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            Node* newLeaf = new Node(static_cast<unsigned char>(i), freq[i]);
            leafNodes.push_back(newLeaf);
            std::cout << "Created leaf: "
                        << static_cast<int>(newLeaf->byte)
                        << " Frequency: "
                        << newLeaf->frequency
                        << std::endl;
        }
    }

    return nullptr;
}