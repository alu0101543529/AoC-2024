/**
 * AoC 2024: Day 1: Historian Hysteria

 * @file   day1_historian_hysteria.cc
 * @author Raúl Gonzalez Acosta
 * @date   01/12/2024
 * @brief  There are two lists of integers, one on the left and one on the right. Calculate the total distance and similarity score between the two lists.
 * @see    https://adventofcode.com/2024/day/1
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <unordered_map>

/**
 * @brief Calculate the total distance between two lists of integers.
 * @param[in] left: The list of integers on the left.
 * @param[in] right: The list of integers on the right.
 * @return The total distance between the two lists.
 */
int calculateTotalDistance(const std::vector<int>& left, const std::vector<int>& right) {
  // Create sorted copies of the two lists
  std::vector<int> sortedLeft = left;
  std::vector<int> sortedRight = right;

  // Sort the two lists in ascending order
  std::sort(sortedLeft.begin(), sortedLeft.end());
  std::sort(sortedRight.begin(), sortedRight.end());

  // Calculate the total distance
  int totalDistance = 0;
  for (size_t i = 0; i < sortedLeft.size(); ++i) {
    // Add the absolute difference between each pair of elements
    totalDistance += std::abs(sortedLeft[i] - sortedRight[i]);
  }

  return totalDistance;
}

/**
 * @brief Calculate the similarity score between two lists of integers.
 * @param[in] left: The list of integers on the left.
 * @param[in] right: The list of integers on the right.
 * @return The similarity score between the two lists.
 */
int calculateSimilarityScore(const std::vector<int>& left, const std::vector<int>& right) {
  // Count occurrences of each number in the right list
  std::unordered_map<int, int> rightCount;
  for (int num : right) { rightCount[num]++; }

  // Calculate the similarity score by multiplying each number in the left list by its count in the right list
  int similarityScore = 0;
  for (int num : left) { similarityScore += num * rightCount[num]; }

  return similarityScore;
}

int main(int argc, char* argv[]) {
  // Check if the input file was provided
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
    return EXIT_FAILURE;
  }

  std::ifstream inputFile(argv[1]);
  // Open the input file and check if it was opened successfully
  if (!inputFile) {
    std::cerr << "Error: Could not open file " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  // Read input from file
  std::vector<int> leftList, rightList;
  int leftValue, rightValue;
  while (inputFile >> leftValue >> rightValue) {
    leftList.push_back(leftValue);
    rightList.push_back(rightValue);
  }

  // Close the input file
  inputFile.close();

  // Ensure both lists have the same size
  if (leftList.size() != rightList.size()) {
    std::cerr << "Error: Input file must have the same number of entries in both columns." << std::endl;
    return EXIT_FAILURE;
  }

  // Phase 1: Calculate the total distance between the two lists
  int totalDistance = calculateTotalDistance(leftList, rightList);
  std::cout << "The total distance is: " << totalDistance << std::endl;

  // Phase 2: Calculate the similarity score between the two lists
  int similarityScore = calculateSimilarityScore(leftList, rightList);
  std::cout << "The similarity score is: " << similarityScore << std::endl;

  return EXIT_SUCCESS;
}
