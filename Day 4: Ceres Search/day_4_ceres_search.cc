#include <iostream>
#include <fstream>
#include <vector>
#include <string>

const std::string WORD = "XMAS";

// Directions: {row offset, column offset}
const std::vector<std::pair<int, int>> DIRECTIONS = {
  {0, 1},   // Right
  {1, 0},   // Down
  {1, 1},   // Down-right
  {1, -1},  // Down-left
  {0, -1},  // Left
  {-1, 0},  // Up
  {-1, -1}, // Up-left
  {-1, 1}   // Up-right
};

// Function to check if a word starts at grid[row][col] in a specific direction
bool isWordAtPosition(const std::vector<std::string>& grid, int row, int col, int dRow, int dCol) {
  int rows = grid.size();
  int cols = grid[0].size();

  for (int i = 0; i < WORD.size(); ++i) {
    int newRow = row + i * dRow;
    int newCol = col + i * dCol;
    if (newRow < 0 || newRow >= rows || 
        newCol < 0 || newCol >= cols || 
        grid[newRow][newCol] != WORD[i]) 
      { return false; }
  }
  return true;
}

// Function to find all instances of "XMAS" in the grid
int findAllOccurrences(const std::vector<std::string>& grid, std::vector<std::string>& highlightedGrid) {
  int rows = grid.size();
  int cols = grid[0].size();
  int count = 0;

  // Initialize highlighted grid with dots
  highlightedGrid = std::vector<std::string>(rows, std::string(cols, '.'));

  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      // Check for "XMAS" starting from this position in all directions
      for (const auto& dir : DIRECTIONS) {
        if (isWordAtPosition(grid, row, col, dir.first, dir.second)) {
          count++;
          // Highlight the word in the result grid
          for (int i = 0; i < WORD.size(); ++i) {
            int newRow = row + i * dir.first;
            int newCol = col + i * dir.second;
            highlightedGrid[newRow][newCol] = grid[newRow][newCol];
          }
        }
      }
    }
  }
  return count;
}

// Function to check if an X-MAS pattern exists at a given position
bool isXMASPattern(const std::vector<std::string>& grid, int row, int col) {
  int rows = grid.size();
  int cols = grid[0].size();

  // Check the X-MAS pattern
  return (
    (grid[row - 1][col - 1] == 'M' && grid[row + 1][col + 1] == 'S' && grid[row][col] == 'A' || 
     grid[row - 1][col - 1] == 'S' && grid[row + 1][col + 1] == 'M' && grid[row][col] == 'A') && 
    (grid[row - 1][col + 1] == 'M' && grid[row + 1][col - 1] == 'S' && grid[row][col] == 'A' || 
     grid[row - 1][col + 1] == 'S' && grid[row + 1][col - 1] == 'M' && grid[row][col] == 'A')
  );
}

// Function to find all X-MAS patterns in the grid
int findAllXMASPatterns(const std::vector<std::string>& grid, std::vector<std::string>& highlightedGrid) {
  int rows = grid.size();
  int cols = grid[0].size();
  int count = 0;

  // Initialize highlighted grid with dots
  highlightedGrid = std::vector<std::string>(rows, std::string(cols, '.'));

  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      if (row == 0 || row == rows - 1 || col == 0 || col == cols - 1) { continue; }
      else if (isXMASPattern(grid, row, col)) {
        count++;
        // Highlight the X-MAS pattern
        highlightedGrid[row][col] = grid[row][col];
        highlightedGrid[row - 1][col - 1] = grid[row - 1][col - 1];
        highlightedGrid[row + 1][col + 1] = grid[row + 1][col + 1];
        highlightedGrid[row - 1][col + 1] = grid[row - 1][col + 1];
        highlightedGrid[row + 1][col - 1] = grid[row + 1][col - 1];
      }
    }
  }

  return count;
}

int main(int argc, char* argv[]) {
  // Check if the input file is provided
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file> [-trace]" << std::endl;
    return EXIT_FAILURE;
  }

  // Check if the -trace option is provided
  bool trace = false;
  if (argc > 2 && std::string(argv[2]) == "-trace") { trace = true;}

  // Open the input file
  std::ifstream inputFile(argv[1]);
  if (!inputFile) {
    std::cerr << "Error: Could not open file " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<std::string> grid;

  // Read the grid from the file
  std::string line;
  while (std::getline(inputFile, line)) { grid.push_back(line); }
  inputFile.close();

  // Check if the grid is empty
  if (grid.empty()) {
    std::cerr << "Error: Input file is empty or invalid." << std::endl;
    return EXIT_FAILURE;
  }

  // Grid to store the highlighted occurrences
  std::vector<std::string> highlightedGrid;

  // Phase 1: Find all occurrences of "XMAS"
  int count = findAllOccurrences(grid, highlightedGrid);
  std::cout << "Total occurrences of 'XMAS': " << count << std::endl;
  if (trace) {
    std::cout << std::endl << "Highlighted Grid:" << std::endl;
    for (const auto& row : highlightedGrid) { std::cout << row << std::endl; }
    std::cout << std::endl;
  }

  std::cout << "--------------------------------" << std::endl;
  if (trace) { std::cout << std::endl; }

  // Phase 2: Find all X-MAS patterns
  count = findAllXMASPatterns(grid, highlightedGrid);
  std::cout << "Total occurrences of 'X-MAS': " << count << std::endl;
  if (trace) {
    std::cout << std::endl << "Highlighted Grid:" << std::endl;
    for (const auto& row : highlightedGrid) { std::cout << row << std::endl; }
  }

  return EXIT_SUCCESS;
}
