#include "compressor.hpp"

#include "frequency.hpp"
#include "huffman_tree.hpp"
#include "huffman_codes.hpp"
#include "encoder.hpp"
#include "decoder.hpp"
#include "utils.hpp"

#include <filesystem>

CompressionStats runCompression(
    const std::string &inputFile,
    const std::string &outputFile)
{
    int freq[256] = {0};

    if (buildFrequencyTable(inputFile, freq) != 0)
    {
        return {
            false,
            0,
            0,
            0,
            0.0
        };
    }

    Node *root = buildHuffmanTree(freq);

    CodeTable codes = generateCodes(root);

    compress(
        codes,
        freq,
        inputFile,
        outputFile);

    std::uintmax_t originalSize = std::filesystem::file_size(inputFile);

    std::uintmax_t compressedSize = std::filesystem::file_size(outputFile);

    std::string extension = getFileExtension(inputFile);

    std::uintmax_t headerSize =
        sizeof(std::size_t) +
        extension.length() +
        (256 * sizeof(int)) +
        sizeof(std::size_t);

    double savings = 0.0;

    if (originalSize > 0)
    {
        savings = 100.0 *
                  (1.0 -
                   static_cast<double>(compressedSize) /
                       static_cast<double>(originalSize));
    }

    deleteTree(root);

    return {
        true,
        originalSize,
        compressedSize,
        headerSize,
        savings};
}

bool runDecompression(
    const std::string &inputFile,
    const std::string &outputFile)
{
    return decompress(
        inputFile,
        outputFile
    );
}