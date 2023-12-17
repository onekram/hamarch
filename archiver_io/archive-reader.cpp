
#include <sstream>
#include "archive-reader.h"


static const int kByteBits = 8;

template <size_t I, size_t O>
ArchiveReader<I, O>::ArchiveReader(const std::string& filename) {
    this->in_ = std::ifstream(filename, std::ios::binary | std::ios::in);
    if (!in_.is_open()) {
        std::cerr << "Error: file no found " + filename + '\n';
        exit(EXIT_FAILURE);
    }
}

template <size_t I, size_t O>
std::string ArchiveReader<I, O>::GetName() {
//    std::cout << "------name-------" << std::endl;

    std::stringstream ss;
    char c;
    for (size_t i = 0; i < kFileNameBytes; ++i) {
        c = Read();
//        std::cout << (i + 1) << ") " <<  GetValFromChar(c) << " - " << PrintChar(c) << " - " << c << std::endl;
        if (c)
            ss << c;
    }
    Update();
    return ss.str();
}

template <size_t I, size_t O>
uint64_t ArchiveReader<I, O>::GetSize() {
//    std::cout << "------size-------" << std::endl;
    unsigned char data[kFileSizeBytes];
    size_t index = 1;
    for (unsigned char& c : data) {
        c = Read();
//        std::cout << index++  << ") " <<  GetValFromChar(c) << " - " << PrintChar(c) << " - " << c << std::endl;
    }
    Update();
    return GetSizeFromChar(data);
}

template <size_t I, size_t O>
void ArchiveReader<I, O>::GetData() {
    char c;
    for (size_t i = 0; i < 24; ++i) {
        c = Read();
//        std::cout << (i + 1)  << ") " <<  GetValFromChar(c) << " - " << PrintChar(c) << " - " << c << std::endl;
    }
    Update();
}


template <size_t I, size_t O>
std::vector<std::string> ArchiveReader<I, O>::GetArchiveList() {
    std::vector<std::string> res;
    uint64_t shift = Convert(kFileNameBytes);
    in_.seekg(shift);
    char c;
    while (in_.get(c)) {
        in_.seekg(-1, std::ios::cur);
        res.push_back(GetName());
        in_.seekg(GetSize(), std::ios::cur);
    }
    return res;
}

template <size_t I, size_t O>
uint64_t ArchiveReader<I, O>::GetSizeFromChar(const unsigned char* data) {
    uint64_t res = 0;
    for (uint64_t i = 0; i < 8; ++i) {
        res += (static_cast<uint64_t>(data[i]) << (i * 8));
    }
    return res;
}

template <size_t I, size_t O>
std::string ArchiveReader<I, O>::PrintChar(char data) {
    std::stringstream ss;
    uint8_t mask = 1 << 7;
    for (uint8_t i = 0; i < kByteBits; ++i) {
        ss << ((data & mask) != 0);
        mask >>= 1;
    }
    return ss.str();
}

template <size_t I, size_t O>
uint64_t ArchiveReader<I, O>::GetValFromChar(unsigned char data) {
    return static_cast<uint64_t>(data);
}

template <size_t I, size_t O>
bool ArchiveReader<I, O>::IsPowerOfTwo(uint32_t num) {
    return ((num - 1) & num) == 0;
}

template <size_t I, size_t O>
uint64_t ArchiveReader<I, O>::Convert(uint64_t size) {
    return std::ceil((std::ceil(((double)size * 8.0) / I) * O) / 8.0);
}

template <size_t I, size_t O>
std::bitset<I> ArchiveReader<I, O>::Decoding(std::bitset<O>& bits) {
    std::bitset<I> res;
    uint32_t error_index = 0;
    size_t index;
    for (size_t i = 0; i < bits.size(); ++i) {
        index = i + 1;
        if (!bits[i]) continue;
        int mask = 1;
        while (mask <= index) {
            if ((mask & index) != 0) {
                error_index ^= mask;
            }
            mask <<= 1;
        }
    }

    index = 0;
    for (int i = 0; i < bits.size(); ++i) {
        if (IsPowerOfTwo(i + 1)) continue;
        res[index++] = (i != error_index - 1) == bits[i];
    }
    return res;
}

template <size_t I, size_t O>
void ArchiveReader<I, O>::Update() {
    last_block_.reset();
    block_index_ = 0;

    read_char_ = 0;
    read_char_index_ = 0;

    last_char_ = 0;
    char_index_ = 0;

    last_converted_block_.reset();
    last_converted_block_index_ = 0;
}

template <size_t I, size_t O>
char ArchiveReader<I, O>::Read() {
    if (last_converted_block_index_ > 0)
        return ReadChar();

    if (!read_char_index_)
        in_.get(read_char_);
    uint8_t mask = 1;
    for (; read_char_index_ < kByteBits; ++read_char_index_) {
        last_block_.set(block_index_++, (read_char_ & (mask << read_char_index_)) != 0);
        if (block_index_ == O) {
            if (read_char_index_ == kByteBits - 1) {
                read_char_index_ = 0;
                mask = 1;
            }
            else {
                read_char_index_++;
            }
            return UpdateBlock();
        }
    }
    read_char_index_ = 0;
    return Read();
}

template <size_t I, size_t O>
char ArchiveReader<I, O>::UpdateBlock() {
    block_index_ = 0;
    last_converted_block_ = Decoding(last_block_);
    last_block_.reset();
    return ReadChar();
}

template <size_t I, size_t O>
char ArchiveReader<I, O>::ReadChar() {
    for (; last_converted_block_index_ < last_converted_block_.size(); ++last_converted_block_index_) {
        if (last_converted_block_[last_converted_block_index_]) {
            last_char_ |= (1 << (kByteBits - char_index_ - 1));
        }
        char_index_++;
        if (char_index_ == kByteBits) {
            if (last_converted_block_index_ == last_converted_block_.size() - 1) {
                last_converted_block_index_ = 0;
            } else {
                last_converted_block_index_++;
            }
            return UpdateChar();
        }
    }
    last_converted_block_index_ = 0;
    return Read();
}
template <size_t I, size_t O>
char ArchiveReader<I, O>::UpdateChar() {
    char res = last_char_;
    last_char_ = 0;
    char_index_ = 0;
    return res;
}

template<size_t I, size_t O>
void ArchiveReader<I, O>::Close() {
    in_.close();
}

template class ArchiveReader<11, 15>;
template class ArchiveReader<26, 31>;

