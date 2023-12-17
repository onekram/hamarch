#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "../archiver_io/archive-writer.h"
#include "../archiver_io/archive-reader.h"

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
    Options(const Options& other) = default;
};

class Archiver {
public:
    explicit Archiver(const Options& opt);
    void Execute();
    void CheckCorrectOpt();
private:
    Options opt_;
};
