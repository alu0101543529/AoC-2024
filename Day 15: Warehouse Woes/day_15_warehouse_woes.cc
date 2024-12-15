/**
 * AoC 2024: Day 15: Warehouse Woes
 * @file   day_15_warehouse_woes.cc
 * @author Raúl Gonzalez Acosta
 * @date   15/12/2024
 * @brief  Simulates a robot moving boxes in a warehouse.
 * @see    https://adventofcode.com/2024/day/15
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Position { int row, col; };

class Warehouse {
 public:
  void readFromFile(const string& filename) {
    ifstream inputFile(filename);
    if (!inputFile) {
      cerr << "Error: Cannot open file " << filename << endl;
      exit(EXIT_FAILURE);
    }

    string line;
    bool readingGrid = true;
    while (getline(inputFile, line)) {
      if (line.empty()) {
        readingGrid = false;
        continue;
      }

      if (readingGrid) {
        grid.push_back(line);
        size_t robotPos = line.find('@');
        if (robotPos != string::npos) { robot = {static_cast<int>(grid.size()) - 1, static_cast<int>(robotPos)}; }
      } else { moves += line; }
    }

    inputFile.close();
  }

  void simulate(const bool& trace) {
    for (char move : moves) { moveRobot(move); }
    if (trace) {
      cout << "Final Grid:" << endl;
      for (const string& row : grid) { cout << row << endl; }
    }
    cout << "Sum of GPS coordinates: " << calculateGPS() << endl;

    // Prepare for wider warehouse simulation
    expandGrid();
    if (trace) {
      cout << endl;
      cout << "Expanded Warehouse Map:" << endl;
      for (const string& row : grid) { cout << row << endl; }
    }

    // Simulate for wide warehouse (same robot moves)
    for (char move : moves) { moveRobot(move); }

    if (trace) {
      cout << endl;
      cout << "Expanded Final Grid:" << endl;
      for (const string& row : grid) { cout << row << endl; }
    }
    cout << "Sum of GPS coordinates for wide warehouse: " << calculateGPSForWideGrid() << endl;
  }

 private:
  vector<string> grid;
  string moves;
  Position robot;

  bool isWall(int row, int col) const { return grid[row][col] == '#'; }

  bool isBox(int row, int col) const { return grid[row][col] == 'O'; }

  void moveRobot(char direction) {
    int dr = 0, dc = 0;
    if (direction == '^') dr = -1;
    else if (direction == 'v') dr = 1;
    else if (direction == '<') dc = -1;
    else if (direction == '>') dc = 1;

    int nextRow = robot.row + dr;
    int nextCol = robot.col + dc;

    if (isWall(nextRow, nextCol)) return;

    if (isBox(nextRow, nextCol)) {
      int boxNextRow = nextRow + dr;
      int boxNextCol = nextCol + dc;

      if (!isWall(boxNextRow, boxNextCol) && !isBox(boxNextRow, boxNextCol)) {
        grid[nextRow][nextCol] = '@';
        grid[boxNextRow][boxNextCol] = 'O';
        grid[robot.row][robot.col] = '.';
        robot = {nextRow, nextCol};
      }
    } else {
      grid[nextRow][nextCol] = '@';
      grid[robot.row][robot.col] = '.';
      robot = {nextRow, nextCol};
    }
  }

  int calculateGPS() const {
    int gpsSum = 0;
    for (int row = 0; row < grid.size(); ++row) {
      for (int col = 0; col < grid[row].size(); ++col) {
        if (grid[row][col] == 'O') { gpsSum += 100 * row + col; }
      }
    }

    return gpsSum;
  }

  void expandGrid() {
    vector<string> newGrid;

    for (const string& row : grid) {
      string newRow;
      for (char tile : row) {
        if (tile == '#') newRow += "##";
        else if (tile == 'O') newRow += "[]";
        else if (tile == '.') newRow += "..";
        else if (tile == '@') newRow += "@.";
      }
      newGrid.push_back(newRow);
    }

    grid = newGrid;
  }

  int calculateGPSForWideGrid() const {
    int gpsSum = 0;
    for (int row = 0; row < grid.size(); ++row) {
      for (int col = 0; col < grid[row].size(); ++col) {
        if (grid[row][col] == '[') { gpsSum += 100 * row + col; }
      }
    }
    return gpsSum;
  }
};

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

  Warehouse warehouse;
  warehouse.readFromFile(filename);
  warehouse.simulate(trace);

  return EXIT_SUCCESS;
}
