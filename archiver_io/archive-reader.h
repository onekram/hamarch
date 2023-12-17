#pragma once

#include <vector>
#include <string>
#include <cinttypes>
#include <bitset>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <iostream>


template <size_t I, size_t O>
class ArchiveReader {
public:
    explicit ArchiveReader(const std::string& filename);

    std::vector<std::string> GetArchiveList();

    void GetData();

    void Close();

private:
    static const size_t kFileNameBytes = 30;
    static const size_t kFileSizeBytes = 8;

    std::bitset<O> last_block_{};
    size_t block_index_{};

    char read_char_{};
    size_t read_char_index_{};

    char last_char_{};
    size_t char_index_{};

    std::bitset<I> last_converted_block_{};
    size_t last_converted_block_index_{};

    std::ifstream in_{};


    [[nodiscard]] static bool IsPowerOfTwo(uint32_t num);

    [[nodiscard]] static uint64_t Convert(uint64_t size);

    std::bitset<I> Decoding(std::bitset<O>& bits);

    char Read();

    char UpdateBlock();

    char ReadChar();

    char UpdateChar();

    void Update();

    uint64_t GetValFromChar(unsigned char data);

    uint64_t GetSizeFromChar(const unsigned char* data);

    uint64_t GetSize();

    std::string GetName();

    std::string PrintChar(char data);

};
