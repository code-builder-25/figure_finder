#include "finders/figure_finder_factory.hpp"
#include "matrix/matrix.hpp"
#include "matrix/matrix_io.hpp"
#include <exception>
#include <iostream>
#include <string>
using namespace figures;

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <matrix_file_name>\n";
    return 1;
  }
  std::string filename = argv[1];
  BoolMatrix matrix;
  try {
    std::cerr << "Loading matrix from " << filename << "..." << std::endl;
    matrix = loadMatrixFromFile<uint8_t>(filename);
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 2;
  }
  std::cerr << "Matrix loaded: " << matrix.rows() << "x" << matrix.cols() << std::endl;

  std::cerr << "Creating FigureFinder (GCC Parallel Algorithm)..." << std::endl;
  auto finder = figures::createFigureFinder(figures::FigureFinderType::GCCParallel);

  std::cerr << "Counting figures..." << std::endl;
  size_t figure_count = finder->countFigures(matrix);

  std::cerr << "Number of figures found: " << figure_count << std::endl;
  std::cout << figure_count << std::endl;
  return 0;
}
