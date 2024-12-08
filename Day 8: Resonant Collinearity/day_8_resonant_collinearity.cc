/**
 * AoC 2024: Day 8: Resonant Collinearity

 * @file   day_8_resonant_collinearity.cc
 * @author Raúl Gonzalez Acosta
 * @date   08/12/2024
 * @brief  Calculate the number of unique antinodes in a given map.
 * @see    https://adventofcode.com/2024/day/8
 */

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <fstream>
#include <cctype>
#include <utility>
#include <numeric>
#include <unordered_set>
#include <unordered_map>

// Function to parse the map and extract antennas by frequency
std::map<char, std::vector<std::pair<int, int>>> parseAntennas(const std::vector<std::string>& map) {
    std::map<char, std::vector<std::pair<int, int>>> antennas;
    for (std::size_t i = 0; i < map.size(); ++i) {
        for (std::size_t j = 0; j < map[i].size(); ++j) {
            char c = map[i][j];
            if (std::isalnum(c)) {
                antennas[c].emplace_back(static_cast<int>(i), static_cast<int>(j));
            }
        }
    }
    return antennas;
}

// Function to calculate antinodes from antennas
std::set<std::pair<int, int>> calculateAntinodes(
    const std::map<char, std::vector<std::pair<int, int>>>& antennas, 
    int rows, int cols
) {
    std::set<std::pair<int, int>> antinodes;

    for (const auto& [frequency, positions] : antennas) {
        std::size_t n = positions.size();

        for (std::size_t i = 0; i < n; ++i) {
            for (std::size_t j = i + 1; j < n; ++j) {
                auto [x1, y1] = positions[i];
                auto [x2, y2] = positions[j];

                // Midpoint between antennas
                int mx = (x1 + x2) / 2;
                int my = (y1 + y2) / 2;

                // Check if the midpoint is an integer point and lies within bounds
                if ((x1 + x2) % 2 == 0 && (y1 + y2) % 2 == 0 && mx >= 0 && mx < rows && my >= 0 && my < cols) {
                    antinodes.emplace(mx, my);
                }

                // Calculate extrapolated positions
                int dx = x2 - x1;
                int dy = y2 - y1;

                int ex1x = x1 - dx;
                int ex1y = y1 - dy;
                int ex2x = x2 + dx;
                int ex2y = y2 + dy;

                // Check if extrapolated positions are within bounds
                if (ex1x >= 0 && ex1x < rows && ex1y >= 0 && ex1y < cols) {
                    antinodes.emplace(ex1x, ex1y);
                }
                if (ex2x >= 0 && ex2x < rows && ex2y >= 0 && ex2y < cols) {
                    antinodes.emplace(ex2x, ex2y);
                }
            }
        }
    }

    return antinodes;
}

// Custom hash function for std::pair<int, int>
struct PairHash {
  template <typename T1, typename T2>
  size_t operator()(const std::pair<T1, T2>& p) const {
    return std::hash<T1>()(p.first) ^ (std::hash<T2>()(p.second) << 1);
  }
};

// Main function to compute unique antinodes
int countUniqueAntinodes(const std::vector<std::string>& grid) {
  int rows = grid.size();
  int cols = grid[0].size();

  std::unordered_map<char, std::vector<std::pair<int, int>>> antennas;

  // Collect positions of all antennas
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      char cell = grid[r][c];
      if (isalnum(cell)) {
        antennas[cell].emplace_back(r, c);
      }
    }
  }

  std::unordered_set<std::pair<int, int>, PairHash> uniqueAntinodes;

  // Check for antinodes for each frequency
  for (const auto& [freq, positions] : antennas) {
    int n = positions.size();
    if (n < 2) {
      continue;
    }

    // Add all antenna positions themselves as antinodes
    for (const auto& pos : positions) {
      uniqueAntinodes.emplace(pos);
    }

    // Check all pairs of positions to find collinear antinodes
    for (int i = 0; i < n; ++i) {
      for (int j = i + 1; j < n; ++j) {
        int r1 = positions[i].first, c1 = positions[i].second;
        int r2 = positions[j].first, c2 = positions[j].second;

        // Calculate the step direction for all points along the line
        int dr = r2 - r1;
        int dc = c2 - c1;
        int stepGcd = std::gcd(dr, dc);  // Standard GCD to normalize the step
        int stepR = dr / stepGcd;
        int stepC = dc / stepGcd;

        // Add all points along the line between the two antennas
        int currentR = r1, currentC = c1;
        while (currentR >= 0 && currentR < rows && currentC >= 0 &&
               currentC < cols) {
          uniqueAntinodes.emplace(currentR, currentC);
          currentR += stepR;
          currentC += stepC;
        }

        // Reset and go in the opposite direction
        currentR = r1 - stepR;
        currentC = c1 - stepC;
        while (currentR >= 0 && currentR < rows && currentC >= 0 &&
               currentC < cols) {
          uniqueAntinodes.emplace(currentR, currentC);
          currentR -= stepR;
          currentC -= stepC;
        }
      }
    }
  }

  // Return the number of unique antinodes
  return uniqueAntinodes.size();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile) {
        std::cerr << "Error: Cannot open file " << argv[1] << "\n";
        return 1;
    }

    // Read the map from the file
    std::vector<std::string> map;
    std::string line;
    while (std::getline(inputFile, line)) {
        map.push_back(line);
    }
    inputFile.close();

    int rows = static_cast<int>(map.size());
    int cols = static_cast<int>(map[0].size());

    // Parse antennas and calculate antinodes
    auto antennas = parseAntennas(map);
    auto antinodes = calculateAntinodes(antennas, rows, cols);

    // Output results for original calculation
    std::cout << "Number of unique antinodes (original rules): " << antinodes.size() << std::endl;

    // Calculate and output results for updated rules
    auto updatedAntinodes = countUniqueAntinodes(map);
    std::cout << "Number of unique antinodes (updated rules): " << updatedAntinodes << std::endl;

    return 0;
}
