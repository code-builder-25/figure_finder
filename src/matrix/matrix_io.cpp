#include "matrix/matrix_io.hpp"
#include <algorithm>
#include <charconv>
#include <fstream>
#include <stdexcept>
#include <system_error>

namespace figures {

namespace {

constexpr bool isWhitespace(char c) { return c == ' ' || c == '\t' || c == '\r' || c == '\n'; }

template <typename T> std::vector<T> parseLine(const std::string& line) {
  std::vector<T> row;
  const char* ptr = line.data();
  const char* end = ptr + line.size();

  while (ptr < end) {
    if (isWhitespace(*ptr)) {
      ++ptr;
      continue;
    }

    T value;
    auto [next_ptr, ec] = std::from_chars(ptr, end, value);

    if (ec != std::errc{}) {
      throw std::runtime_error("Parse error: " + std::make_error_code(ec).message());
    }

    row.push_back(value);
    ptr = next_ptr;
  }

  return row;
}
} // namespace

template <typename T> Matrix<T> loadMatrixFromFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + filename);
  }

  std::vector<std::vector<T>> rows;
  std::string line;

  while (std::getline(file, line)) {
    if (line.empty() || std::all_of(line.begin(), line.end(), isWhitespace)) {
      continue;
    }

    std::vector<T> row;
    try {
      row = parseLine<T>(line);
    } catch (const std::runtime_error& e) {
      throw std::runtime_error(e.what() + std::string(" in file: ") + filename);
    }

    if (!rows.empty() && row.size() != rows[0].size()) {
      throw std::runtime_error("Inconsistent number of columns in file: " + filename);
    }

    rows.push_back(std::move(row));
  }

  if (rows.empty()) {
    throw std::runtime_error("File is empty or contains no valid data: " + filename);
  }

  return Matrix<T>(rows);
}

template Matrix<uint8_t> loadMatrixFromFile<uint8_t>(const std::string&);

} // namespace figures
