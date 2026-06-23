#pragma once

#include <array>
#include <string>

#include "huffman_tree.hpp"

using CodeTable = std::array<std::string, 256>;

CodeTable generateCodes(Node* root);

void generateCodesHelper(
    Node* node,
    const std::string& currentCode,
    CodeTable& codes
);