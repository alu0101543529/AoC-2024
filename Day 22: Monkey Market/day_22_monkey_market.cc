/**
 * AoC 2024: Day 22: Monkey Market

 * @file   day_22_monkey_market.cc
 * @author Raúl Gonzalez Acosta
 * @date   22/12/2024
 * @brief  Determine the total amount of bananas and the maximum value of a pattern of 4 consecutive differences.
 * @see    https://adventofcode.com/2024/day/22
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <algorithm>
#include <iterator>
#include <functional>

constexpr int MOD24 = 0xFFFFFF;

// Function to calculate the transformation f(s)
int f(int s) {
  s = (s ^ (s << 6)) & MOD24;
  s = (s ^ (s >> 5)) & MOD24;
  return (s ^ (s << 11)) & MOD24;
}

// Helper function to compute pairwise differences modulo 10
std::vector<int> compute_differences(const std::vector<int>& nums) {
  std::vector<int> diffs;
  for (size_t i = 0; i < nums.size() - 1; ++i) { diffs.push_back(nums[i + 1] % 10 - nums[i] % 10); }
  return diffs;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>\n";
    return EXIT_FAILURE;
  }

  std::ifstream input(argv[1]);
  if (!input) {
    std::cerr << "Error: Could not open file " << argv[1] << "\n";
    return EXIT_FAILURE;
  }

  struct TupleHash {
    std::size_t operator()(const std::tuple<int, int, int, int>& tuple) const {
      std::size_t hash = 0;
      std::apply([&hash](auto&&... args) { ((hash ^= std::hash<std::decay_t<decltype(args)>>{}(args) + 0x9e3779b9 + (hash << 6) + (hash >> 2)), ...); }, tuple);
      return hash;
    }
  };

  std::unordered_map<std::tuple<int, int, int, int>, int, TupleHash> ans2;
  int ans1 = 0, s;
  while (input >> s) {
    // Generate the sequence
    std::vector<int> nums = {s};
    for (int i = 0; i < 2000; ++i) {
      s = f(s);
      nums.push_back(s);
    }

    // Update ans1 with the last element of the sequence
    ans1 += nums.back();

    // Compute the differences modulo 10
    auto diffs = compute_differences(nums);

    // Track patterns of 4 consecutive differences
    std::unordered_set<std::tuple<int, int, int, int>, TupleHash> seen;
    for (size_t i = 0; i + 4 < diffs.size(); ++i) {
      auto pat = std::make_tuple(diffs[i], diffs[i + 1], diffs[i + 2], diffs[i + 3]);
      if (seen.find(pat) == seen.end()) {
        ans2[pat] += nums[i + 4] % 10;
        seen.insert(pat);
      }
    }
  }

  // Find the maximum value in ans2
  int max_ans2 = 0;
  for (const auto& [key, value] : ans2) { max_ans2 = std::max(max_ans2, value); }

  std::cout << ans1 << " " << max_ans2 << "\n";
  
  return EXIT_SUCCESS;
}