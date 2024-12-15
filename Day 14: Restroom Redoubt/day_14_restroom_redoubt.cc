/**
 * AoC 2024: Day 14: Restroom Redoubt
 * @file   day_14_restroom_redoubt.cc
 * @author Raúl Gonzalez Acosta
 * @date   14/12/2024
 * @brief  Simulates the motion of robots in a restroom.
 * @see    https://adventofcode.com/2024/day/14
 */

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <tuple>
#include <fstream>
#include <limits>

using namespace std;

struct Robot {
  int x, y;    // Position (p)
  int vx, vy;  // Velocity (v)
};

// Parse input to extract robot positions and velocities
vector<Robot> parseInput(const string& filename) {
  ifstream inputFile(filename);
  if (!inputFile) {
    cerr << "Error: Cannot open file " << filename << endl;
    exit(EXIT_FAILURE);
  }

  vector<Robot> robots;
  string line;

  while (getline(inputFile, line)) {
    if (line.empty()) continue;
    Robot robot;
    sscanf(line.c_str(), "p=%d,%d v=%d,%d", &robot.x, &robot.y, &robot.vx, &robot.vy);
    robots.push_back(robot);
  }

  inputFile.close();

  return robots;
}

// Simulate the motion of robots for the given number of seconds
vector<vector<int>> simulate(const vector<Robot>& robots, int width, int height, int seconds) {
  vector<vector<int>> grid(height, vector<int>(width, 0));

  for (const Robot& robot : robots) {
    // Calculate final position after wrapping
    int finalX = (robot.x + robot.vx * seconds) % width;
    int finalY = (robot.y + robot.vy * seconds) % height;

    // Handle negative wrapping
    if (finalX < 0) finalX += width;
    if (finalY < 0) finalY += height;

    // Increment the count of robots on the resulting tile
    grid[finalY][finalX]++;
  }

  return grid;
}

// Count robots in each quadrant and calculate the safety factor
int calculateSafetyFactor(const vector<vector<int>>& grid, int width, int height) {
  int midX = width / 2;
  int midY = height / 2;

  // Quadrant counts
  int q1 = 0, q2 = 0, q3 = 0, q4 = 0;

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if (x == midX || y == midY || grid[y][x] == 0) continue;

      if (x < midX && y < midY) q1 += grid[y][x]; // Top-left
      else if (x >= midX && y < midY) q2 += grid[y][x]; // Top-right
      else if (x < midX && y >= midY) q3 += grid[y][x]; // Bottom-left
      else if (x >= midX && y >= midY) q4 += grid[y][x]; // Bottom-right
    }
  }

  // Safety factor is the product of all quadrant counts
  return q1 * q2 * q3 * q4;
}

// Calculate the bounding box of the robots' positions
tuple<int, int, int, int> calculateBoundingBox(const vector<Robot>& robots) {
  int minX = numeric_limits<int>::max();
  int maxX = numeric_limits<int>::min();
  int minY = numeric_limits<int>::max();
  int maxY = numeric_limits<int>::min();

  for (const Robot& robot : robots) {
    minX = min(minX, robot.x);
    maxX = max(maxX, robot.x);
    minY = min(minY, robot.y);
    maxY = max(maxY, robot.y);
  }

  return {minX, maxX, minY, maxY};
}

// Update robot positions for one second
void updatePositions(vector<Robot>& robots) {
  for (Robot& robot : robots) {
    robot.x += robot.vx;
    robot.y += robot.vy;
  }
}

// Find the time when the robots are most compact
int findEasterEgg(vector<Robot> robots) {
  int seconds = 0;
  int minArea = numeric_limits<int>::max();
  int bestTime = 0;

  while (true) {
    // Calculate bounding box
    auto [minX, maxX, minY, maxY] = calculateBoundingBox(robots);
    int width = maxX - minX + 1;
    int height = maxY - minY + 1;
    int area = width * height;

    // If the area starts increasing, we've passed the most compact state
    if (area > minArea) {
      auto [minXBest, maxXBest, minYBest, maxYBest] = calculateBoundingBox(robots);
      break;
    }

    // Update minimum area and best time
    minArea = area;
    bestTime = seconds;

    // Update robot positions
    updatePositions(robots);
    ++seconds;
  }

  return bestTime;
}

int main(int argc, char* argv[]) {
  // Check if the input file is provided
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <input_file>" << endl;
    return EXIT_FAILURE;
  }

  string filename = argv[1];

  const int width = 101;
  const int height = 103;
  const int seconds = 100;

  // Parse input robots
  vector<Robot> robots = parseInput(filename);

  // Simulate the motion
  vector<vector<int>> grid = simulate(robots, width, height, seconds);

  // Calculate the safety factor
  int safetyFactor = calculateSafetyFactor(grid, width, height);
  // Output the safety factor result
  cout << "Safety factor: " << safetyFactor << endl;

  // Find the Easter egg
  robots = parseInput(filename);
  int easterEgg = findEasterEgg(robots);
  // Output the Easter egg result
  cout << "Easter egg found at second " << easterEgg << endl;

  return 0;
}
