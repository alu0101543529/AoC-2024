/**
 * AoC 2024: Day 7: Bridge Repair

 * @file   day_7_bridge_repair.cc
 * @author Raúl Gonzalez Acosta
 * @date   07/12/2024
 * @brief  Check if a given equation can be evaluated to a target value.
 * @see    https://adventofcode.com/2024/day/7
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

// Helper function to split a line into target value and numbers
std::pair<long, std::vector<long>> parseEquation(const std::string& line) {
  std::istringstream iss(line);
  long target;
  char colon;
  if (!(iss >> target >> colon) || colon != ':') {
    throw std::runtime_error("Invalid line format: " + line);
  }

  std::vector<long> numbers;
  long num;
  while (iss >> num) { numbers.push_back(num); }

  if (numbers.empty()) {
    throw std::runtime_error("No numbers found in line: " + line);
  }

  return {target, numbers};
}

// Function to evaluate all combinations of operators iteratively
bool isValidEquationSumMultiply(long target, const std::vector<long>& numbers) {
  size_t numbers_size = numbers.size() - 1;
  if (numbers.size() == 1) {
    return numbers[0] == target;
  }

  // Total combinations of operators: 2^(numbers.size() - 1)
  size_t totalCombinations = 1 << numbers_size;

  for (size_t mask = 0; mask < totalCombinations; ++mask) {
    long result = numbers[0];

    for (size_t i = 0; i < numbers_size; ++i) {
      // Operator '+'
      if (mask & (1 << i)) {  result += numbers[i + 1]; } 
      // Operator '*'
      else { result *= numbers[i + 1]; }
    }

    if (result == target) { return true; }
  }
  return false;
}

// Function to concatenate two numbers
long concatenate(long left, long right) {
  std::string leftStr = std::to_string(left);
  std::string rightStr = std::to_string(right);
  std::string concatenated = leftStr + rightStr;
  
  return std::stol(concatenated);
}

// Function to evaluate expressions iteratively with all operator combinations
bool isValidEquationConcatenation(long target, const std::vector<long>& numbers) {
  size_t n = numbers.size() - 1;
  if (numbers.size() == 1) { return numbers[0] == target; }

  // Total combinations of operators: 3^(numbers.size() - 1)
  size_t totalCombinations = pow(3, n);

  for (size_t mask = 0; mask < totalCombinations; ++mask) {
    long long result = numbers[0];
    size_t currentMask = mask;

    for (size_t i = 0; i < n; ++i) {
      int operatorChoice = currentMask % 3;  // 0: +, 1: *, 2: ||
      currentMask /= 3;

      // Operator '+'
      if (operatorChoice == 0) { result += numbers[i + 1]; }
      // Operator '*'
      else if (operatorChoice == 1) { result *= numbers[i + 1]; } 
      // Operator '||'
      else if (operatorChoice == 2) {
        long long concatenated = concatenate(result, numbers[i + 1]);
        if (concatenated == -1) {
          result = -1;  // Handle invalid concatenation
          break;
        }
        result = concatenated;
      }

      // Short-circuit evaluation if result exceeds target
      if (result > target) { break; }
    }

    if (result == target) { return true; }
  }
  return false;
}

int main(int argc, char* argv[]) {
  // Check if the input file is provided
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
    return EXIT_FAILURE;
  }

  std::ifstream inputFile(argv[1]);
  if (!inputFile) {
    std::cerr << "Error: Cannot open file " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  std::string line;
  long totalCalibrationResultSumMultiply = 0, totalCalibrationResultConcatenate = 0;

  while (std::getline(inputFile, line)) {
    if (line.empty()) { continue; }
    auto [target, numbers] = parseEquation(line);

    if (isValidEquationSumMultiply(target, numbers)) { totalCalibrationResultSumMultiply += target; }
    if (isValidEquationConcatenation(target, numbers)) { totalCalibrationResultConcatenate += target; }
  }

  // Close the file
  inputFile.close();  

  std::cout << "Total calibration result with '+' and '*' operators: " << totalCalibrationResultSumMultiply << std::endl;
  std::cout << "Total calibration result with '+', '*' and '|' operators: " << totalCalibrationResultConcatenate << std::endl;

  return EXIT_SUCCESS;
}