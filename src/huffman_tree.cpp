#include <iostream>

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