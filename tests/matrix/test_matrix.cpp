#include "matrix/matrix.hpp"
#include <gtest/gtest.h>
#include <vector>

using namespace figures;

TEST(MatrixTest, ConstructorWithDefaultValue) {
  BoolMatrix m(3, 4);
  EXPECT_EQ(3, m.rows());
  EXPECT_EQ(4, m.cols());

  for (size_t i = 0; i < m.rows(); ++i) {
    for (size_t j = 0; j < m.cols(); ++j) {
      EXPECT_EQ(0, m(i, j));
    }
  }
}

TEST(MatrixTest, ConstructorWithNonZeroDefault) {
  BoolMatrix m(2, 3, 1);
  EXPECT_EQ(2, m.rows());
  EXPECT_EQ(3, m.cols());

  for (size_t i = 0; i < m.rows(); ++i) {
    for (size_t j = 0; j < m.cols(); ++j) {
      EXPECT_EQ(1, m(i, j));
    }
  }
}

TEST(MatrixTest, ConstructorFrom2DVector) {
  std::vector<std::vector<uint8_t>> data = {{1, 0, 1}, {0, 1, 0}, {1, 1, 1}};

  BoolMatrix m(data);
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

TEST(MatrixTest, ConstructorFromInitializerList) {
  BoolMatrix m({{1, 0, 1}, {0, 1, 0}});

  EXPECT_EQ(2, m.rows());
  EXPECT_EQ(3, m.cols());

  EXPECT_EQ(1, m(0, 0));
  EXPECT_EQ(0, m(0, 1));
  EXPECT_EQ(1, m(0, 2));
  EXPECT_EQ(0, m(1, 0));
  EXPECT_EQ(1, m(1, 1));
  EXPECT_EQ(0, m(1, 2));
}

TEST(MatrixTest, ElementAccess) {
  BoolMatrix m(2, 2);

  m(0, 0) = 1;
  m(0, 1) = 0;
  m(1, 0) = 1;
  m(1, 1) = 1;

  EXPECT_EQ(1, m(0, 0));
  EXPECT_EQ(0, m(0, 1));
  EXPECT_EQ(1, m(1, 0));
  EXPECT_EQ(1, m(1, 1));
}

TEST(MatrixTest, ConstElementAccess) {
  const BoolMatrix m({{1, 0}, {0, 1}});

  EXPECT_EQ(1, m(0, 0));
  EXPECT_EQ(0, m(0, 1));
  EXPECT_EQ(0, m(1, 0));
  EXPECT_EQ(1, m(1, 1));
}

TEST(MatrixTest, EmptyMatrix) {
  BoolMatrix m(0, 0);
  EXPECT_EQ(0, m.rows());
  EXPECT_EQ(0, m.cols());
}

TEST(MatrixTest, SingleElement) {
  BoolMatrix m(1, 1, 42);
  EXPECT_EQ(1, m.rows());
  EXPECT_EQ(1, m.cols());
  EXPECT_EQ(42, m(0, 0));
}

TEST(MatrixTest, SingleRow) {
  BoolMatrix m(1, 5, 1);
  EXPECT_EQ(1, m.rows());
  EXPECT_EQ(5, m.cols());

  for (size_t j = 0; j < m.cols(); ++j) {
    EXPECT_EQ(1, m(0, j));
  }
}

TEST(MatrixTest, SingleColumn) {
  BoolMatrix m(5, 1, 1);
  EXPECT_EQ(5, m.rows());
  EXPECT_EQ(1, m.cols());

  for (size_t i = 0; i < m.rows(); ++i) {
    EXPECT_EQ(1, m(i, 0));
  }
}

TEST(MatrixTest, LargeMatrix) {
  BoolMatrix m(1000, 1000, 0);
  EXPECT_EQ(1000, m.rows());
  EXPECT_EQ(1000, m.cols());

  m(0, 0) = 1;
  m(999, 999) = 1;
  m(500, 500) = 1;

  EXPECT_EQ(1, m(0, 0));
  EXPECT_EQ(1, m(999, 999));
  EXPECT_EQ(1, m(500, 500));
  EXPECT_EQ(0, m(0, 1));
  EXPECT_EQ(0, m(500, 501));
}

TEST(MatrixTest, CopyConstructor) {
  BoolMatrix m1(3, 3, 1);
  BoolMatrix m2 = m1;

  EXPECT_EQ(m1.rows(), m2.rows());
  EXPECT_EQ(m1.cols(), m2.cols());

  for (size_t i = 0; i < m1.rows(); ++i) {
    for (size_t j = 0; j < m1.cols(); ++j) {
      EXPECT_EQ(m1(i, j), m2(i, j));
    }
  }

  m1(0, 0) = 0;
  EXPECT_EQ(0, m1(0, 0));
  EXPECT_EQ(1, m2(0, 0));
}

TEST(MatrixTest, CopyAssignment) {
  BoolMatrix m1(3, 3, 1);
  BoolMatrix m2(1, 1, 0);
  m2 = m1;

  EXPECT_EQ(m1.rows(), m2.rows());
  EXPECT_EQ(m1.cols(), m2.cols());

  for (size_t i = 0; i < m1.rows(); ++i) {
    for (size_t j = 0; j < m1.cols(); ++j) {
      EXPECT_EQ(m1(i, j), m2(i, j));
    }
  }
}

TEST(MatrixTest, MoveConstructor) {
  BoolMatrix m1(100, 100, 1);
  BoolMatrix m2 = std::move(m1);

  EXPECT_EQ(100, m2.rows());
  EXPECT_EQ(100, m2.cols());
  EXPECT_EQ(1, m2(0, 0));
  EXPECT_EQ(1, m2(99, 99));
}

TEST(MatrixTest, MoveAssignment) {
  BoolMatrix m1(100, 100, 1);
  BoolMatrix m2(1, 1, 0);
  m2 = std::move(m1);

  EXPECT_EQ(100, m2.rows());
  EXPECT_EQ(100, m2.cols());
  EXPECT_EQ(1, m2(0, 0));
  EXPECT_EQ(1, m2(99, 99));
}

TEST(MatrixTest, Irregular2DVectorPadding) {
  std::vector<std::vector<uint8_t>> data = {{1, 0, 1, 1}, {0, 1}, {1, 1, 1}};

  BoolMatrix m(data);
  EXPECT_EQ(3, m.rows());
  EXPECT_EQ(4, m.cols());

  EXPECT_EQ(1, m(0, 0));
  EXPECT_EQ(0, m(0, 1));
  EXPECT_EQ(1, m(0, 2));
  EXPECT_EQ(1, m(0, 3));

  EXPECT_EQ(0, m(1, 0));
  EXPECT_EQ(1, m(1, 1));
  EXPECT_EQ(0, m(1, 2));
  EXPECT_EQ(0, m(1, 3));

  EXPECT_EQ(1, m(2, 0));
  EXPECT_EQ(1, m(2, 1));
  EXPECT_EQ(1, m(2, 2));
  EXPECT_EQ(0, m(2, 3));
}

TEST(MatrixTest, CheckerboardPattern) {
  BoolMatrix m(4, 4, 0);

  for (size_t i = 0; i < m.rows(); ++i) {
    for (size_t j = 0; j < m.cols(); ++j) {
      m(i, j) = (i + j) % 2;
    }
  }

  EXPECT_EQ(0, m(0, 0));
  EXPECT_EQ(1, m(0, 1));
  EXPECT_EQ(0, m(0, 2));
  EXPECT_EQ(1, m(0, 3));

  EXPECT_EQ(1, m(1, 0));
  EXPECT_EQ(0, m(1, 1));
  EXPECT_EQ(1, m(1, 2));
  EXPECT_EQ(0, m(1, 3));
}
