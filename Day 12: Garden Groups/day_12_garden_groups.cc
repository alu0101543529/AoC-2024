/**
 * AoC 2024: Day 12: Garden Groups
 * @file   day_12_garden_groups.cc
 * @author Raúl Gonzalez Acosta
 * @date   12/12/2024
 * @brief  Simulate garden groups and calculate the total price for fencing.
 * @see    https://adventofcode.com/2024/day/12
 */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>

using namespace std;

// Directions for moving in the grid (up, down, left, right)
const vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

// Perform a flood-fill to find a region's area and perimeter
void floodFill(const vector<string>& garden, vector<vector<bool>>& visited, int x, int y, char plantType, int& area, int& perimeter) {
  int rows = garden.size();
  int cols = garden[0].size();
  visited[x][y] = true;
  area++;

  for (const auto& dir : directions) {
    int nx = x + dir.first;
    int ny = y + dir.second;

    if (nx >= 0 && nx < rows && ny >= 0 && ny < cols) {
      if (garden[nx][ny] == plantType) {
        if (!visited[nx][ny]) { floodFill(garden, visited, nx, ny, plantType, area, perimeter); }
      } 
      // Adjacent plot of a different type contributes to the perimeter
      else { perimeter++; }
    } 
    // Out-of-bounds edge contributes to the perimeter
    else { perimeter++; }
  }
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

  std::ifstream inputFile(argv[1]);
  if (!inputFile) {
    std::cerr << "Error: Cannot open file " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  vector<string> garden;
  string line;

  // Read the garden map from the input file
  while (getline(inputFile, line)) {
    if (!line.empty()) { garden.push_back(line); }
  }

  int rows = garden.size();
  int cols = garden[0].size();

  vector<vector<bool>> visited(rows, vector<bool>(cols, false));
  unordered_map<char, vector<pair<int, int>>> regionDetails; // Stores details for each plant type
  int totalPrice = 0;

  // Process each cell in the garden
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (!visited[i][j]) {
        char plantType = garden[i][j];
        int area = 0;
        int perimeter = 0;

        // Perform flood-fill to compute area and perimeter
        floodFill(garden, visited, i, j, plantType, area, perimeter);

        // Calculate price and update total
        int price = area * perimeter;
        totalPrice += price;

        // Store details for debugging or additional output
        regionDetails[plantType].emplace_back(area, perimeter);
      }
    }
  }

  // Output results
  if (trace) {
    cout << "Region details:" << endl;
    for (const auto& [plantType, regions] : regionDetails) {
      cout << "Plant type " << plantType << ":" << endl;
      for (const auto& [area, perimeter] : regions) {
        cout << "  Area: " << area << ", Perimeter: " << perimeter << ", Price: " << area * perimeter << endl;
      }
    }
  }
  cout << "Total price for fencing: " << totalPrice << endl;

  return EXIT_SUCCESS;
}
