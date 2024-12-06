/**
 * AoC 2024: Day 6: Guard Gallivant

 * @file   day_6_guard_gallivant.cc
 * @author Raúl Gonzalez Acosta
 * @date   06/12/2024
 * @brief  Simulate a guard's patrol and find the number of visited positions and valid obstruction positions.
 * @see    https://adventofcode.com/2024/day/6
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <set>
using namespace std;

// Directions: {dx, dy} for Up, Right, Down, Left
const vector<pair<int, int>> DIRECTIONS = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

// Helper function to find the guard's starting position and direction
pair<pair<int, int>, int> findGuard(const vector<string>& map) {
  for (int i = 0; i < map.size(); ++i) {
    for (int j = 0; j < map[i].size(); ++j) {
      if (map[i][j] == '^') return {{i, j}, 0}; // Up
      if (map[i][j] == '>') return {{i, j}, 1}; // Right
      if (map[i][j] == 'v') return {{i, j}, 2}; // Down
      if (map[i][j] == '<') return {{i, j}, 3}; // Left
    }
  }
  throw runtime_error("Guard not found on the map.");
}

// Simulate the guard's patrol
int simulatePatrol(vector<string>& map) {
  auto [position, direction] = findGuard(map);
  int rows = map.size();
  int cols = map[0].size();
  set<pair<int, int>> visited; // Track unique visited positions

  // Mark the starting position as visited
  visited.insert(position);
  map[position.first][position.second] = 'X';

  while (true) {
    // Calculate next position
    int nx = position.first + DIRECTIONS[direction].first;
    int ny = position.second + DIRECTIONS[direction].second;

    // Check if the next position is within bounds and not an obstacle
    if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && map[nx][ny] != '#') {
      // Move forward
      position = {nx, ny};
      visited.insert(position);
      map[nx][ny] = 'X'; // Mark as visited
    } 
    // In other case, turn right 90 degrees
    else { direction = (direction + 1) % 4; }

    // Stop if the guard goes out of bounds
    if (nx < 0 || nx >= rows || ny < 0 || ny >= cols) { break; }
  }

  // Return the count of distinct visited positions
  return visited.size();
}

// Simulate the guard's patrol and check if it gets stuck in a loop
bool simulateWithLoopCheck(vector<string>& map, const pair<int, int>& obstruction) {
  auto [position, direction] = findGuard(map);
  int rows = map.size();
  int cols = map[0].size();

  set<pair<pair<int, int>, int>> seenStates; // Track visited states (position + direction)

  // Place obstruction
  map[obstruction.first][obstruction.second] = '#';

  while (true) {
    // Save current state
    if (seenStates.count({position, direction})) {
      // Loop detected
      map[obstruction.first][obstruction.second] = '.'; // Restore map
      return true;
    }
    seenStates.insert({position, direction});

    // Calculate next position
    int nx = position.first + DIRECTIONS[direction].first;
    int ny = position.second + DIRECTIONS[direction].second;

    // Check if the next position is within bounds and not an obstacle
    if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && map[nx][ny] != '#') { position = {nx, ny}; } 
    // In other case, turn right 90 degrees
    else { direction = (direction + 1) % 4; }

    // Check if the guard leaves the map
    if (nx < 0 || nx >= rows || ny < 0 || ny >= cols) {
      map[obstruction.first][obstruction.second] = '.'; // Restore map
      return false;
    }
  }
}

// Find all possible obstruction positions that cause a loop
vector<pair<int, int>> findLoopCausingObstructions(vector<string>& map) {
  int rows = map.size();
  int cols = map[0].size();
  vector<pair<int, int>> validObstructions;

  auto [startPos, _] = findGuard(map);

  // Try every possible position for an obstruction
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (map[i][j] == '.' && make_pair(i, j) != startPos) {
        if (simulateWithLoopCheck(map, {i, j})) {
          validObstructions.push_back({i, j});
        }
      }
    }
  }

  return validObstructions;
}

int main(int argc, char* argv[]) {
  // Check if the input file is provided
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <input_file> [-trace]" << endl;
    return EXIT_FAILURE;
  }

  // Check if the -trace option is provided
  bool trace = false;
  if (argc > 2 && string(argv[2]) == "-trace") { trace = true; }

  ifstream inputFile(argv[1]);
  if (!inputFile) {
    cerr << "Error: Cannot open file " << argv[1] << endl;
    return EXIT_FAILURE;
  }

  // Read the map from the file, and create a copy for each phase
  vector<string> map_visited, map_obstructions;
  string line;
  while (getline(inputFile, line)) {
    map_visited.push_back(line);
    map_obstructions.push_back(line);
  }
  inputFile.close();

  // Phase 1: Simulate the guard's patrol and find the number of visited positions
  int visitedCount = simulatePatrol(map_visited);

  // Output the number of visited positions, and the map after the guard's patrol if -trace is provided
  cout << "Number of distinct positions visited: " << visitedCount << endl << endl;
  if (trace) {
    cout << "Map after guard's patrol:" << endl;
    for (const auto& row : map_visited) { cout << row << endl; }
  }

  if (trace) {
    cout << endl << "--------------------------------" << endl << endl;
  }

  // Phase 2: Find all possible obstruction positions that cause a loop
  if (map_obstructions.size() > 100) { cout << "Warning: This phase may take a while to complete." << endl; }
  vector<pair<int, int>> obstructions = findLoopCausingObstructions(map_obstructions);

  // Output the number of valid obstruction positions, and the positions if -trace is provided
  cout << "Number of valid obstruction positions: " << obstructions.size() << endl;
  if (trace) {
    cout << endl;
    cout << "Valid obstruction positions:" << endl;
    for (const auto& obstruction : obstructions) {
      cout << "(" << obstruction.first << ", " << obstruction.second << ")" << endl;
    }
  }

  return EXIT_SUCCESS;
}
