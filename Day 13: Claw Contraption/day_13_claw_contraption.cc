/**
 * AoC 2024: Day 13 Claw Contraption
 * @file   day_13_claw_contraption.cc
 * @author Raúl Gonzalez Acosta
 * @date   13/12/2024
 * @brief  Simulates a claw machine to win a prize.
 * @see    https://adventofcode.com/2024/day/13
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>

using namespace std;

// Structure to store machine details
struct Machine {
  int dxA, dyA;  // Button A movement
  int dxB, dyB;  // Button B movement
  int targetX, targetY;  // Prize location
};

// Function to parse input file
vector<Machine> parseInput(const string& filename) {
  ifstream inputFile(filename);
  if (!inputFile) {
    std::cerr << "Error: Cannot open file " << filename << std::endl;
    exit(EXIT_FAILURE);
  }

  vector<Machine> machines;
  string line;

  while (getline(inputFile, line)) {
    if (line.empty()) { continue; }

    Machine machine;
    sscanf(line.c_str(), "Button A: X+%d, Y+%d", &machine.dxA, &machine.dyA);
    getline(inputFile, line);
    sscanf(line.c_str(), "Button B: X+%d, Y+%d", &machine.dxB, &machine.dyB);
    getline(inputFile, line);
    sscanf(line.c_str(), "Prize: X=%d, Y=%d", &machine.targetX, &machine.targetY);
    
    machines.push_back(machine);
    getline(inputFile, line); // Skip empty line
  }

  return machines;
}

void SolveMachine(const Machine& machine, int& totalCost) {
  int minCost = numeric_limits<int>::max();
  bool canWin = false;

  // Brute-force through possible presses for A and B
  for (int a = 0; a <= 100; ++a) {
    for (int b = 0; b <= 100; ++b) {
      int moveX = a * machine.dxA + b * machine.dxB;
      int moveY = a * machine.dyA + b * machine.dyB;

      if (moveX == machine.targetX && moveY == machine.targetY) {
        canWin = true;
        int cost = 3 * a + 1 * b;
        minCost = min(minCost, cost);
      }
    }
  }

  if (canWin) { totalCost += minCost; }
}

void solveWithCorrectedPrizeCoordinates(const Machine& machine, long long& bigtotalCost) {
  int64_t correctedTargetX = machine.targetX + 10000000000000;
  int64_t correctedTargetY = machine.targetY + 10000000000000;

  int64_t minCost = numeric_limits<int64_t>::max();
  bool canWin = false;

  // Search through a much larger range of possible presses
  for (int64_t a = 0; a <= 10000; ++a) {
    for (int64_t b = 0; b <= 10000; ++b) {
      int64_t moveX = a * machine.dxA + b * machine.dxB;
      int64_t moveY = a * machine.dyA + b * machine.dyB;

      if (moveX == correctedTargetX && moveY == correctedTargetY) {
        canWin = true;
        int64_t cost = 3 * a + b;
        minCost = min(minCost, cost);
      }
    }
  }

  if (canWin) { bigtotalCost += minCost; }
}


int main(int argc, char* argv[]) {
  // Check if the input file is provided
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
    return EXIT_FAILURE;
  }

  string filename = argv[1];
  vector<Machine> machines = parseInput(filename);

  int totalCost = 0;
  // Solve each machine
  for (const Machine& machine : machines) { SolveMachine(machine, totalCost); }
  // Output results
  cout << "Total tokens spent (bad measurements): " << totalCost << endl;

  long long bigtotalCost = 0;
  // Solve each machine with corrected prize coordinates
  for (const Machine& machine : machines) { solveWithCorrectedPrizeCoordinates(machine, bigtotalCost); }
  // Output results
  cout << "Total tokens spent (good measurements): " << totalCost << endl;

  return 0;
}