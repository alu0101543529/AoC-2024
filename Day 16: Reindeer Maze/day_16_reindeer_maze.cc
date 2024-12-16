/**
 * AoC 2024: Day 16: Reindeer Maze
 * @file   day_16_reindeer_maze.cc
 * @author Raúl Gonzalez Acosta
 * @date   16/12/2024
 * @brief  Finds the shortest path in a maze.
 * @see    https://adventofcode.com/2024/day/16
 */

#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <string>
#include <limits>
#include <fstream>
#include <set>

using namespace std;

struct State {
  int row, col, direction, score;

  bool operator>(const State& other) const { return score > other.score; }
};

// Directions: 0 = East, 1 = South, 2 = West, 3 = North
const int dr[4] = {0, 1, 0, -1};
const int dc[4] = {1, 0, -1, 0};
const char dirSymbols[4] = {'>', 'v', '<', '^'};

// Finds the minimum score and tracks the best path tiles
int findMinScore(const vector<string>& maze, pair<int, int> start, pair<int, int> end, vector<vector<bool>>& bestPathTiles) {
  int rows = maze.size();
  int cols = maze[0].size();

  // Priority queue for Dijkstra's algorithm
  priority_queue<State, vector<State>, greater<State>> pq;

  // Distance array: 4 directions at each cell
  vector<vector<vector<int>>> dist(rows, vector<vector<int>>(cols, vector<int>(4, numeric_limits<int>::max())));

  // Initialize starting point
  for (int d = 0; d < 4; d++) {
    dist[start.first][start.second][d] = 0;
    pq.push({start.first, start.second, d, 0});
  }

  // Stores the previous state for backtracking paths
  vector<vector<vector<pair<int, int>>>> prev(rows, vector<vector<pair<int, int>>>(cols, vector<pair<int, int>>(4, {-1, -1})));

  int minScore = -1;

  while (!pq.empty()) {
    State current = pq.top();
    pq.pop();

    int r = current.row;
    int c = current.col;
    int dir = current.direction;
    int score = current.score;

    // Skip if this state is already worse than known
    if (score > dist[r][c][dir]) continue;

    // If we've reached the end, capture the score
    if (r == end.first && c == end.second) {
      minScore = score;
      break; // Stop as we only need the minimum score
    }

    // Move forward
    int nr = r + dr[dir];
    int nc = c + dc[dir];
    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && maze[nr][nc] != '#') {
      if (dist[nr][nc][dir] > score + 1) {
        dist[nr][nc][dir] = score + 1;
        pq.push({nr, nc, dir, score + 1});
        prev[nr][nc][dir] = {r, c};
      }
    }

    // Rotate clockwise
    int newDir = (dir + 1) % 4;
    if (dist[r][c][newDir] > score + 1000) {
      dist[r][c][newDir] = score + 1000;
      pq.push({r, c, newDir, score + 1000});
      prev[r][c][newDir] = {r, c};
    }

    // Rotate counterclockwise
    newDir = (dir + 3) % 4;
    if (dist[r][c][newDir] > score + 1000) {
      dist[r][c][newDir] = score + 1000;
      pq.push({r, c, newDir, score + 1000});
      prev[r][c][newDir] = {r, c};
    }
  }

  // Backtrack to mark the best path tiles
  set<pair<int, int>> bestPathSet;
  for (int d = 0; d < 4; ++d) {
  if (dist[end.first][end.second][d] == minScore) {
    int r = end.first, c = end.second, dir = d;
    while (r != -1 && c != -1) {
      bestPathSet.insert({r, c});
      tie(r, c) = prev[r][c][dir];
    }
  }
  }

  // Mark the best path tiles
  for (const auto& [r, c] : bestPathSet) { bestPathTiles[r][c] = true; }

  return minScore;
}

void displayMazeWithPaths(const vector<string>& maze, const vector<vector<bool>>& bestPathTiles) {
  int rows = maze.size();
  int cols = maze[0].size();

  int pathCount = 0;
  vector<string> mazeWithPaths = maze;

  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      if (bestPathTiles[r][c]) {
        mazeWithPaths[r][c] = 'O';
        pathCount++;
      }
    }
  }

  cout << "Maze with Best Paths Marked:" << endl;
  for (const string& row : mazeWithPaths) { cout << row << endl; }
  cout << "Number of tiles part of at least one best path: " << pathCount << '\n';
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

  string filename = argv[1];

  ifstream inputFile(filename);
  if (!inputFile) {
    cerr << "Error: Cannot open file " << filename << endl;
    return EXIT_FAILURE;
  }

  vector<string> maze;
  string line;
  pair<int, int> start, end;

  int row = 0;
  while (getline(inputFile, line)) {
    maze.push_back(line);

    // Locate start (S) and end (E)
    size_t startPos = line.find('S');
    if (startPos != string::npos) start = {row, static_cast<int>(startPos)};
    size_t endPos = line.find('E');
    if (endPos != string::npos) end = {row, static_cast<int>(endPos)};

    row++;
  }

  inputFile.close();

  // Prepare best path tracking
  vector<vector<bool>> bestPathTiles(maze.size(), vector<bool>(maze[0].size(), false));

  // Solve and mark the best paths
  int minScore = findMinScore(maze, start, end, bestPathTiles);

  // Display results
  cout << "Lowest possible score: " << minScore << endl;
  displayMazeWithPaths(maze, bestPathTiles);

  return EXIT_SUCCESS;
}