#include "archive-reader.h"
#include "archive-writer.h"

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <string>
#include <vector>

const std::string kFirstArchiveName{"archive1.haf32"};
const std::string kSecondArchiveName{"archive2.haf32"};
const std::string kThirdArchiveName{"archive3.haf32"};

const std::string kPathFromDirectory = PATH"/assets/";

const std::string kFirstPicFileName{"1.jpg"};
const std::string kSecondPicFileName{"2.jpg"};
const std::string kFirstTXTFileName{"1.txt"};
const std::string kSecondTXTFileName{"2.txt"};
const std::string kFirstVideoFileName{"1.mp4"};

const std::vector<std::string> kFiles{kFirstPicFileName, kSecondPicFileName,
                                      kFirstTXTFileName, kSecondTXTFileName,
                                      kFirstVideoFileName};
const std::vector<std::string> kArchives{kFirstArchiveName, kSecondArchiveName,
                                         kThirdArchiveName};

namespace fs = std::filesystem;

void RemoveFiles();

class TestArchive : public ::testing::Test {
protected:
  void SetUp() override {
    for (const std::string &file : kFiles) {
      if (!fs::exists(file)) {
        fs::copy_file(kPathFromDirectory + file, file);
      }
    }
  }
  void TearDown() override {
    RemoveFiles();
    for (const std::string &archive : kArchives) {
      if (fs::exists(archive)) {
        fs::remove(archive);
      }
    }
  }
};

void RemoveFiles() {
  for (const std::string &file : kFiles) {
    if (fs::exists(file)) {
      fs::remove(file);
    }
  }
}

bool CompareData(const std::string &filename) {
  std::ifstream file1(filename, std::ios::binary);
  std::ifstream file2(kPathFromDirectory + filename, std::ios::binary);

  char c;
  char b;
  while (!file1.eof() || !file2.eof()) {
    file1.get(c);
    file2.get(b);
    if (c != b)
      return false;
  }
  return file1.eof() == file2.eof();
}

TEST_F(TestArchive, AdditionTest) {
  ArchiveWriter<26, 32> arc_writer(kFirstArchiveName, true);
  arc_writer.AddFile(kFirstPicFileName);
  arc_writer.AddFile(kSecondPicFileName);
  arc_writer.Close();

  ArchiveReader<26, 32> arc_reader(kFirstArchiveName);
  std::vector<std::string> list = arc_reader.GetArchiveList();
  arc_reader.Close();

  ASSERT_EQ(list.size(), 2);
  ASSERT_EQ(list[0], kFirstPicFileName);
  ASSERT_EQ(list[1], kSecondPicFileName);
}

TEST_F(TestArchive, DeletionTest) {
  ArchiveWriter<26, 32> arc_writer(kFirstArchiveName, true);
  arc_writer.AddFile(kFirstPicFileName);
  arc_writer.AddFile(kSecondPicFileName);
  arc_writer.Close();

  ArchiveReader<26, 32> arc_reader(kFirstArchiveName);
  arc_reader.DeleteFiles({kFirstPicFileName});
  std::vector<std::string> list = arc_reader.GetArchiveList();
  arc_reader.Close();

  ASSERT_EQ(list.size(), 1);
  ASSERT_EQ(list[0], kSecondPicFileName);
}

TEST_F(TestArchive, ConcatenationTest) {
  ArchiveWriter<26, 32> arc_writer1(kFirstArchiveName, true);
  arc_writer1.AddFile(kFirstPicFileName);
  arc_writer1.AddFile(kSecondPicFileName);
  arc_writer1.Close();

  ArchiveWriter<26, 32> arc_writer2(kSecondArchiveName, true);
  arc_writer2.AddFile(kFirstTXTFileName);
  arc_writer2.AddFile(kSecondTXTFileName);
  arc_writer2.AddFile(kFirstVideoFileName);
  arc_writer2.Close();

  ArchiveWriter<26, 32> arc_writer3(kThirdArchiveName, true);
  arc_writer3.AddArchive(kFirstArchiveName);
  arc_writer3.AddArchive(kSecondArchiveName);
  arc_writer3.Close();

  ArchiveReader<26, 32> arc_reader(kThirdArchiveName);
  std::vector<std::string> list = arc_reader.GetArchiveList();
  arc_reader.Close();

  ASSERT_EQ(list.size(), 5);
  ASSERT_EQ(list[0], kFirstPicFileName);
  ASSERT_EQ(list[1], kSecondPicFileName);
  ASSERT_EQ(list[2], kFirstTXTFileName);
  ASSERT_EQ(list[3], kSecondTXTFileName);
  ASSERT_EQ(list[4], kFirstVideoFileName);
}

TEST_F(TestArchive, ExtractionAllTest) {
  ArchiveWriter<26, 32> arc_writer1(kFirstArchiveName, true);
  for (const auto &file : kFiles) {
    arc_writer1.AddFile(file);
  }
  arc_writer1.Close();

  RemoveFiles();

  ArchiveReader<26, 32> arc_reader(kFirstArchiveName);
  arc_reader.ExtractFiles();
  arc_reader.Close();
  for (const auto &file : kFiles) {
    ASSERT_TRUE(fs::exists(file));
    ASSERT_TRUE(CompareData(file));
  }
}

TEST_F(TestArchive, ExtractionTest) {
  ArchiveWriter<26, 32> arc_writer1(kFirstArchiveName, true);
  for (const auto &file : kFiles) {
    arc_writer1.AddFile(file);
  }
  arc_writer1.Close();

  RemoveFiles();

  std::vector<std::string> extract_files{kFirstPicFileName, kFirstTXTFileName};
  ArchiveReader<26, 32> arc_reader(kFirstArchiveName);
  arc_reader.ExtractFiles(extract_files);
  arc_reader.Close();
  for (const auto &file : extract_files) {
    ASSERT_TRUE(fs::exists(file));
    ASSERT_TRUE(CompareData(file));
  }
}

TEST_F(TestArchive, ComboTest) {
  ArchiveWriter<26, 32> arc_writer1(kFirstArchiveName, true);
  arc_writer1.AddFile(kFirstPicFileName);
  arc_writer1.AddFile(kSecondPicFileName);
  arc_writer1.Close();

  ArchiveWriter<26, 32> arc_writer2(kSecondArchiveName, true);
  arc_writer2.AddFile(kFirstTXTFileName);
  arc_writer2.AddFile(kSecondTXTFileName);
  arc_writer2.Close();

  RemoveFiles();

  ArchiveWriter<26, 32> arc_writer3(kThirdArchiveName, true);
  arc_writer3.AddArchive(kFirstArchiveName);
  arc_writer3.AddArchive(kSecondArchiveName);
  arc_writer3.Close();

  ArchiveReader<26, 32> arc_reader3(kThirdArchiveName);
  arc_reader3.DeleteFiles({kFirstPicFileName, kFirstTXTFileName});
  std::vector<std::string> list3 = arc_reader3.GetArchiveList();
  arc_reader3.Close();

  ASSERT_EQ(list3.size(), 2);
  ASSERT_EQ(list3[0], kSecondPicFileName);
  ASSERT_EQ(list3[1], kSecondTXTFileName);

  ArchiveReader<26, 32> arc_reader1(kFirstArchiveName);
  arc_reader1.DeleteFiles({kSecondPicFileName});
  arc_reader1.ExtractFiles();
  arc_reader1.Close();

  ASSERT_TRUE(CompareData(kFirstPicFileName));

  ArchiveReader<26, 32> arc_reader2(kSecondArchiveName);
  arc_reader2.DeleteFiles({kSecondTXTFileName});
  arc_reader2.ExtractFiles();
  arc_reader2.Close();

  ASSERT_TRUE(CompareData(kFirstTXTFileName));

  ArchiveWriter<26, 32> arc_writer_app(kThirdArchiveName);
  arc_writer_app.AddFile(kFirstPicFileName);
  arc_writer_app.AddFile(kFirstTXTFileName);
  arc_writer_app.Close();

  ArchiveReader<26, 32> arc_reader_app(kThirdArchiveName);
  std::vector<std::string> list_app = arc_reader_app.GetArchiveList();
  arc_reader_app.Close();

  ASSERT_EQ(list_app.size(), 4);
  ASSERT_EQ(list_app[0], kSecondPicFileName);
  ASSERT_EQ(list_app[1], kSecondTXTFileName);
  ASSERT_EQ(list_app[2], kFirstPicFileName);
  ASSERT_EQ(list_app[3], kFirstTXTFileName);
}
