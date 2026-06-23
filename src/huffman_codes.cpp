#include "huffman_tree.hpp"
#include "huffman_codes.hpp"


CodeTable generateCodes(Node* root) {
    CodeTable codeTable{{}};

    if (root == nullptr) {
        return codeTable;
    }

    if (root->isLeaf()) {
        generateCodesHelper(root, "0", codeTable);
    } else {
        generateCodesHelper(
            root,
            "",
            codeTable
        );
    }

    return codeTable;
}

void generateCodesHelper(
    Node* node,
    const std::string& currentCode,
    CodeTable& codes
) {
    if (node->isLeaf()) {
        codes[node->byte] = currentCode;
        return;
    }

    if (node->left) {
        generateCodesHelper(node->left, currentCode + "0", codes);
    }
    if (node->right) {
        generateCodesHelper(node->right, currentCode + "1", codes);
    }
}