/**
 * AoC 2024: Day 2: Red-Nosed Reports

 * @file   day2_red_nosed_reports.cc
 * @author Raúl Gonzalez Acosta
 * @date   02/12/2024
 * @brief  Determine the number of safe reports that can be generated from a list of levels.
 * @see    https://adventofcode.com/2024/day/2
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

/**
 * @brief Check if a report is safe.
 * @param[in] levels: The list of levels in the report.
 * @return True if the report is safe, false otherwise.
 */
bool isSafeReport(const std::vector<int>& levels) {
  // A report must have at least two levels to be valid.
  if (levels.size() < 2) { return false; }

  // The difference between the first two levels must be within [1, 3].
  int diff = levels[1] - levels[0];
  if (std::abs(diff) < 1 || std::abs(diff) > 3) { return false; }

  bool increasing = diff > 0;
  for (size_t i = 1; i < levels.size(); ++i) {
    int currentDiff = levels[i] - levels[i - 1];
    // Difference between adjacent levels is not within [1, 3].
    if (std::abs(currentDiff) < 1 || std::abs(currentDiff) > 3) { return false; }
    // The levels are not consistently increasing or decreasing.
    if ((increasing && currentDiff < 0) || (!increasing && currentDiff > 0)) { return false; }
  }

  // The report satisfies all conditions for safety.
  return true; 
}

/**
 * @brief Check if a report can be made safe by removing one level.
 * @param[in] levels: The list of levels in the report.
 * @return True if the report can be made safe, false otherwise.
 */
bool canBeMadeSafe(const std::vector<int>& levels) {
  for (size_t i = 0; i < levels.size(); ++i) {
    // Create a new vector excluding the current level
    std::vector<int> modifiedLevels;
    for (size_t j = 0; j < levels.size(); ++j) {
      if (j != i) { modifiedLevels.push_back(levels[j]); }
    }

    // Check if the modified report is safe
    if (isSafeReport(modifiedLevels)) { return true; }
  }

  return false;
}

/**
 * @brief Count the number of safe reports in a file.
 * @param[in] filename: The name of the input file.
 * @return The number of safe reports in the file.
 */
int countSafeReports(const std::string& filename) {
  std::ifstream inputFile(filename);

  // Check if the file was opened successfully
  if (!inputFile) {
    std::cerr << "Error: Could not open file " << filename << std::endl;
    return -1;
  }

  int safeCount = 0;
  std::string line;
  
  while (std::getline(inputFile, line)) {
    std::istringstream lineStream(line);
    std::vector<int> levels;
    int level;

    // Read all levels in the report
    while (lineStream >> level) { levels.push_back(level); }

    // Check if the report is safe directly or can be made safe
    if (isSafeReport(levels) || canBeMadeSafe(levels)) { safeCount++; }
  }

  // Close the input file
  inputFile.close();
  return safeCount;
}

int main(int argc, char* argv[]) {
  // Check if the input file was provided
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
    return EXIT_FAILURE;
  }

  int safeReports = countSafeReports(argv[1]);
  // There was an error reading the input file
  if (safeReports == -1) { return EXIT_FAILURE; }

  std::cout << "The number of safe reports is: " << safeReports << std::endl;

  return EXIT_SUCCESS;
}
