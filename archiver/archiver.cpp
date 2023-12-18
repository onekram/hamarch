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

    if (opt_.block_size != 16 && opt_.block_size != 32 && opt_.block_size != 64) {
        std::cerr << "Error: Use 16, 32 or 64 block size" << std::endl;;
        exit(EXIT_FAILURE);
    }

    if (!opt_.archive_filename.length()) {
        std::cerr << "Error: Pass archive filename_" << std::endl;;
        exit(EXIT_FAILURE);
    }

    if (!opt_.extract && !opt_.list && opt_.filenames.empty()) {
        std::cerr << "Error: Pass used kFiles" << std::endl;;
        exit(EXIT_FAILURE);
    }
}

void Archiver::Execute() {
    CheckCorrectOpt();

    if (opt_.create) {
        if (opt_.block_size == 16) {
            ArchiveWriter<11, 16> arc(opt_.archive_filename, true);
            for (auto& filename: opt_.filenames)
                arc.AddFile(filename);
            arc.Close();
        } else if (opt_.block_size == 32) {
            ArchiveWriter<26, 32> arc(opt_.archive_filename, true);
            for (auto& filename: opt_.filenames)
                arc.AddFile(filename);
            arc.Close();
        } else {
            ArchiveWriter<57, 64> arc(opt_.archive_filename, true);
            for (auto& filename: opt_.filenames)
                arc.AddFile(filename);
            arc.Close();
        }
    } else if (opt_.list) {
        if (opt_.block_size == 16) {
            ArchiveReader<11, 16> arc(opt_.archive_filename);
            for (auto& filename: arc.GetArchiveList())
                std::cout << filename << std::endl;
            arc.Close();
        } else if (opt_.block_size == 32) {
            ArchiveReader<26, 32> arc(opt_.archive_filename);
            for (auto& filename: arc.GetArchiveList())
                std::cout << filename << std::endl;
            arc.Close();
        } else {
            ArchiveReader<57, 64> arc(opt_.archive_filename);
            for (auto& filename: arc.GetArchiveList())
                std::cout << filename << std::endl;
            arc.Close();
        }
    } else if (opt_.concatenate) {
        if (opt_.block_size == 16) {
            ArchiveWriter<11, 16> arc(opt_.archive_filename, true);
            for (auto& filename: opt_.filenames)
                arc.AddArchive(filename);
            arc.Close();
        } else if (opt_.block_size == 32) {
            ArchiveWriter<26, 32> arc(opt_.archive_filename, true);
            for (auto& filename: opt_.filenames)
                arc.AddArchive(filename);
            arc.Close();
        } else {
            ArchiveWriter<57, 64> arc(opt_.archive_filename, true);
            for (auto& filename: opt_.filenames)
                arc.AddArchive(filename);
            arc.Close();
        }
    } else if (opt_.append) {
        if (opt_.block_size == 16) {
            ArchiveWriter<11, 16> arc(opt_.archive_filename, false);
            for (auto& filename: opt_.filenames)
                arc.AddFile(filename);
            arc.Close();
        } else if (opt_.block_size == 32) {
            ArchiveWriter<26, 32> arc(opt_.archive_filename, false);
            for (auto& filename: opt_.filenames)
                arc.AddFile(filename);
            arc.Close();
        } else {
            ArchiveWriter<57, 64> arc(opt_.archive_filename, false);
            for (auto& filename: opt_.filenames)
                arc.AddFile(filename);
            arc.Close();
        }
    } else if (opt_.extract) {
        if (opt_.block_size == 16) {
            ArchiveReader<11, 16> arc(opt_.archive_filename);
            if (opt_.filenames.empty())
                arc.ExtractFiles();
            else
                arc.ExtractFiles(opt_.filenames);
            arc.Close();
        } else if (opt_.block_size == 32) {
            ArchiveReader<26, 32> arc(opt_.archive_filename);
            if (opt_.filenames.empty())
                arc.ExtractFiles();
            else
                arc.ExtractFiles(opt_.filenames);
            arc.Close();
        } else {
            ArchiveReader<57, 64> arc(opt_.archive_filename);
            if (opt_.filenames.empty())
                arc.ExtractFiles();
            else
                arc.ExtractFiles(opt_.filenames);
            arc.Close();
        }
    } else if (opt_.del) {
        if (opt_.block_size == 16) {
            ArchiveReader<11, 16> arc(opt_.archive_filename);
            arc.DeleteFiles(opt_.filenames);
            arc.Close();
        } else if (opt_.block_size == 32) {
            ArchiveReader<26, 32> arc(opt_.archive_filename);
            arc.DeleteFiles(opt_.filenames);
            arc.Close();
        } else {
            ArchiveReader<57, 64> arc(opt_.archive_filename);
            arc.DeleteFiles(opt_.filenames);
            arc.Close();
        }
    }
}
