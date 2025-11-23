#include "finders/figure_finder_factory.hpp"
#include "matrix/matrix.hpp"
#include <gtest/gtest.h>
#include <vector>

using namespace figures;

class AllParallelFindersTest : public ::testing::TestWithParam<FigureFinderType> {};

INSTANTIATE_TEST_SUITE_P(AllParallelFinders, AllParallelFindersTest,
                         ::testing::ValuesIn({FigureFinderType::GCCParallel}),
                         [](const ::testing::TestParamInfo<FigureFinderType>& info) {
                           return toString(info.param);
                         });

TEST_P(AllParallelFindersTest, LargeConnectedComponent) {
  BoolMatrix matrix(200, 200, 1);

  auto result = createFigureFinder(GetParam())->countFigures(matrix);

  EXPECT_EQ(1, result) << "Large all-ones matrix should be 1 figure";
}

TEST_P(AllParallelFindersTest, StressTestManyIterations) {
  // clang-format off
  std::vector<std::vector<uint8_t>> data = {
    {1, 1, 0, 1, 0},
    {0, 1, 0, 1, 1},
    {1, 0, 0, 0, 0},
    {1, 1, 1, 1, 0}};
  // clang-format on

  BoolMatrix matrix(data);
  auto result = createFigureFinder(GetParam())->countFigures(matrix);

  for (int i = 0; i < 100; ++i) {
    EXPECT_EQ(result, 3) << "Failed on iteration " << i;
  }
}

TEST_P(AllParallelFindersTest, ThreadBoundaries) {
  std::vector<std::vector<uint8_t>> data(1000, std::vector<uint8_t>(100));

  // Vertical stripes - ensure connections cross row boundaries
  for (size_t i = 0; i < 1000; ++i) {
    for (size_t j = 0; j < 100; ++j) {
      data[i][j] = (j % 10 < 5) ? 1 : 0;
    }
  }

  BoolMatrix matrix(data);

  auto result = createFigureFinder(GetParam())->countFigures(matrix);
  EXPECT_EQ(result, 10) << "There should be 10 vertical stripe figures";
}
