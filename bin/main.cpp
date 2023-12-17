#include <iostream>



#include "lib/ArgParser.h"

#include "archiver/archiver.h"



int main(int argc, char** argv) {
    Options opt;

    ArgumentParser::ArgParser parser("HamArc");
    parser.AddFlag('c', "create", "Create new archive").StoreValue(opt.create);
    parser.AddFlag('l', "list", "List files from archive").StoreValue(opt.list);
    parser.AddFlag('x', "extract", "Extract files from archive").StoreValue(opt.extract);
    parser.AddFlag('a', "append", "Add file to archive").StoreValue(opt.append);
    parser.AddFlag('d', "delete", "Delete file from archive").StoreValue(opt.del);
    parser.AddFlag('A', "concatenate", "Merge two archives to third archive").StoreValue(opt.concatenate);
    parser.AddStringArgument('f', "file", "ArchiveWriter file").StoreValue(opt.archive_filename);
    parser.AddStringArgument("files", "Files as arguments").Default("DEFAULT").MultiValue(1).Positional().StoreValues(opt.filenames);
    parser.AddIntArgument('b', "blocksize", "Block size (15/31)").StoreValue(opt.block_size).Default(15);
    if(!parser.Parse(argc, argv)) {
        std::cerr << "Error: Wrong arguments" << std::endl;
        std::cout << parser.HelpDescription() << std::endl;
        return 1;
    }

    if(parser.Help()) {
        std::cout << parser.HelpDescription() << std::endl;
        return 0;
    }

    Archiver archiver(opt);
    archiver.Execute();

//    auto arch_i = ArchiveWriter<26, 31>(opt.archive_filename, true);
//    for (auto& filename : opt.filenames)
//        arch_i.AddFile(filename);
//    arch_i.Close();
//
//    auto arch_o = ArchiveReader<26, 31>(opt.archive_filename);
//    for (auto& filename : arch_o.GetArchiveList())
//        std::cout << filename << std::endl;
//    arch_o.Close();

}

