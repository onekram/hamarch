#include "archive-writer.h"

template <size_t I, size_t O>
ArchiveWriter<I, O>::ArchiveWriter(const std::string &filename, bool is_new) {
  if (is_new) {
    this->out_ = std::ofstream(filename, std::ios::binary | std::ios::out |
                                             std::ios::trunc);
  } else {
    this->out_ = std::ofstream(filename, std::ios::binary | std::ios::out |
                                             std::ios::app);
  }
}

template <size_t I, size_t O>
void ArchiveWriter<I, O>::PutData(std::ifstream &in) {
  char c;
  while (in.get(c)) {
    out_ << c;
  }
}

template <size_t I, size_t O>
void ArchiveWriter<I, O>::AddArchive(const std::string &path) {
  std::ifstream in = std::ifstream(path, std::ios::binary);
  if (!in.is_open()) {
    std::cerr << "Error: file no found " + path + '\n';
    exit(EXIT_FAILURE);
  }
  PutData(in);
}

template <size_t I, size_t O>
void ArchiveWriter<I, O>::AddFile(const std::string &path) {
  uint64_t file_size = std::ifstream(path, std::ios::ate).tellg();
  std::ifstream in = std::ifstream(path, std::ios::binary);
  if (!in.is_open()) {
    std::cerr << "Error: file no found " + path + '\n';
    exit(EXIT_FAILURE);
  }

  size_t data_size = strlen(path.c_str());
  for (size_t i = 0; i < kFileNameBytes; ++i) {
    if (i < data_size)
      Write(path[i]);
    else
      Write(0);
  }
  Update();

  char *char_file_size = reinterpret_cast<char *>(&file_size);
  for (uint8_t i = 0; i < kFileSizeBytes; ++i) {
    Write(char_file_size[i]);
  }
  Update();

  char byte;
  while (in.get(byte))
    Write(byte);
  Update();
}

template <size_t I, size_t O>
bool ArchiveWriter<I, O>::IsPowerOfTwo(uint32_t num) {
  return ((num - 1) & num) == 0;
}

template <size_t I, size_t O> void ArchiveWriter<I, O>::Close() {
  out_.close();
}

template <size_t I, size_t O> void ArchiveWriter<I, O>::Update() {
  if (block_index_ > 0)
    UpdateBlock();
  if (char_index_ > 0)
    UpdateChar();
}

template <size_t I, size_t O>
std::bitset<O> ArchiveWriter<I, O>::Coding(const std::bitset<I> &bits) {
  std::bitset<O> res;
  uint8_t index = 0;
  for (int i = 0; i < res.size(); ++i) {
    if (!IsPowerOfTwo(i + 1)) {
      res.set(i, bits[index++]);
    }
  }

  std::string str = res.to_string();
  for (int i = 0; i < res.size() - 1; i++) {
    index = i + 1;
    if (IsPowerOfTwo(index))
      continue;
    int mask = 1;
    while (mask <= index) {
      if ((mask & index) != 0) {
        if (res[i])
          res.flip(mask - 1);
      }
      mask <<= 1;
    }
  }
  for (int i = 0; i < res.size() - 1; i++) {
    if (res[i])
      res.flip(res.size() - 1);
  }

  return res;
}

template <size_t I, size_t O> void ArchiveWriter<I, O>::Write(char data) {
  uint8_t mask = 1 << 7;
  for (uint8_t i = 0; i < 8; ++i) {
    last_block_.set(block_index_++, (data & mask) != 0);

    if (block_index_ == I)
      UpdateBlock();
    mask >>= 1;
  }
}

template <size_t I, size_t O> void ArchiveWriter<I, O>::UpdateBlock() {
  block_index_ = 0;
  std::bitset<O> bs = Coding(last_block_);
  WriteChar(bs);
  last_block_.reset();
}

template <size_t I, size_t O>
void ArchiveWriter<I, O>::WriteChar(const std::bitset<O> &bs) {
  for (size_t i = 0; i < bs.size(); ++i) {
    if (bs[i]) {
      last_char_ |= (1 << char_index_);
    }
    char_index_++;
    if (char_index_ == 8) {
      UpdateChar();
    }
  }
}

template <size_t I, size_t O> void ArchiveWriter<I, O>::UpdateChar() {
  out_ << last_char_;
  last_char_ = 0;
  char_index_ = 0;
}

template class ArchiveWriter<11, 16>;
template class ArchiveWriter<26, 32>;
template class ArchiveWriter<57, 64>;
