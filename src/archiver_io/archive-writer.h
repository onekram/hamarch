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

template <size_t I, size_t O> class ArchiveWriter {
public:
  explicit ArchiveWriter(const std::string &filename, bool is_new = false);

  void AddFile(const std::string &path);

  void AddArchive(const std::string &path);

  void Close();

  static std::bitset<O> Coding(const std::bitset<I> &bits);

private:
  static const uint8_t kFileNameBytes = 30;
  static const uint8_t kFileSizeBytes = 8;

  std::bitset<I> last_block_{};
  uint8_t block_index_{};

  char last_char_{};
  uint8_t char_index_{};

  std::ofstream out_{};

  [[nodiscard]] static bool IsPowerOfTwo(uint32_t num);

  void Update();
  void Write(char data);
  void UpdateBlock();
  void WriteChar(const std::bitset<O> &bs);
  void UpdateChar();

  void PutData(std::ifstream &in);
};