#include <stdlib.h>

#include <vector>
#include <queue>

#include "huffman_tree.hpp"


struct compareNodes {
    bool operator()(Node* a, Node* b) {
        return a->frequency > b->frequency;
    }
};


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

    std::priority_queue<
        Node*,
        std::vector<Node*>,
        compareNodes
    > pq;

    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            Node* newLeaf = new Node(static_cast<unsigned char>(i), freq[i]);
            pq.push(newLeaf);
            std::cout << "Created leaf: "
                        << static_cast<int>(newLeaf->byte)
                        << " Frequency: "
                        << newLeaf->frequency
                        << std::endl;
        }

    }

    if (pq.empty()) {
        return nullptr;
    }

    while (pq.size() > 1) {

        Node* smallest = pq.top();
        pq.pop();

        Node* secondSmallest = pq.top();
        pq.pop();

        Node* newInternal = new Node(smallest, secondSmallest);
        pq.push(newInternal);

    }

    return pq.top();
}