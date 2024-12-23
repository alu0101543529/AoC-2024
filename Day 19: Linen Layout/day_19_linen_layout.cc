/**
 * AoC 2024: Day 19: Linen Layout

 * @file   day_19_linen_layout.cc
 * @author Raúl Gonzalez Acosta
 * @date   19/12/2024
 * @brief  Determine the number of possible designs and total number of arrangements.
 * @see    https://adventofcode.com/2024/day/19
 */

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

// Function to parse input from command line
void parseInput(const std::string& filename, std::vector<std::string>& patterns, std::vector<std::string>& designs) {
  std::istringstream inputFile(filename);
  std::string line;
  bool isPatterns = true;

  while (std::getline(inputFile, line)) {
    if (line.empty()) {
      isPatterns = false; // Switch to reading designs
      continue;
    }

    std::istringstream iss(line);
    std::string token;
    if (isPatterns) {
      while (iss >> token) { patterns.push_back(token); }
    } 
    else { designs.push_back(line); }
  }
}

// Function to check if a design is possible using the given patterns
bool isPossible(const std::string& design, const std::unordered_set<std::string>& patternSet) {
  size_t n = design.size();
  std::vector<bool> dp(n + 1, false);
  dp[0] = true;

  for (size_t i = 1; i <= n; ++i) {
    for (size_t j = 0; j < i; ++j) {
      if (dp[j] && patternSet.count(design.substr(j, i - j))) {
        dp[i] = true;
        break;
      }
    }
  }

  return dp[n];
}

// Recursive function to count all possible arrangements of a design
int countArrangements(const std::string& design, const std::unordered_set<std::string>& patternSet, std::unordered_map<std::string, int>& memo) {
  if (design.empty()) return 1; // Base case: empty design has one arrangement
  if (memo.count(design)) return memo[design]; // Return cached result

  int totalArrangements = 0;
  for (size_t i = 1; i <= design.size(); ++i) {
    std::string prefix = design.substr(0, i);
    if (patternSet.count(prefix)) {
      totalArrangements += countArrangements(design.substr(i), patternSet, memo);
    }
  }

  memo[design] = totalArrangements;
  return totalArrangements;
}

std::pair<int, int> calculateArrangements(std::vector<std::string>& patterns, std::vector<std::string>& designs, bool trace) {
  std::unordered_set<std::string> patternSet(patterns.begin(), patterns.end());
  int possibleDesigns = 0;
  int totalArrangements = 0;

  std::unordered_map<std::string, int> memo;

  if (trace) {
    std::cout << "Patterns: ";
    for (const auto& pattern : patterns) { std::cout << pattern << " "; }

    std::cout << std::endl;

    std::cout << "Designs: ";
    for (const auto& design : designs) { std::cout << design << " "; }
    
    std::cout << std::endl;
  }

  for (const auto& design : designs) {
    if (isPossible(design, patternSet)) {
      possibleDesigns++;
      int arrangements = countArrangements(design, patternSet, memo);
      totalArrangements += arrangements;
      std::cout << "Design \"" << design << "\" is possible with " << arrangements << " arrangements.\n";
    } else {
      std::cout << "Design \"" << design << "\" is impossible.\n";
    }
  }

  return std::make_pair(possibleDesigns, totalArrangements);
}

int main(int argc, char* argv[]) {
  // Check if the input file is provided
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file> [-trace]" << std::endl;
    return EXIT_FAILURE;
  }

  // Check if the -trace option is provided
  bool trace = false;
  if (argc > 2 && std::string(argv[2]) == "-trace") { trace = true; }

  std::string filename = argv[1];
  std::vector<std::string> patterns;
  std::vector<std::string> designs;

  parseInput(filename, patterns, designs);

  auto [possibleDesigns, totalArrangements] = calculateArrangements(patterns, designs, trace);

  std::cout << "Number of possible designs: " << possibleDesigns << "\n";
  std::cout << "Total number of arrangements: " << totalArrangements << "\n";

  return EXIT_SUCCESS;
}
