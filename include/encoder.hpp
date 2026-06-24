#pragma once

#include <string>

#include "huffman_codes.hpp"

void compress(
    const CodeTable &codeTable,
    const int freq[256],
    const std::string &fileInput,
    const std::string &fileOutput
);
