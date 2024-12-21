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

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

struct PointHash {
    size_t operator()(const Point& p) const {
        return hash<int>()(p.x) ^ hash<int>()(p.y);
    }
};

// Directions for movement: {dx, dy}
const vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

// Function to parse input from a file
void parseInput(const string& filename, vector<string>& grid, Point& start, Point& end) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error: Could not open file " << filename << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    while (getline(infile, line)) {
        grid.push_back(line);
        for (size_t i = 0; i < line.size(); ++i) {
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
            if (next.x >= 0 && next.x < grid.size() && next.y >= 0 && next.y < grid[0].size() &&
                grid[next.x][next.y] != '#' && !visited[next]) {
                visited[next] = true;
                q.push({next, steps + 1});
            }
        }
    }
    return -1; // No path found
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

        if (current == end) return steps;

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
    return -1; // No path found
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
      cerr << "Usage: " << argv[0] << " <input_file>" << endl;
      return EXIT_FAILURE;
  }

  vector<string> grid;
  Point start, end;
  parseInput(argv[1], grid, start, end);

  int partOneResult = bfs(grid, start, end);
  int partTwoResult = bfsWithCheat(grid, start, end, 20);

  cout << "--- Part One ---\n";
  if (partOneResult == -1) {
      cout << "No path found in Part One." << endl;
  } else {
      cout << "Shortest path without cheating: " << partOneResult << " picoseconds." << endl;
  }

  cout << "--- Part Two ---\n";
  if (partTwoResult == -1) {
      cout << "No path found in Part Two." << endl;
  } else {
      cout << "Shortest path with up to 20 cheat steps: " << partTwoResult << " picoseconds." << endl;
  }

  return 0;
}
