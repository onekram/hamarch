#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "archive-reader.h"
#include "archive-writer.h"

struct Options {
  bool create{};
  bool list{};
  bool extract{};
  bool append{};
  bool del{};
  bool concatenate{};
  std::string archive_filename{};
  std::vector<std::string> filenames{};
  int block_size{};

  Options() = default;
  Options(const Options &other) = default;
};

class Archiver {
public:
  explicit Archiver(const Options &opt);
  void Execute();
  void CheckCorrectOpt();

private:
  Options opt_;
};
