/**
 * AoC 2024: Day 18: RAM Run

 * @file   day_18_ram_run.cc
 * @author Raúl Gonzalez Acosta
 * @date   18/12/2024
 * @brief  Determines the shortest path in a corrupted grid and the first byte that blocks the path.
 * @see    https://adventofcode.com/2024/day/18
 */

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <fstream>
#include <sstream>
#include <set>

// Constants for the grid size
const int GRID_SIZE = 71; // 71 * 71 grid

// Directions for moving in the grid: up, down, left, right
const std::vector<std::pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

// Function to simulate byte corruption on the grid
void corruptGrid(const std::pair<int, int>& byte, std::vector<std::vector<bool>>& grid) {
  int x = byte.first, y = byte.second;
  if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
    // Mark the cell as corrupted
    grid[x][y] = true; 
  }
}

// Function to find the shortest path using BFS
int findShortestPath(const std::vector<std::vector<bool>>& grid) {
  std::queue<std::pair<int, int>> q;
  std::vector<std::vector<int>> distance(GRID_SIZE, std::vector<int>(GRID_SIZE, -1));

  q.push({0, 0});
  distance[0][0] = 0;

  while (!q.empty()) {
    auto [x, y] = q.front();
    q.pop();

    if (x == GRID_SIZE - 1 && y == GRID_SIZE - 1) {
      return distance[x][y];
    }

    for (const auto& [dx, dy] : directions) {
      int nx = x + dx;
      int ny = y + dy;

      if (nx >= 0 && nx < GRID_SIZE && ny >= 0 && ny < GRID_SIZE && !grid[nx][ny] && distance[nx][ny] == -1) {
        distance[nx][ny] = distance[x][y] + 1;
        q.push({nx, ny});
      }
    }
  }

  // No path found
  return -1;
}

// Function to parse input from a file
std::vector<std::pair<int, int>> parseInput(const std::string& filename) {
  std::vector<std::pair<int, int>> bytes;
  std::ifstream file(filename);
  if (!file) {
    std::cerr << "Error: Cannot open file " << filename << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    int x, y;
    char comma;
    if (iss >> x >> comma >> y) {
      bytes.emplace_back(x, y);
    }
  }

  file.close();
  return bytes;
}

// Function to determine the first byte that blocks the path
std::pair<int, int> findBlockingByte(const std::vector<std::pair<int, int>>& bytes, std::vector<std::vector<bool>>& grid) {
  for (const auto& byte : bytes) {
    corruptGrid(byte, grid);
    // Return the first byte that blocks the path
    if (findShortestPath(grid) == -1) { return byte; }
  }
  // No blocking byte found
  return {-1, -1}; 
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
    return EXIT_FAILURE;
  }

  std::string filename = argv[1];

  // Parse input
  std::vector<std::pair<int, int>> bytes = parseInput(filename);

  // Initialize grid
  std::vector<std::vector<bool>> grid(GRID_SIZE, std::vector<bool>(GRID_SIZE, false));

  // Corrupt the grid with the first 1024 bytes
  std::vector<std::pair<int, int>> firstKilobyte(bytes.begin(), bytes.begin() + std::min(1024, (int)bytes.size()));
  for (const auto& byte : firstKilobyte) { corruptGrid(byte, grid); }

  // Phase 1: Find the shortest path
  int shortestPath = findShortestPath(grid);

  // Phase 2: Find the first blocking byte
  std::vector<std::vector<bool>> newGrid(GRID_SIZE, std::vector<bool>(GRID_SIZE, false)); // Fresh grid for simulation
  std::pair<int, int> blockingByte = findBlockingByte(bytes, newGrid);

  // Output results
  if (shortestPath != -1) { std::cout << "Minimum number of steps: " << shortestPath << std::endl; } 
  else { std::cout << "No path to the exit found." << std::endl; }

  if (blockingByte.first == -1) { std::cout << "No blocking byte found." << std::endl; }
  else { std::cout << "Blocking byte: " << blockingByte.first << "," << blockingByte.second << std::endl; }

  return EXIT_SUCCESS;
}