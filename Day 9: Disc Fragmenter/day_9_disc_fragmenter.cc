/**
 * AoC 2024: Day 9: Disc Fragmenter

 * @file   day_9_disc_fragmenter.cc
 * @author Raúl Gonzalez Acosta
 * @date   09/12/2024
 * @brief  Calculate the checksum of a disk map after compacting the disk.
 * @see    https://adventofcode.com/2024/day/9
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Function to parse the disk map and generate the initial block representation
std::vector<int> parseDiskMap(const std::string& diskMap) {
  std::vector<int> blocks;
  int fileId = 0;

  for (std::size_t i = 0; i < diskMap.size(); i += 2) {
    int fileLength = (diskMap[i] - '0');
    int freeSpace = (i + 1 < diskMap.size()) ? (diskMap[i + 1] - '0') : 0;

    // Add file blocks
    for (int j = 0; j < fileLength; ++j) { blocks.push_back(fileId); }
    fileId++;
    // Add free space
    for (int j = 0; j < freeSpace; ++j) { blocks.push_back(-1); }
  }

  return blocks;
}

// Function to compact the disk map by moving files to the left
void compactDisk(std::vector<int>& blocks) {
  // Position of the first free block and the rightmost file block
  int leftIndex = 0, rightIndex = (blocks.size() - 1);

  while (leftIndex < rightIndex) {
    // Find the next free block on the left
    while (leftIndex < blocks.size() && blocks[leftIndex] != -1) { leftIndex++; }
    // Find the next file block on the right
    while (rightIndex >= 0 && blocks[rightIndex] == -1) { rightIndex--; }
    // If valid indices, move the block
    if (leftIndex < rightIndex) {
      blocks[leftIndex] = blocks[rightIndex];
      blocks[rightIndex] = -1;
    }
  }
}

// Function to compact the disk map by moving files to the leftmost free space
void compactFreeSpace(std::vector<int>& blocks) {
  int numBlocks = blocks.size();

  // Iterate over blocks from the rightmost file to the left
  for (int i = numBlocks - 1; i >= 0; --i) {
    // Skip free space
    if (blocks[i] == -1) { continue; }

    // Identify the file ID and its size
    int fileId = blocks[i];
    std::vector<int> fileBlocks;
    for (int j = 0; j < numBlocks; ++j) {
      if (blocks[j] == fileId) { fileBlocks.push_back(j); }
    }
    int fileSize = fileBlocks.size();

    // Find the leftmost span of free space that can fit the file
    for (int j = 0; j <= numBlocks - fileSize; ++j) {
      bool canFit = true;
      for (int k = 0; k < fileSize; ++k) {
        if (blocks[j + k] != -1) {
          canFit = false;
          break;
        }
      }

      // If a suitable span is found, move the file
      if (canFit && j < fileBlocks.front()) {
        // Clear old file blocks
        for (int block : fileBlocks) { blocks[block] = -1; }
        // Place the file in the new span
        for (int k = 0; k < fileSize; ++k) { blocks[j + k] = fileId; }
        break;
      }
    }
  }
}

// Function to calculate the checksum
long long calculateChecksum(const std::vector<int>& blocks) {
  long long checksum = 0;
  for (std::size_t i = 0; i < blocks.size(); ++i) {
    // If the block is not free space, add to the checksum 
    if (blocks[i] != -1) { checksum += i * blocks[i]; }
  }
  return checksum;
}

// Function to convert blocks to a string representation
std::string blocksToString(const std::vector<int>& blocks) {
  std::string result;
  for (int block : blocks) {
    if (block == -1) { result += '.'; } 
    else { result += ('0' + block); }
  }
  return result;
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

  std::string diskMap;
  std::getline(inputFile, diskMap);
  inputFile.close();

  // Phase 1: Compact the disk map by moving files to the left
  auto disk_blocks = parseDiskMap(diskMap);
  auto free_space_blocks = disk_blocks;

  compactDisk(disk_blocks);
  long long checksum = calculateChecksum(disk_blocks);
  if (trace) {
    std::cout << "Compacted Disk: " << blocksToString(disk_blocks) << std::endl;
  }
  std::cout << "Checksum (Compacted Disk): " << checksum << std::endl;

  checksum = 0;

  // Phase 2: Compact the free space by moving files to the leftmost free space
  compactFreeSpace(free_space_blocks);
  checksum = calculateChecksum(free_space_blocks);
  if (trace) {
    std::cout << "Compacted Free Space: " << blocksToString(free_space_blocks) << std::endl;
  }
  std::cout << "Checksum (Compacted Free Space): " << checksum << std::endl;

  return EXIT_SUCCESS;
}
