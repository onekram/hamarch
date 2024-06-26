#pragma once

#include <algorithm>
#include <bitset>
#include <cinttypes>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

template <size_t I, size_t O> class ArchiveReader {
public:
  explicit ArchiveReader(const std::string &filename);

  std::vector<std::string> GetArchiveList();

  void ExtractFiles();
  void ExtractFiles(const std::vector<std::string> &filenames);

  void DeleteFiles(const std::vector<std::string> &filenames);

  void Close();
  static std::bitset<I> Decoding(std::bitset<O> &bits);

private:
  const size_t kFileNameBytes = 30;
  const size_t kFileSizeBytes = 8;
  const std::string kTemporaryName{"temporary"};

  std::bitset<O> last_block_{};
  size_t block_index_{};

  char read_char_{};
  size_t read_char_index_{};

  char last_char_{};
  size_t char_index_{};

  std::bitset<I> last_converted_block_{};
  size_t last_converted_block_index_{};

  std::ifstream in_{};
  const std::string &filename_;

  [[nodiscard]] static bool IsPowerOfTwo(uint32_t num);
  [[nodiscard]] static uint64_t Convert(uint64_t size);

  char Read();
  char UpdateBlock();
  char ReadChar();
  char UpdateChar();
  void Update();

  uint64_t GetSizeFromChar(const unsigned char *data);
  uint64_t GetSize();
  std::string GetName();

  void PutData(std::ifstream &in, std::fstream &out);
};
