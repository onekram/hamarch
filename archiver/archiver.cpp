#include "archiver.h"

Archiver::Archiver(const Options& opt) : opt_(opt) {
}

void Archiver::CheckCorrectOpt() {
    std::vector<bool> opt_val {opt_.append, opt_.del, opt_.concatenate, opt_.extract, opt_.list, opt_.create};
    bool res = false;
    for (bool flag : opt_val) {
        if (flag && !res) res = true;
        else if (flag && res) {
            std::cerr << "Error: Use only one command" << std::endl;;
            exit(EXIT_FAILURE);
        }
    }
    if (!res) {
        std::cerr << "Error: No command pass" << std::endl;;
        exit(EXIT_FAILURE);
    }

    if (opt_.block_size != 15 && opt_.block_size != 31) {
        std::cerr << "Error: Use 15 or 31 block size" << std::endl;;
        exit(EXIT_FAILURE);
    }
}

void Archiver::Execute() {
    CheckCorrectOpt();

    if (opt_.create) {
        if (!opt_.archive_filename.length()) {
            std::cerr << "Error: Pass archive filename" << std::endl;;
            exit(EXIT_FAILURE);
        }

        if (opt_.filenames.empty()) {
            std::cerr << "Error: Pass files to compress" << std::endl;;
            exit(EXIT_FAILURE);
        }
        if (opt_.block_size == 15) {
            ArchiveWriter<11, 15> arc(opt_.archive_filename, true);
            for (auto& filename: opt_.filenames)
                arc.AddFile(filename);
            arc.Close();
        } else if (opt_.block_size == 31) {
            ArchiveWriter<26, 31> arc(opt_.archive_filename, true);
            for (auto& filename: opt_.filenames)
                arc.AddFile(filename);
            arc.Close();
        }


    } else if (opt_.list) {
        if (opt_.block_size == 15) {
            ArchiveReader<11, 15> arc(opt_.archive_filename);
            for (auto& filename: arc.GetArchiveList())
                std::cout << filename << std::endl;
            arc.Close();
        } else if (opt_.block_size == 31) {
            ArchiveReader<26, 31> arc(opt_.archive_filename);
            for (auto& filename: arc.GetArchiveList())
                std::cout << filename << std::endl;
            arc.Close();
        }
    }
}

