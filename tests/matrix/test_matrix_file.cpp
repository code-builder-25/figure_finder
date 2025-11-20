#include "matrix/matrix_io.hpp"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

using namespace figures;
namespace fs = std::filesystem;

class MatrixFileTest : public ::testing::Test {
protected:
  void SetUp() override {
    auto temp_path = fs::temp_directory_path() / "matrix_test_XXXXXX";
    std::string path_str = temp_path.string();

    if (mkdtemp(path_str.data())) {
      temp_dir = path_str;
    }
  }

  void TearDown() override {
    if (fs::exists(temp_dir)) {
      fs::remove_all(temp_dir);
    }
  }

  std::string createTestFile(const std::string& name, const std::string& content) {
    std::string filepath = temp_dir + "/" + name;
    std::ofstream file(filepath);
    file << content;
    file.close();
    return filepath;
  }

private:
  std::string temp_dir;
};

TEST_F(MatrixFileTest, LoadBasicMatrix) {
  std::string filepath = createTestFile("basic.txt", "1 0 1\n"
                                                     "0 1 0\n"
                                                     "1 1 1\n");

  BoolMatrix m = loadMatrixFromFile<uint8_t>(filepath);

  EXPECT_EQ(3, m.rows());
  EXPECT_EQ(3, m.cols());

  EXPECT_EQ(1, m(0, 0));
  EXPECT_EQ(0, m(0, 1));
  EXPECT_EQ(1, m(0, 2));
  EXPECT_EQ(0, m(1, 0));
  EXPECT_EQ(1, m(1, 1));
  EXPECT_EQ(0, m(1, 2));
  EXPECT_EQ(1, m(2, 0));
  EXPECT_EQ(1, m(2, 1));
  EXPECT_EQ(1, m(2, 2));
}

TEST_F(MatrixFileTest, LoadSingleElement) {
  std::string filepath = createTestFile("single.txt", "42\n");

  BoolMatrix m = loadMatrixFromFile<uint8_t>(filepath);

  EXPECT_EQ(1, m.rows());
  EXPECT_EQ(1, m.cols());
  EXPECT_EQ(42, m(0, 0));
}

TEST_F(MatrixFileTest, LoadSingleRow) {
  std::string filepath = createTestFile("single_row.txt", "1 0 1 1 0\n");

  BoolMatrix m = loadMatrixFromFile<uint8_t>(filepath);

  EXPECT_EQ(1, m.rows());
  EXPECT_EQ(5, m.cols());
  EXPECT_EQ(1, m(0, 0));
  EXPECT_EQ(0, m(0, 1));
  EXPECT_EQ(1, m(0, 2));
  EXPECT_EQ(1, m(0, 3));
  EXPECT_EQ(0, m(0, 4));
}

TEST_F(MatrixFileTest, LoadSingleColumn) {
  std::string filepath = createTestFile("single_col.txt", "1\n"
                                                          "0\n"
                                                          "1\n"
                                                          "1\n");

  BoolMatrix m = loadMatrixFromFile<uint8_t>(filepath);

  EXPECT_EQ(4, m.rows());
  EXPECT_EQ(1, m.cols());
  EXPECT_EQ(1, m(0, 0));
  EXPECT_EQ(0, m(1, 0));
  EXPECT_EQ(1, m(2, 0));
  EXPECT_EQ(1, m(3, 0));
}

TEST_F(MatrixFileTest, LoadWithVariousWhitespace) {
  std::string filepath = createTestFile("whitespace.txt", "1   0\t1  \n"
                                                          "0\t 1 0\n"
                                                          "1  \t1    1  \t \n");

  BoolMatrix m = loadMatrixFromFile<uint8_t>(filepath);

  EXPECT_EQ(3, m.rows());
  EXPECT_EQ(3, m.cols());
  EXPECT_EQ(1, m(0, 0));
  EXPECT_EQ(0, m(0, 1));
  EXPECT_EQ(1, m(0, 2));
  EXPECT_EQ(0, m(1, 0));
  EXPECT_EQ(1, m(1, 1));
  EXPECT_EQ(0, m(1, 2));
}

TEST_F(MatrixFileTest, LoadWithBlankLines) {
  std::string filepath = createTestFile("blank_lines.txt", "1 0\n"
                                                           "\n"
                                                           "0 1\n"
                                                           "\n");

  BoolMatrix m = loadMatrixFromFile<uint8_t>(filepath);

  EXPECT_EQ(2, m.rows());
  EXPECT_EQ(2, m.cols());
  EXPECT_EQ(1, m(0, 0));
  EXPECT_EQ(0, m(0, 1));
  EXPECT_EQ(0, m(1, 0));
  EXPECT_EQ(1, m(1, 1));
}

TEST_F(MatrixFileTest, LoadIrregularRowsThrows) {
  std::string filepath = createTestFile("irregular.txt", "1 0 1 1\n"
                                                         "0 1\n"
                                                         "1 1 1\n");

  EXPECT_THROW(loadMatrixFromFile<uint8_t>(filepath), std::runtime_error);
}

TEST_F(MatrixFileTest, LoadEmptyFileThrows) {
  std::string filepath = createTestFile("empty.txt", "");

  EXPECT_THROW(loadMatrixFromFile<uint8_t>(filepath), std::runtime_error);
}

TEST_F(MatrixFileTest, LoadNonExistentFileThrows) {
  EXPECT_THROW(loadMatrixFromFile<uint8_t>("/nonexistent/path/file.txt"), std::runtime_error);
}

TEST_F(MatrixFileTest, LoadLargeValues) {
  std::string filepath = createTestFile("large_values.txt", "0 1 255\n"
                                                            "128 64 32\n");

  BoolMatrix m = loadMatrixFromFile<uint8_t>(filepath);

  EXPECT_EQ(2, m.rows());
  EXPECT_EQ(3, m.cols());
  EXPECT_EQ(0, m(0, 0));
  EXPECT_EQ(1, m(0, 1));
  EXPECT_EQ(255, m(0, 2));
  EXPECT_EQ(128, m(1, 0));
  EXPECT_EQ(64, m(1, 1));
  EXPECT_EQ(32, m(1, 2));
}

TEST_F(MatrixFileTest, LoadOutOfRangeValueThrows) {
  std::string filepath = createTestFile("out_of_range.txt", "0 1 257\n"
                                                            "128 64 32\n");

  EXPECT_THROW(loadMatrixFromFile<uint8_t>(filepath), std::runtime_error);
}

TEST_F(MatrixFileTest, LoadNegativeValueThrows) {
  std::string filepath = createTestFile("negative.txt", "0 1 2\n"
                                                        "-1 64 32\n");

  EXPECT_THROW(loadMatrixFromFile<uint8_t>(filepath), std::runtime_error);
}

TEST_F(MatrixFileTest, LoadOnlyWhitespaceThrows) {
  std::string filepath = createTestFile("only_whitespace.txt", "   \n"
                                                               "\t\t\n"
                                                               "  \t  \n");

  EXPECT_THROW(loadMatrixFromFile<uint8_t>(filepath), std::runtime_error);
}

TEST_F(MatrixFileTest, LoadInvalidCharacterThrows) {
  std::string filepath = createTestFile("invalid_char.txt", "1 0 1\n"
                                                            "0 abc 0\n");

  EXPECT_THROW(loadMatrixFromFile<uint8_t>(filepath), std::runtime_error);
}
