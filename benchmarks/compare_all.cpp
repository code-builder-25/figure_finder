// Compare all algorithms on extra large files
#include "finders/figure_finder_factory.hpp"
#include "matrix/matrix_io.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;
using namespace std::chrono;
using namespace figures;

struct BenchmarkResult {
  string algorithm_name;
  string file_name;
  int figure_count;
  double time_seconds;
};

BenchmarkResult runBenchmark(figures::FigureFinderType type, const string& file_path) {
  using namespace figures;
  string algorithm_name = toString(type);
  cout << "  Testing " << algorithm_name << "..." << flush;
  auto start = high_resolution_clock::now();
  // Load matrix
  BoolMatrix matrix = loadMatrixFromFile<uint8_t>(file_path);
  // Run algorithm
  auto finder = createFigureFinder(type);
  int count = finder->countFigures(matrix);
  auto end = high_resolution_clock::now();
  duration<double> elapsed = end - start;
  cout << " Done! (" << fixed << setprecision(2) << elapsed.count() << "s)" << endl;
  BenchmarkResult result;
  result.algorithm_name = algorithm_name;
  result.file_name = file_path;
  result.figure_count = count;
  result.time_seconds = elapsed.count();
  return result;
}

void printResults(const vector<BenchmarkResult>& results, const string& file_description) {
  cout << "\n" << string(80, '=') << endl;
  cout << "Results for: " << file_description << endl;
  cout << string(80, '=') << endl;

  // Find the fastest
  double min_time = results[0].time_seconds;
  for (const auto& r : results) {
    if (r.time_seconds < min_time) {
      min_time = r.time_seconds;
    }
  }

  cout << left << setw(20) << "Algorithm" << right << setw(15) << "Figures" << setw(15)
       << "Time (s)" << setw(20) << "vs Fastest" << endl;
  cout << string(80, '-') << endl;

  for (const auto& r : results) {
    double ratio = r.time_seconds / min_time;
    cout << left << setw(20) << r.algorithm_name << right << setw(15) << r.figure_count << setw(15)
         << fixed << setprecision(2) << r.time_seconds;

    if (ratio <= 1.01) {
      cout << setw(20) << "FASTEST";
    } else {
      cout << setw(19) << fixed << setprecision(2) << ratio << "x";
    }
    cout << endl;
  }
  cout << string(80, '=') << endl;
}

int main(int argc, char* argv[]) {
  vector<string> test_files;
  vector<string> descriptions;

  if (argc > 1) {
    if ((argc - 1) % 2 != 0) {
      cerr << "Usage: " << argv[0] << " <file1> <desc1> [<file2> <desc2> ...]" << endl;
      return 1;
    }
    for (int i = 1; i < argc; i += 2) {
      test_files.push_back(argv[i]);
      descriptions.push_back(argv[i + 1]);
    }
  } else {
    test_files = {
        "../data/matrix_5000x4000_1M.txt",
        "../data/matrix_7500x6000_5M.txt",
    };
    descriptions = {"Extra Large 1M figures", "Extra Large 5M figures"};
  }

  cout << "\n" << string(80, '=') << endl;
  cout << "ALGORITHM COMPARISON ON EXTRA LARGE FILES" << endl;
  cout << string(80, '=') << endl;
  cout << "\nTesting available algorithms from factory:" << endl;
  for (auto type : figures::getAllFigureFinderTypes()) {
    cout << "  - " << figures::toString(type) << endl;
  }
  cout << endl;

  for (size_t i = 0; i < test_files.size(); i++) {
    const auto& file = test_files[i];
    const auto& desc = descriptions[i];

    cout << "\n" << string(80, '=') << endl;
    cout << "Testing File " << (i + 1) << ": " << file << endl;
    cout << string(80, '=') << endl;

    vector<BenchmarkResult> results;
    for (auto type : figures::getAllFigureFinderTypes()) {
      results.push_back(runBenchmark(type, file));
    }
    printResults(results, desc);
  }

  cout << "\n" << string(80, '=') << endl;
  cout << "COMPARISON COMPLETE" << endl;
  cout << string(80, '=') << endl;
  cout << endl;

  return 0;
}
