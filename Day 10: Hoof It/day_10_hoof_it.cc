/**
 * AoC 2024: Day 10: Hoof It

 * @file   day_10_hoof_it.cc
 * @author Raúl Gonzalez Acosta
 * @date   10/12/2024
 * @brief  Calculate the score and rating of trailheads in a map.
 * @see    https://adventofcode.com/2024/day/10
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <set>
#include <string>

// Directions for up, down, left, right
const std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

// Function to parse the map
std::vector<std::vector<int>> parseMap(const std::string& inputFile) {
  std::vector<std::vector<int>> map;
  std::ifstream file(inputFile);
  std::string line;

  try {
    while (std::getline(file, line)) {
      std::vector<int> row;
      for (char ch : line) {
        if (ch >= '0' && ch <= '9') { row.push_back(ch - '0'); }
      }
      map.push_back(row);
    }
    file.close();

    return map;
  } catch (...) {
    throw std::runtime_error("Error: Cannot parse the map");
    file.close();
    exit(EXIT_FAILURE);
  }
}

// Check if a position is within the bounds of the map
bool isValid(int x, int y, const std::vector<std::vector<int>>& map) {
  return ((x >= 0 && x < map.size()) && (y >= 0 && y < map[0].size()));
}

// Recursive DFS to calculate the distinct trails starting from (x, y)
void dfs(int x, int y, const std::vector<std::vector<int>>& map, std::set<std::vector<std::pair<int, int>>>& trails, 
         std::vector<std::pair<int, int>>& currentTrail, std::vector<std::vector<bool>>& visited) {
  if (map[x][y] == 9) {
    trails.insert(currentTrail);
    return;
  }

  visited[x][y] = true;

  for (const auto& [dx, dy] : directions) {
    int nx = x + dx, ny = y + dy;

    if (isValid(nx, ny, map) && !visited[nx][ny] && map[nx][ny] == map[x][y] + 1) {
      currentTrail.emplace_back(nx, ny);
      dfs(nx, ny, map, trails, currentTrail, visited);
      currentTrail.pop_back();
    }
  }

  visited[x][y] = false;
}

// BFS to calculate the score of a trailhead
int calculateTrailheadScore(int startX, int startY, const std::vector<std::vector<int>>& map) {
  int score = 0;
  std::queue<std::pair<int, int>> queue;
  std::vector<std::vector<bool>> visited(map.size(), std::vector<bool>(map[0].size(), false));

  queue.push({startX, startY});
  visited[startX][startY] = true;

  while (!queue.empty()) {
    auto [x, y] = queue.front();
    queue.pop();

    // If we reach height 9, increment the score
    if (map[x][y] == 9) { score++; }

    // Explore neighbors
    for (const auto& [dx, dy] : directions) {
      int nx = x + dx, ny = y + dy;

      if (isValid(nx, ny, map) && !visited[nx][ny] && map[nx][ny] == map[x][y] + 1) {
        queue.push({nx, ny});
        visited[nx][ny] = true;
      }
    }
  }

  return score;
}

// Function to calculate the rating of a trailhead using DFS
int calculateTrailheadRating(const std::vector<std::vector<int>>& map, int x, int y) {
  std::set<std::vector<std::pair<int, int>>> trails;
  std::vector<std::pair<int, int>> currentTrail = {{x, y}};
  std::vector<std::vector<bool>> visited(map.size(), std::vector<bool>(map[0].size(), false));
  dfs(x, y, map, trails, currentTrail, visited);

  return trails.size();
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

  std::vector<std::vector<int>> map = parseMap(argv[1]);

  // Calculate trailhead scores and ratings
  int totalScore = 0, totalRating = 0;

  for (int i = 0; i < map.size(); ++i) {
    for (int j = 0; j < map[i].size(); ++j) {
      if (map[i][j] == 0) { // Trailhead
        // Calculate score
        int score = calculateTrailheadScore(i, j, map);
        // Calculate rating
        int rating = calculateTrailheadRating(map, i, j);

        if (trace) {
          std::cout << "Trailhead at (" << i << ", " << j << ") has score: " << score 
                    << " and rating: " << rating << std::endl;
        }

        totalScore += score;
        totalRating += rating;
      }
    }
  }

  std::cout << "Total score of all trailheads: " << totalScore << "\n";
  std::cout << "Total rating of all trailheads: " << totalRating << "\n";

  return EXIT_SUCCESS;
}
