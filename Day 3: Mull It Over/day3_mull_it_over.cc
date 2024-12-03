/**
 * AoC 2024: Day 3: Mull It Over

 * @file   day3_mull_it_over.cc
 * @author Raúl Gonzalez Acosta
 * @date   03/12/2024
 * @brief  Extract and sum the valid mul instructions from a corrupted memory.
 * @see    https://adventofcode.com/2024/day/3
 */

#include <iostream>
#include <fstream>
#include <string>
#include <regex>

/**
 * @brief Extract and sum the valid mul instructions from a corrupted memory.
 * @param[in] input: The corrupted memory containing mul and control instructions.
 * @return The total sum of valid mul instructions.
 */
int extractAndSumValidInstructions(const std::string& input) {
  // Regex to match valid mul(X,Y) and control instructions do(), don't()
  std::regex instructionRegex(R"(mul\((\d{1,3}),(\d{1,3})\)|do\(\)|don't\(\))");

  std::smatch match;
  std::string::const_iterator searchStart(input.cbegin());

  int totalSum = 0;
  // The program starts with mul instructions enabled
  bool isEnabled = true; 

  // Process all instructions
  while (std::regex_search(searchStart, input.cend(), match, instructionRegex)) {
    std::string instruction = match.str();

    if (instruction == "do()") { isEnabled = true; } 
    else if (instruction == "don't()") { isEnabled = false; } 
    else if (isEnabled) {
      // Extract and calculate mul(X,Y) if enabled
      int x = std::stoi(match[1]);
      int y = std::stoi(match[2]);
      totalSum += x * y;
    }

    // Move the search start to after the current match
    searchStart = match.suffix().first;
  }

  return totalSum;
}


int main(int argc, char* argv[]) {
  // Check if the input file was provided
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
    return EXIT_FAILURE;
  }

  std::ifstream inputFile(argv[1]);
  // Check if the input file can be opened
  if (!inputFile) {
    std::cerr << "Error: Could not open file " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  std::string corruptedMemory((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

  // Close the input file
  inputFile.close();

  int result = extractAndSumValidInstructions(corruptedMemory);
  std::cout << "The total sum of valid mul instructions is: " << result << std::endl;

  return EXIT_SUCCESS;
}
