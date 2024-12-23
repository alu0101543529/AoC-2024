/**
 * AoC 2024: Day 20: Race Condition

 * @file   day_20_race_condition.cc
 * @author Raúl Gonzalez Acosta
 * @date   20/12/2024
 * @brief  Determine the shortest path from the start to the end point without cheating and with up to 20 cheat steps.
 * @see    https://adventofcode.com/2024/day/20
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <tuple>
#include <string>

using namespace std;

struct Point {
  int x, y;
  bool operator==(const Point& other) const { return x == other.x && y == other.y; }
};

struct PointHash {
  size_t operator()(const Point& p) const { return hash<int>()(p.x) ^ hash<int>()(p.y); }
};

// Directions for movement: {dx, dy}
const vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

// Function to parse input from a file
void parseInput(const string& filename, vector<string>& grid, Point& start, Point& end) {
  ifstream inputFile(filename);
  if (!inputFile) {
    cerr << "Error: Could not open file " << filename << endl;
    exit(EXIT_FAILURE);
  }

  string line;
  while (getline(inputFile, line)) {
    grid.push_back(line);
    for (size_t i = 0; i < line.size(); i++) {
      if (line[i] == 'S') start = {static_cast<int>(grid.size()) - 1, static_cast<int>(i)};
      if (line[i] == 'E') end = {static_cast<int>(grid.size()) - 1, static_cast<int>(i)};
    }
  }
}

// BFS to find the shortest path without cheating
int bfs(const vector<string>& grid, const Point& start, const Point& end) {
  queue<pair<Point, int>> q; // {current point, steps}
  unordered_map<Point, bool, PointHash> visited;
  
  q.push({start, 0});
  visited[start] = true;

  while (!q.empty()) {
    auto [current, steps] = q.front();
    q.pop();

    if (current == end) return steps;

    for (const auto& [dx, dy] : directions) {
      Point next = {current.x + dx, current.y + dy};
      if (next.x >= 0 && next.x < grid.size() && next.y >= 0 && next.y < grid[0].size() && grid[next.x][next.y] != '#' && !visited[next]) {
        visited[next] = true;
        q.push({next, steps + 1});
      }
    }
  }

  // No path found
  return -1; 
}

// BFS to find the shortest path with up to 20 cheat steps
int bfsWithCheat(const vector<string>& grid, const Point& start, const Point& end, int maxCheat) {
  struct State {
    Point point;
    int steps;
    int cheatsUsed;
  };

  queue<State> q;
  unordered_map<tuple<Point, int>, bool, tuple<Point, PointHash>> visited;

  q.push({start, 0, 0});
  visited[{start, 0}] = true;

  while (!q.empty()) {
    auto [current, steps, cheats] = q.front();
    q.pop();

    if (current == end) { return steps; }

    // Normal moves
    for (const auto& [dx, dy] : directions) {
      Point next = {current.x + dx, current.y + dy};
      if (next.x >= 0 && next.x < grid.size() && next.y >= 0 && next.y < grid[0].size()) {
        if (grid[next.x][next.y] != '#' && !visited[{next, cheats}]) {
          visited[{next, cheats}] = true;
          q.push({next, steps + 1, cheats});
        }

        // Cheating moves
        if (grid[next.x][next.y] == '#' && cheats < maxCheat && !visited[{next, cheats + 1}]) {
          visited[{next, cheats + 1}] = true;
          q.push({next, steps + 1, cheats + 1});
        }
      }
    }
  }

  // No path found
  return -1; 
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <input_file>" << endl;
    return EXIT_FAILURE;
  }

  string filename = argv[1];

  vector<string> grid;
  Point start, end;

  parseInput(filename, grid, start, end);

  int result = bfs(grid, start, end);
  if (result == -1) { cout << "No path found." << endl; } 
  else { cout << "Shortest path without cheating: " << result << " picoseconds." << endl; }

  parseInput(filename, grid, start, end);

  result = bfsWithCheat(grid, start, end, 20);
  if (result == -1) { cout << "No path found." << endl; } 
  else { cout << "Shortest path with up to 20 cheat steps: " << result << " picoseconds." << endl; }

  return EXIT_SUCCESS;
}
