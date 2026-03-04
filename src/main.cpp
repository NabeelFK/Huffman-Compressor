#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 4) {
        cout << "\nUsage:\n";
        cout << "huff compress <input_file> <output_file>\n";
        cout << "huff decompress <input_file> <output_file>\n\n";
        return 1;
    }

    string command = argv[1];
    string input_file = argv[2];
    string output_file = argv[3];

    if (command == "compress") {
        cout << "Compressing " << input_file << " to " << output_file << "\n";
        return 0;
    } else if (command == "decompress") {
        cout << "Decompressing " << input_file << " to " << output_file << "\n";
        return 0;
    } else {
        cout << "Unknown command: " << command << "\n";
        return 1;
    }



}

int readFileRawBytes(const string& filename, string& data) {
    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }

    unsigned char byte;

    while (file.read(reinterpret_cast<char*>(&byte), 1)) {
        data.push_back(byte);
    }

    return 0;
}

