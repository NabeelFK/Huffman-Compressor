# Huffman Compressor

A lossless file compression and decompression tool written in C++ using Huffman Coding.

This project compresses and decompresses arbitrary files, including text documents, images, PDFs, and other binary files. The compressor preserves the original file extension and reconstructs the original file byte-for-byte during decompression.

## Features

* Lossless Huffman compression
* Lossless Huffman decompression
* Supports arbitrary binary files

  * `.txt`
  * `.jpg`
  * `.png`
  * `.pdf`
  * `.mp4`
  * and more
* Automatic restoration of original file extensions
* Compression statistics reporting
* Header size reporting
* Handles empty files
* Handles single-character files
* Automatic Huffman tree memory cleanup

---

## How It Works

### Compression

1. Read the input file and count the frequency of every byte.
2. Build a Huffman Tree using a priority queue.
3. Generate Huffman codes for each byte.
4. Encode the file into a compressed bit stream.
5. Store metadata in a file header:

   * Original file extension
   * Frequency table
   * Total number of encoded bits
6. Write the compressed output file.

### Decompression

1. Read the header information.
2. Restore the original file extension.
3. Rebuild the Huffman Tree from the stored frequency table.
4. Decode the compressed bit stream.
5. Reconstruct the original file byte-for-byte.

---

## Building

### Requirements

* C++17 or newer
* CMake 3.15+

### Build Instructions

Clone the repository:

```bash
git clone https://github.com/NabeelFK/Huffman-Compressor.git
cd Huffman-Compressor
```

Generate build files:

```bash
cmake -S . -B build
```

Build the project:

```bash
cmake --build build --config Release
```

The executable will be generated in:

```text
build/bin/huff
```

or

```text
build/bin/huff.exe
```

on Windows.

---

## Usage

### Compress a File

```bash
huff compress input.txt
```

Default Output:

```text
compressed.huff
```

Specify a custom output file:

```bash
huff compress input.txt myfile.huff
```

---

### Decompress a File

```bash
huff decompress compressed.huff
```

Default Output:

```text
decompressed.txt
```

The original file extension is automatically restored.

Specify a custom output file:

```bash
huff decompress compressed.huff restored.txt
```

---

## Example Output

```text
Compressing 10MB.txt to compressed.huff

Compression Complete.

Compression Statistics
----------------------
Original Size: 10921739 bytes
Compressed Size: 6268507 bytes
Header Size: 1043 bytes
Savings: 42.61%
```

---

## File Header Format

Each compressed file stores:

```text
[Extension Length]
[Extension]
[Frequency Table]
[Total Encoded Bit Count]
[Compressed Data]
```

The frequency table is used to rebuild the Huffman Tree during decompression.

---

## Testing

The compressor has been tested on:

* Text files
* Large text files (~10 MB)
* Images (.jpg)
* PDF documents
* Empty files
* Single-character files

All decompressed files matched the original input exactly.

---

## Future Improvements

* Graphical user interface using Qt
* Improved header compression
* Human-readable file size formatting
* Batch file compression
* Directory compression support

---

## Author

Nabeel Furqan

Built from scratch in C++ as a personal systems programming project focused on Huffman coding, lossless data compression, binary file formats, tree-based algorithms, memory management, and low-level bit manipulation.
