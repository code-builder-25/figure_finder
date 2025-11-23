#include "finders/figure_finder_factory.hpp"
#include "matrix/matrix.hpp"
#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <vector>

using namespace figures;

class AllFindersTest : public ::testing::TestWithParam<FigureFinderType> {};

INSTANTIATE_TEST_SUITE_P(AllFinders, AllFindersTest, ::testing::ValuesIn(getAllFigureFinderTypes()),
                         [](const ::testing::TestParamInfo<FigureFinderType>& info) {
                           return toString(info.param);
                         });

TEST_P(AllFindersTest, TestEmptyMatrix) {
  BoolMatrix empty(0, 0);
  auto result = createFigureFinder(GetParam())->countFigures(empty);
  EXPECT_EQ(result, 0) << "Empty matrix should have 0 figures";
}

TEST_P(AllFindersTest, TestAllZeros) {
  BoolMatrix zeros(5, 5, 0);
  auto result = createFigureFinder(GetParam())->countFigures(zeros);
  EXPECT_EQ(result, 0) << "All zeros matrix should have 0 figures";
}

TEST_P(AllFindersTest, TestAllOnes) {
  BoolMatrix ones(3, 3, 1);
  auto result = createFigureFinder(GetParam())->countFigures(ones);
  EXPECT_EQ(result, 1) << "All ones matrix should have 1 figure";
}

TEST_P(AllFindersTest, TestLShape) {
  // clang-format off
  std::vector<std::vector<uint8_t>> data = {
    {1, 0, 0},
    {1, 0, 0},
    {1, 1, 1}};
  // clang-format on
  BoolMatrix matrix(data);

  auto result = createFigureFinder(GetParam())->countFigures(matrix);
  EXPECT_EQ(result, 1) << "L-shape should be 1 connected figure";
}

TEST_P(AllFindersTest, TestFourCorners) {
  // clang-format off
  std::vector<std::vector<uint8_t>> data = {
    {1, 0, 1},
    {0, 0, 0},
    {1, 0, 1}};
  // clang-format on
  BoolMatrix matrix(data);

  auto result = createFigureFinder(GetParam())->countFigures(matrix);
  EXPECT_EQ(result, 4) << "Four separate corners should be 4 figures";
}

TEST_P(AllFindersTest, TestPlusSign) {
  // clang-format off
  std::vector<std::vector<uint8_t>> data = {
    {0, 1, 0},
    {1, 1, 1},
    {0, 1, 0}};
  // clang-format on
  BoolMatrix matrix(data);

  auto result = createFigureFinder(GetParam())->countFigures(matrix);
  EXPECT_EQ(result, 1) << "Plus sign should be 1 connected figure";
}

TEST_P(AllFindersTest, TestDiagonal) {
  // clang-format off
  std::vector<std::vector<uint8_t>> data = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}};
  // clang-format on
  BoolMatrix matrix(data);

  auto result = createFigureFinder(GetParam())->countFigures(matrix);
  EXPECT_EQ(result, 3) << "Diagonal cells are not connected";
}

TEST_P(AllFindersTest, TestHorizontalLine) {
  // clang-format off
  std::vector<std::vector<uint8_t>> data = {
    {1, 1, 1, 1, 1}};
  // clang-format on
  BoolMatrix matrix(data);

  auto result = createFigureFinder(GetParam())->countFigures(matrix);
  EXPECT_EQ(result, 1) << "Horizontal line should be 1 connected figure";
}

TEST_P(AllFindersTest, TestVerticalLine) {
  // clang-format off
  std::vector<std::vector<uint8_t>> data = {
    {1},
    {1},
    {1},
    {1},
    {1}};
  // clang-format on
  BoolMatrix matrix(data);

  auto result = createFigureFinder(GetParam())->countFigures(matrix);
  EXPECT_EQ(result, 1) << "Vertical line should be 1 connected figure";
}

TEST_P(AllFindersTest, TestSingleCell) {
  // clang-format off
  std::vector<std::vector<uint8_t>> data = {
    {0, 0, 0},
    {0, 1, 0},
    {0, 0, 0}};
  // clang-format on
  BoolMatrix matrix(data);

  auto result = createFigureFinder(GetParam())->countFigures(matrix);
  EXPECT_EQ(result, 1) << "Single cell should be 1 figure";
}

TEST_P(AllFindersTest, TestTShape) {
  // clang-format off
  std::vector<std::vector<uint8_t>> data = {
    {1, 1, 1},
    {0, 1, 0},
    {0, 1, 0}};
  // clang-format on
  BoolMatrix matrix(data);

  auto result = createFigureFinder(GetParam())->countFigures(matrix);
  EXPECT_EQ(result, 1) << "T-shape should be 1 connected figure";
}

TEST_P(AllFindersTest, TestThreeNestedFigures) {
  // clang-format off
  std::vector<std::vector<uint8_t>> data = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
  };
  // clang-format on
  BoolMatrix matrix(data);

  auto result = createFigureFinder(GetParam())->countFigures(matrix);
  EXPECT_EQ(result, 3) << "There should be 3 nested figures";
}

TEST_P(AllFindersTest, TestBranchingFigure) {
  // clang-format off
  std::vector<std::vector<uint8_t>> data = {
    {0, 1, 0, 1, 1},
    {1, 1, 0, 1, 0},
    {0, 1, 1, 1, 0},
    {0, 0, 0, 1, 0},
    {1, 1, 1, 1, 0}};
  // clang-format on
  BoolMatrix matrix(data);

  auto result = createFigureFinder(GetParam())->countFigures(matrix);
  EXPECT_EQ(result, 1) << "There should be 1 branching figure";
}

TEST_P(AllFindersTest, TestMultipleFigures) {
  // clang-format off
  std::vector<std::vector<uint8_t>> data = {
    {1, 0, 0, 1, 1},
    {1, 0, 0, 0, 0},
    {0, 0, 1, 1, 0},
    {0, 0, 0, 0, 0},
    {1, 1, 0, 0, 1}};
  // clang-format on
  BoolMatrix matrix(data);

  auto result = createFigureFinder(GetParam())->countFigures(matrix);
  EXPECT_EQ(result, 5) << "There should be 5 distinct figures";
}

TEST_P(AllFindersTest, TestCheckerboardMatrix) {
  const size_t rows = 100;
  const size_t cols = 100;
  BoolMatrix matrix(rows, cols, 0);

  // Create a checkerboard pattern
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      if ((i + j) % 2 == 0) {
        matrix(i, j) = 1;
      }
    }
  }

  auto result = createFigureFinder(GetParam())->countFigures(matrix);
  EXPECT_EQ(result, (rows * cols + 1) / 2)
      << "Checkerboard pattern should have half as many figures";
}
