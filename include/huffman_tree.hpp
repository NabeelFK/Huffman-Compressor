#pragma once

struct Node {
    unsigned char byte;
    int frequency;

    Node* left;
    Node* right;

    // Constructor for leaf node
    Node(unsigned char b, int freq);

    // Constructor for internal node
    Node(Node* leftChild, Node* rightChild);

    bool isLeaf() const;
};

Node* buildHuffmanTree(int freq[256]);