/**
 * AoC 2024: Day 11: Plutonian Pebbles

 * @file   day_11_plutonian_pebbles.cc
 * @author Raúl Gonzalez Acosta
 * @date   11/12/2024
 * @brief  Simulate the blinks of Plutonian pebbles, following specific rules.
 * @see    https://adventofcode.com/2024/day/11
 */

#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

// Splits a number with even digits into two numbers
std::pair<long long, long long> splitNumber(long long num) {
  std::string numStr = std::to_string(num);
  int mid = numStr.size() / 2;
  long long left = std::stoll(numStr.substr(0, mid));
  long long right = std::stoll(numStr.substr(mid));
  return {left, right};
}

// Function to simulate blinks using a frequency map
void simulateBlinks(std::unordered_map<long long, long long>& stoneCounts, int blinks) {
  for (int blink = 0; blink < blinks; blink++) {
    std::unordered_map<long long, long long> newStoneCounts;
    for (const auto& [stone, count] : stoneCounts) {
      // Rule 1: Replace 0 with 1
      if (stone == 0) { newStoneCounts[1] += count; } 
      // Rule 2: Split even-digit stones
      else if (std::to_string(stone).size() % 2 == 0) {
        auto [left, right] = splitNumber(stone);
        newStoneCounts[left] += count;
        newStoneCounts[right] += count;
      } 
      // Rule 3: Multiply odd-digit stones by 2024
      else { newStoneCounts[stone * 2024] += count; }
    }
    stoneCounts = std::move(newStoneCounts);
  }
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
  if (!std::getline(inputFile, line)) {
    std::cerr << "Error: Empty input file" << std::endl;
    return EXIT_FAILURE;
  }


  std::istringstream iss(line);
  std::unordered_map<long long, long long> stoneCounts;
  long long num;
  while (iss >> num) { stoneCounts[num]++; }

  int blinks;
  std::cout << "Enter the number of blinks: ";
  std::cin >> blinks;

  simulateBlinks(stoneCounts, blinks);
  
  inputFile.close();

  // Output the final result
  long long totalStones = 0;
  for (const auto& [stone, count] : stoneCounts) { totalStones += count; }

  std::cout << "After " << blinks << " blinks, there are " << totalStones << " stones." << std::endl;

  return EXIT_SUCCESS;
}
